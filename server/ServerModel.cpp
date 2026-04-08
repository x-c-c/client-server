#include "ServerModel.h"
#include "common/CongigManager.h"
#include "common/Logger.h"
#include "DatabaseManager.h"
ServerModel::ServerModel(QObject* parent) : QObject(parent)
{
	maxClients_ = CongigManager::instance().getMaxClients();
	//cleanUpTimer_ = new QTimer(this);
}

ServerModel::~ServerModel()
{
	//cleanUpTimer_->stop();
}

bool ServerModel::registerUser(const QString& username, const QString& password)
{
	// Проверяем, не занято ли имя
	if (DatabaseManager::instance().userExists(username))
	{
		return false;
	}
	// Хешируем пароль
	QByteArray hash = Argon2Hasher::hashPassword(password);
	
	if (!DatabaseManager::instance().addUser(username, hash))
	{
		return false;
	}
	
	return true;
}
