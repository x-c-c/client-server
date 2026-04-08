// Синглтон Майерса для работы с SQLite. Создаёт таблицы, выполняет запросы пользователей и сессий

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QMutex>

class DatabaseManager : public QObject
{
	Q_Object;
public:
	static DatabaseManager& instance();
	
	// Инициализация БД: открытие, настройка, создание таблиц
	bool init();
	
	// Операции с пользователями
	bool addUser(const QString& username, const QString& passwordHash); //, const QString& email = QString() 
	bool userExists(const QString& username);
	QString getUserPasswordHash(const QString& username);
	
	 // Операции с сессиями (токенами)
	bool createSession(const QString& token, const QString& username);
	bool validateSession(const QString& token, QString& username);
	bool updateSession(const QString& token);
	bool removeSession(const QString& token);
	void cleanExpiredSession(qint64 idleTimeoutSeconds);
private:
	DatabaseManager() = default;
	~DatabaseManager();
	
	bool createTables();	// Создание таблиц users и sessions

	QSqlDatabase db_;
	QMutex mutex_;		// Для потокобезопасного доступа к БД
};


#endif
