#include "DatabaseManager.h"
#include "common/ConfigManager.h"
#include "common/ConfigKeys.h"
#include "common/Logger.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>

DatabaseManager& DatabaseManager::instance()
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
bool DatabaseManager::init()
{
	QMutexLocker locker(&mutex_);	// Блокируем на всё время инициализации
	
	auto& config = ConfigManager::instance();
	QString dbFile = config.getString(	ConfigKeys::DatabaseSection,
						ConfigKeys::DatabaseFileKey,
						ConfigKeys::DefaultDatabaseFile);
						
	int timeoutMs = config.getInt(	ConfigKeys::DatabaseSection,
					ConfigKeys::DatabaseTimeoutMsKey,
					ConfigKeys::DefaultDatabaseTimeoutMs);
	
	bool walMode = config.getBool(	ConfigKeys::DatabaseSection,
					ConfigKeys::DatabaseWalModeKey,
					ConfigKeys::DefaultDatabaseWalMode);
	
	// Подключаем драйвер SQLite
	db_ = QSqlDatabase::addDatabase("QSQLITE", "chat_server_connection");
	db_.setDatabaseName(dbFile);
	db_.setConnectOptions(QString("QSQLITE_BUSY_TIMEOUT=%1").arg(timeoutMs));
	
	if (!db_.open())
	{
		LOG_ERROR("Failed to open database: {}", db_.lastError().text().toStdString());
		return false;
	}
	
	// Включаем WAL-режим для лучшей конкурентности
	if (walMode)
	{
		QSqlQuery pragma(db_);
		pragma.exec("PRAGMA journal_mode=WAL");
	}
	return createTables();
}

bool DatabaseManager::createTables()
{
	QSqlQuery query(db_);

	// Таблица пользователей
	bool ok = query.exec
	(
		"CREATE TABLE IF NOT EXISTS users ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"username TEXT UNIQUE NOT NULL,"
		"password_hash TEXT NOT NULL,"
		"created_at INTEGER NOT NULL)"
	);
	if (!ok)
	{
		LOG_ERROR("Failed to create users table: {}", query.lastError().text().toStdString());
		return false;
	}

	// Таблица сессий
	ok = query.exec
	(
		"CREATE TABLE IF NOT EXISTS sessions ("
		"token TEXT PRIMARY KEY,"
		"username TEXT NOT NULL,"
		"last_activity INTEGER NOT NULL,"
		"FOREIGN KEY(username) REFERENCES users(username) ON DELETE CASCADE)"
	);
	if (!ok)
	{
		LOG_ERROR("Failed to create sessions table: {}", query.lastError().text().toStdString());
		return false;
	}

	// Индексы для ускорения поиска
	query.exec("CREATE INDEX IF NOT EXISTS idx_sessions_username ON sessions(username)");
	query.exec("CREATE INDEX IF NOT EXISTS idx_sessions_last_activity ON sessions(last_activity)");
	return true;
}

bool DatabaseManager::addUser(const QString& username, const QString& passwordHash)
{
	QMutexLocker locker(&mutex_);
	QSqlQuery query(db_);
	query.prepare("INSERT INTO users (username, password_hash, created_at) VALUES (?, ?, ?)");
	query.addBindValue(username);
	query.addBindValue(passwordHash);
	query.addBindValue(QDateTime::currentSecsSinceEpoch());
	return query.exec();
}

bool DatabaseManager::userExists(const QString& username)
{
	QMutexLocker locker(&mutex_);
	QSqlQuery query(db_);
	query.prepare("SELECT 1 FROM users WHERE username = ?");
	query.addBindValue(username);
	return query.exec() && query.next();
}

QString DatabaseManager::getUserPasswordHash(const QString& username)
{
	QMutexLocker locker(&mutex_);
	QSqlQuery query(db_);
	query.prepare("SELECT password_hash FROM users WHERE username = ?");
	query.addBindValue(username);
	if (query.exec() && query.next())
	{
		return query.value(0).toString();
	}
	return QString();
}

bool DatabaseManager::createSession(const QString& token, const QString& username)
{
	QMutexLocker locker(&mutex_);
	QSqlQuery query(db_);
	query.prepare("INSERT OR REPLACE INTO sessions (token, username, last_activity) VALUES (?, ?, ?)");
	query.addBindValue(token);
	query.addBindValue(username);
	query.addBindValue(QDateTime::currentSecsSinceEpoch());
	return query.exec();
}

bool DatabaseManager::validateSession(const QString& token, QString& username)
{
	QMutexLocker locker(&mutex_);
	QSqlQuery query(db_);
	query.prepare("SELECT username FROM sessions WHERE token = ?");
	query.addBindValue(token);
	if (!query.exec() || !query.next())
	{
		return false;
	}
	username = query.value(0).toString();
	return true;
}

void DatabaseManager::updateSessionActivity(const QString& token)
{
	QMutexLocker locker(&mutex_);
	QSqlQuery query(db_);
	query.prepare("UPDATE sessions SET last_activity = ? WHERE token = ?");
	query.addBindValue(QDateTime::currentSecsSinceEpoch());
	query.addBindValue(token);
	query.exec();
}

void DatabaseManager::removeSession(const QString& token)
{
	QMutexLocker locker(&mutex_);
	QSqlQuery query(db_);
	query.prepare("DELETE FROM sessions WHERE token = ?");
	query.addBindValue(token);
	query.exec();
}

void DatabaseManager::cleanExpiredSessions(qint64 idleTimeoutSeconds)
{
	QMutexLocker locker(&mutex_);
	qint64 expireTime = QDateTime::currentSecsSinceEpoch() - idleTimeoutSeconds;
	QSqlQuery query(db_);
	query.prepare("DELETE FROM sessions WHERE last_activity < ?");
	query.addBindValue(expireTime);
	query.exec();
}






















