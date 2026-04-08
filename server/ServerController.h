// Связывает модель, представление и сетевой сервер
// Обрабатывает входящие команды от клиентов, управляет сессиями, рассылает сообщения

#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include "common/Protocol.h"
#include <QObject>
#include <QHostAddress>
#include <QMutex>
#include "ServerModel.h"
#include "TcpServer.h"

class ServerView;   // Предварительное объявление

class ServerController : public QObject
{
	Q_OBJECT
public:
	explicit ServerController(ServerModel* model, QObject* parent = nullptr);
	~ServerController();

	void setView(ServerView* view);

	// Запустить сервер на указанном порту и адресе
	void start(quint16 port, const QHostAddress& address = QHostAddress::Any);
	void stop();

public slots:
	void slotNewConnection(qintptr socketDescriptor);
	void slotPacketReceived(qintptr socketDescriptor, const QByteArray& packet);
	void slotClientDisconnected(qintptr socketDescriptor);

private:
	// Обработчики конкретных команд
	void handleAuth(qintptr sock, const QVariantMap& data);
	void handleRegister(qintptr sock, const QVariantMap& data);
	void handleSend(qintptr sock, const QVariantMap& data);
	void handlePing(qintptr sock, const QVariantMap& data);

	// Отправка сообщения всем клиентам, кроме exclude
	void broadcastMessage(const QString& sender, const QString& text, qintptr exclude);

	// Отправка ответа конкретному клиенту
	void sendToClient(qintptr sock, Command cmd, const QVariantMap& data);
	void sendError(qintptr sock, const QString& msg);

	ServerModel* model_;
	TcpServer* tcpServer_;
	ServerView* view_;

	// Соответствие дескриптор сокета -> токен сессии
	QMap<qintptr, QString> sockToToken_;
	QMutex mutex_;
};

#endif




