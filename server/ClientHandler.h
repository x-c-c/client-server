// Отвечает за сетевое взаимодействие с одним клиентом: чтение данных, сборка пакетов, тайм-аут бездействия

#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <chrono>

class ClientHandler : public QObject
{
	Q_OBJECT
public:
	explicit ClientHandler(qintptr socketDescriptor, QObject* parent = nullptr);
	~ClientHandler();

	// Отправить пакет клиенту
	void sendPacket(const QByteArray& packet);

	// Принудительно закрыть соединение
	void disconnectFromHost();

	qintptr getDescriptor() const { return descriptor_; }

signals:
	// Собран полный пакет (после проверки заголовка и checksum)
	void signalPacketReceived(qintptr descriptor, const QByteArray& packet);
	// Клиент отключился
	void signalDisconnected(qintptr descriptor);

private slots:
	void onReadyRead();
	void onDisconnected();
	void onError(QAbstractSocket::SocketError socketError);
	void onIdleTimeout();

private:
	qintptr descriptor_;
	QTcpSocket* socket_;
	QByteArray buffer_;		  // Буфер для накопления данных
	QTimer* idleTimer_;		  // Таймер бездействия
	std::chrono::seconds idleTimeout_;
};

#endif





