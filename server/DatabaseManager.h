#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMutex>

class DatabaseManager : public QObject
{
	Q_Object;
public:
	static DatabaseManager& instance();
	bool init();
	
	// operations wits users
	bool addUser(const QString& username, const QString& passwordHash, const QString& email = QString() );	// email is optional
	bool getUser(const QString& username, const QString& passwordHash, const QString& email, qint64& createdAt);
	bool userExists(const QString& username);
	
	//operations with sessions
	bool createSession(const QString& token, const QString& username);
	bool validateSession(const QString& token, QString& username, qint64& lastActivity);
	bool updateSession(const QString& token);
	bool removeSession(const QString& token);
	void cleanExpiredSession(qint64 idleTimeoutSeconds);
private:
	DatabaseManager() = default;
	~DatabaseManager();
	bool createTables();
	QSqlDatabase db_;
	QMutex mutex_;
};


#endif
