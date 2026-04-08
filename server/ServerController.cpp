#include "ServerController.h"
#include "ServerView.h"
#include "common/Serializer.h"
#include "common/PacketHandler.h"
#include "common/InputValidator.h"
#include "common/Logger.h"

ServerController::ServerController(ServerModel* model, QObject* parent) :
	  QObject(parent)
	, model_(model)
	, tcpServer_(new TcpServer(this))
	, view_(nullptr)
{
	// Подключаем сигналы от TcpServer к нашим слотам
	connect(tcpServer_, &TcpServer::signalNewConnectionDescriptor, this, &ServerController::slotNewConnection);
	connect(tcpServer_, &TcpServer::signalPacketReady, this, &ServerController::slotPacketReceived);
	connect(tcpServer_, &TcpServer::signalClientDisconnected, this, &ServerController::slotClientDisconnected);
}

ServerController::~ServerController()
{
	stop();
}

void ServerController::setView(ServerView* view)
{
	view_ = view;
}

void ServerController::start(quint16 port, const QHostAddress& address)
{
	if (!tcpServer_->listen(address, port))
	{
		LOG_ERROR("Failed to start server: {}", tcpServer_->errorString().toStdString());
		if (view_)
		{
			view_->slotAppendLog("Failed to start: " + tcpServer_->errorString());
		}
		return;
	}
	LOG_INFO("Server started on {}:{}", address.toString().toStdString(), port);
	if (view_)
	{
		view_->slotAppendLog(QString("Started on %1:%2").arg(address.toString()).arg(port));
		view_->slotSetServerStatus(true);
	}
}

void ServerController::stop()
{
	tcpServer_->close();
	LOG_INFO("Server stopped");
	if (view_)
	{
		view_->slotAppendLog("Stopped");
		view_->slotSetServerStatus(false);
	}
}

void ServerController::slotNewConnection(qintptr sock)
{
	// Проверяем лимит клиентов
	if (model_->isClientLimitReached())
	{
		LOG_WARN("Client limit reached, rejecting {}", sock);
		sendError(sock, "Server full");
		tcpServer_->disconnectClient(sock);
		return;
	}

	model_->addActiveClient(sock, {});
	LOG_INFO("New client {} (total {})", sock, model_->getActiveClientsCount());
	if (view_)
	{
		view_->slotAppendLog(QString("Client %1 connected").arg(sock));
	}
}

void ServerController::slotPacketReceived(qintptr sock, const QByteArray& packet)
{
	Command cmd;
	QVariantMap data;
	if (!Serializer::deserialize(packet, cmd, data))
	{
		LOG_WARN("Bad packet from {}", sock);
		return;
	}

	switch (cmd)
	{
		case Command::Auth:	handleAuth(sock, data);		break;
		case Command::Register:	handleRegister(sock, data);	break;
		case Command::Send:	handleSend(sock, data);		break;
		case Command::Ping:	handlePing(sock, data);		break;
		default:
			sendError(sock, "Unknown command");
	}
}

void ServerController::handleAuth(qintptr sock, const QVariantMap& data)
{
	QString username = data["username"].toString();
	QString password = data["password"].toString();

	if (!InputValidator::validateUsername(username) || !InputValidator::validatePassword(password))
	{
		sendError(sock, "Invalid credentials");
		return;
	}

	QString token;
	if (model_->authenticateUser(username, password, token))
	{
		QVariantMap reply {{"success", true}, {"token", token}};
		sendToClient(sock, Command::AuthAck, reply);
		{
			QMutexLocker lock(&mutex_);
			sockToToken_[sock] = token;
		}

		LOG_INFO("User '{}' authenticated", username.toStdString());
		if (view_)
		{
			view_->slotAppendLog(QString("User '%1' logged in").arg(username));
		}
	}
	else
	{
		sendError(sock, "Authentication failed");
	}
}

void ServerController::handleRegister(qintptr sock, const QVariantMap& data)
{
	QString username = data["username"].toString();
	QString password = data["password"].toString();

	if (!InputValidator::validateUsername(username) || !InputValidator::validatePassword(password))
	{
		sendError(sock, "Invalid username or password");
		return;
	}

	if (model_->registerUser(username, password))
	{
		sendToClient(sock, Command::RegisterAck, {{"success", true}});
		LOG_INFO("User '{}' registered", username.toStdString());
		if (view_)
		{
			view_->slotAppendLog(QString("User '%1' registered").arg(username));
		}
	}
	else
	{
		sendError(sock, "Username already exists");
	}
}

void ServerController::handleSend(qintptr sock, const QVariantMap& data)
{
	QString token = data["token"].toString();
	QString text = data["text"].toString();

	QString username;
	if (!model_->validateSession(token, username))
	{
		sendError(sock, "Invalid session");
		return;
	}

	if (!InputValidator::validateMessage(text))
	{
		sendError(sock, "Message too long or empty");
		return;
	}

	// Проверяем, что токен соответствует сокету (защита от подмены)
	{
		QMutexLocker lock(&mutex_);
		if (sockToToken_.value(sock) != token)
		{
			sendError(sock, "Token mismatch");
			return;
		}
	}

	// Обновляем активность сессии
	model_->updateSessionActivity(token);

	LOG_INFO("Message from '{}': {}", username.toStdString(), text.toStdString());
	if (view_)
	{
		view_->slotAppendLog(QString("[%1]: %2").arg(username, text));
	}

	// Подтверждаем отправку
	sendToClient(sock, Command::SendAck, {{"success", true}});

	// Рассылаем сообщение всем остальным
	broadcastMessage(username, text, sock);
}

void ServerController::handlePing(qintptr sock, const QVariantMap& data)
{
	Q_UNUSED(data)
	sendToClient(sock, Command::Pong, {});
}

void ServerController::broadcastMessage(const QString& sender, const QString& text, qintptr exclude)
{
	QVariantMap msg {{"sender", sender}, {"text", text}};
	QMutexLocker lock(&mutex_);
	for (auto it = sockToToken_.begin(); it != sockToToken_.end(); ++it)
	{
		if (it.key() != exclude)
		{
			sendToClient(it.key(), Command::Broadcast, msg);
		}
	}
}

void ServerController::sendToClient(qintptr sock, Command cmd, const QVariantMap& data)
{
	QByteArray payload = Serializer::serialize(cmd, data);
	QByteArray packet = PacketHandler::buildPacket(cmd, payload);
	tcpServer_->sendToClient(sock, packet);
}

void ServerController::sendError(qintptr sock, const QString& msg)
{
	sendToClient(sock, Command::Error, {{"message", msg}});
}

void ServerController::slotClientDisconnected(qintptr sock)
{
	{
		QMutexLocker lock(&mutex_);
		sockToToken_.remove(sock);
	}
	model_->removeActiveClient(sock);
	LOG_INFO("Client {} disconnected", sock);
	if (view_)
	{
		view_->slotAppendLog(QString("Client %1 disconnected").arg(sock));
	}
}



