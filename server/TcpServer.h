// Управляет входящими соединениями, создаёт обработчики ClientHandler для каждого клиента
// Предоставляет методы для отправки данных конкретному клиенту

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QMap>
#include "ClientHandler.h"

class TcpServer : public QTcpServer
{
	Q_OBJECT
public:
	explicit TcpServer(QObject* parent = nullptr);

	// Отправить пакет клиенту по дескриптору сокета
	void sendToClient(qintptr descriptor, const QByteArray& packet);

	// Принудительно отключить клиента
	void disconnectClient(qintptr descriptor);

signals:
	void signalNewConnectionDescriptor(qintptr socketDescriptor);
	void signalPacketReady(qintptr socketDescriptor, const QByteArray& packet);
	void signalClientDisconnected(qintptr socketDescriptor);

protected:
	// Вызывается при новом входящем подключении
	void incomingConnection(qintptr socketDescriptor) override;

private:
	// Храним указатели на обработчики по дескрипторам
	QMap<qintptr, ClientHandler*> handlers_;
};

#endif
