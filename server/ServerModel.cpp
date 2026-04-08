#include "ServerModel.h"
#include "common/ConfigManager.h"
#include "common/Logger.h"
#include "DatabaseManager.h"
#include "common/Argon2Hasher.h"
#include <QUuid>
ServerModel::ServerModel(QObject* parent) : QObject(parent)
{
	maxClients_ = ConfigManager::instance().getMaxClients();
	cleanupTimer_ = new QTimer(this);
	
	connect(cleanupTimer_, &QTimer::timeout, this, &ServerModel::slotCleanExpiredSessions);
	cleanupTimer_->start(300000);	// Каждые 10 минут
}

ServerModel::~ServerModel()
{
	cleanupTimer_->stop();
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
	
	emit signalUserRegistered(username);
		
	return true;
}

bool ServerModel::authenticateUser(const QString& username, const QString& password, QString& outToken)
{
	// Получаем хеш пароля из БД
	QString storedHash = DatabaseManager::instance().getUserPasswordHash(username);
	if (storedHash.isEmpty())
	{
		return false;
	}
	// Проверяем пароль
	if (!Argon2Hasher::verifyPassword(password, storedHash.toUtf8()))
	{
		return false;
	}
	// Генерируем токен сессии (UUID)
	outToken = QUuid::createUuid().toString(QUuid::WithoutBraces);

	// Сохраняем сессию в БД
	DatabaseManager::instance().createSession(outToken, username);

	emit signalUserAuthenticated(username);
	return true;
}

bool ServerModel::validateSession(const QString& token, QString& outUsername)
{
	if (!DatabaseManager::instance().validateSession(token, outUsername))
	{
		return false;
	}
	// Обновляем время последней активности
	DatabaseManager::instance().updateSessionActivity(token);
	return true;
}

void ServerModel::updateSessionActivity(const QString& token)
{
	DatabaseManager::instance().updateSessionActivity(token);
}

void ServerModel::removeSession(const QString& token)
{
	DatabaseManager::instance().removeSession(token);
}

void ServerModel::addActiveClient(qintptr socketDescriptor, const QString& token)
{
	activeClients_.insert(socketDescriptor);
	if (!token.isEmpty())
	{
		updateSessionActivity(token);
	}
}

void ServerModel::removeActiveClient(qintptr socketDescriptor)
{
	activeClients_.remove(socketDescriptor);
}

bool ServerModel::isClientLimitReached() const
{
	return activeClients_.size() >= maxClients_;
}

void ServerModel::slotCleanExpiredSessions()
{
	auto timeout = ConfigManager::instance().getIdleTimeout();
	DatabaseManager::instance().cleanExpiredSessions(timeout.count());
}

int ServerModel::getActiveClientsCount() const
{
	return activeClients_.size();
}




