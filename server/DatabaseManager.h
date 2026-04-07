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
	bool addUser();
	bool getUser();
	bool userExists();
private:
	DatabaseManager() = default;
	~DatabaseManager();
};


#endif DATABASEMANAGER_H
