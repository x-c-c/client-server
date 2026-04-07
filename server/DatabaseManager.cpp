#include "DatabaseManager.h"

DatabaseManager& DatabaseManager::instance();
{
	static DatabaseManager instance;
	return instance;
}

DatabaseManager::~DatabaseManager()
{
	if (db_.isOpen())
	{
		db_.close();
	}
}
DatabaseManager::init()
{

}
