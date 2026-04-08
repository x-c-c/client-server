#include "TcpServer.h"

TcpServer::TcpServer(QObject* parent) : QTcpServer(parent){}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	// Создаём обработчик для нового клиента
	ClientHandler* handler = new ClientHandler(socketDescriptor, this);
	handlers_[socketDescriptor] = handler;

	// Пробрасываем сигналы от обработчика наружу
	connect(handler, &ClientHandler::signalPacketReceived, this, &TcpServer::signalPacketReady);
	connect(handler, &ClientHandler::signalDisconnected, this, &TcpServer::signalClientDisconnected);

	// При отключении удаляем обработчик из карты
	connect(handler, &ClientHandler::signalDisconnected, this, [this, socketDescriptor](){handlers_.remove(socketDescriptor);} );

	emit signalNewConnectionDescriptor(socketDescriptor);
}

void TcpServer::sendToClient(qintptr descriptor, const QByteArray& packet)
{
	if (handlers_.contains(descriptor))
	{
		handlers_[descriptor]->sendPacket(packet);
	}
}

void TcpServer::disconnectClient(qintptr descriptor)
{
	if (handlers_.contains(descriptor))
	{
		handlers_[descriptor]->disconnectFromHost();
	}
}




