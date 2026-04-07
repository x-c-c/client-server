#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseManager : public QObject
{
	Q_Object;
public:
	static DatabaseManager& instance();
	bool init();
	
	bool addUser(const QString& username, const QString& passwordHash, const QString& email = QString() );	// email is optional
	bool getUser(const QString& username, const QString& passwordHash, const QString& email, qint64& createdAt);
	bool userExists(const QString& username);
private:
	DatabaseManager() = default;
	~DatabaseManager();
};


#endif DATABASEMANAGER_H
