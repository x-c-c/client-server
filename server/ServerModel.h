// Содержит логику сервера: регистрация, аутентификация, управление сессиями, подсчёт активных клиентов, ограничения

#ifndef SERVERMODEL_H
#define SERVERMODEL_H

#include <QObject>
#include <QSet>
#include <QTimer>
class ServerModel : public QObject
{
	Q_OBJECT
public:
	explicit ServerModel(QObject* parent = nullptr);
	~ServerModel();
	
	// Регистрация нового пользователя
	bool registerUser(const QString& username, const QString& password);
	
	// Аутентификация: при успехе возвращает true и генерирует токен сессии
	bool authenticateUser(const QString& username, const QString& password, QString& outToken);
	
	// Проверка валидности токена сессии
	bool validateSession(const QString& token, QString& outUsername);
	void updateSessionActivity(const QString& token);
	void removeSession(const QString& token);
	
	// Управление активными клиентами
	void addActiveClient(qintptr socketDescriptor, const QString& token);
	void removeActiveClient(qintptr socketDescriptor);
	bool isClientLimitReached() const;
	int getActiveClientsCount() const;
	
signals:
	void signalUserRegistered(const QString& username);
	void signalUserAuthenticated(const QString& username);
	void signalSessionExpired(const QString& token);
private slots:
	// Периодическая очистка истёкших сессий из БД
	void slotCleanExpiredSessions();
private:
	QSet<qintptr> activeClients_;	// Множество дескрипторов подключённых клиентов
	int maxClients_;		// Максимальное число клиентов (из конфига)
	QTimer* cleanupTimer_;		// Таймер для очистки сессий
};


#endif
