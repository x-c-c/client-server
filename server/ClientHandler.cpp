#include "ClientHandler.h"
#include "common/PacketHandler.h"
#include "common/Logger.h"
#include "common/ConfigManager.h"
#include <QDataStream>
ClientHandler::ClientHandler(qintptr socketDescriptor, QObject* parent)
	: QObject(parent)
	, descriptor_(socketDescriptor)
	, socket_(new QTcpSocket(this))
	, idleTimer_(new QTimer(this))
	, idleTimeout_(ConfigManager::instance().getIdleTimeout())
{
	socket_->setSocketDescriptor(descriptor_);

	// Подключаем сигналы сокета
	connect(socket_, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
	connect(socket_, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);
	connect(socket_, &QTcpSocket::errorOccurred, this, &ClientHandler::onError);

	// Настраиваем таймер бездействия
	idleTimer_->setSingleShot(true);
	connect(idleTimer_, &QTimer::timeout, this, &ClientHandler::onIdleTimeout);
	idleTimer_->start(std::chrono::duration_cast<std::chrono::milliseconds>(idleTimeout_).count());

	LOG_DEBUG("ClientHandler created for descriptor {}", descriptor_);
}

ClientHandler::~ClientHandler()
{
	if (socket_->state() == QTcpSocket::ConnectedState)
	{
		socket_->disconnectFromHost();
	}
	delete socket_;
}

void ClientHandler::sendPacket(const QByteArray& packet)
{
	if (socket_->state() == QTcpSocket::ConnectedState)
	{
		socket_->write(packet);
		socket_->flush();	// Немедленная отправка
	}
}

void ClientHandler::disconnectFromHost()
{
	socket_->disconnectFromHost();
}

void ClientHandler::onReadyRead()
{
	// Сбрасываем таймер бездействия
	idleTimer_->start(std::chrono::duration_cast<std::chrono::milliseconds>(idleTimeout_).count());

	buffer_.append(socket_->readAll());

	// Пытаемся извлечь полные пакеты из буфера
	while (buffer_.size() >= 4)
	{
		QDataStream ds(buffer_.left(4));
		ds.setVersion(QDataStream::Qt_DefaultCompiledVersion);
		quint32 size;
		ds >> size;

		// Проверка на слишком большой размер (защита от атак)
		if (size > PacketHandler::MaxPacketSize)
		{
			LOG_WARN("Oversized packet from client {}, disconnecting", descriptor_);
			socket_->disconnectFromHost();
			return;
		}

		if (static_cast<quint32>(buffer_.size()) < size)
		{
			break;   // Пакет ещё не полностью получен
		}

		QByteArray packet = buffer_.left(size);
		buffer_.remove(0, size);

		Command cmd;
		QByteArray payload;
		if (PacketHandler::parsePacket(packet, cmd, payload))
		{
			emit signalPacketReceived(descriptor_, payload);
		}
		else
		{
			LOG_WARN("Invalid packet from client {}", descriptor_);
		}
	}
}

void ClientHandler::onDisconnected()
{
	LOG_DEBUG("Client {} disconnected", descriptor_);
	emit signalDisconnected(descriptor_);
	deleteLater();   // Планируем удаление этого объекта
}

void ClientHandler::onError(QAbstractSocket::SocketError socketError)
{
	LOG_WARN("Socket error on client {}: {}", descriptor_, socket_->errorString().toStdString());
}

void ClientHandler::onIdleTimeout()
{
	LOG_INFO("Client {} idle timeout, disconnecting", descriptor_);
	socket_->disconnectFromHost();
}




