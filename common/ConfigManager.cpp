// Реализация загрузки JSON и методов доступа

#include "ConfigManager.h"
#include "ConfigKeys.h"
#include <fstream>
#include <sstream>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>

ConfigManager& ConfigManager::instance()
{
	static ConfigManager instance;   // Локальная статическая переменная - потокобезопасная инициализация в C++11
	return instance;
}

bool ConfigManager::loadConfigFromFile(const std::filesystem::path& filePath)
{
	// Открываем файл
	std::ifstream fileStream(filePath);
	if (!fileStream.is_open())
	{
		qCritical() << "Cannot open config file:" << filePath.c_str();
		return false;
	}

	// Читаем содержимое в строку
	std::stringstream stringStream;
	stringStream << fileStream.rdbuf();
	std::string fileContent = stringStream.str();

	// Парсим JSON
	QJsonParseError parseError;
	QJsonDocument jsonDocument = QJsonDocument::fromJson( QByteArray::fromStdString(fileContent), &parseError );
	if (parseError.error != QJsonParseError::NoError)
	{
		qCritical() << "JSON parse error:" << parseError.errorString();
		return false;
	}
	if (!jsonDocument.isObject())
	{
		qCritical() << "Config root must be a JSON object";
		return false;
	}

	rootJsonObject_ = jsonDocument.object();
	return true;
}

// Специализированные геттеры (используют ConfigKeyss и значения по умолчанию)
quint16 ConfigManager::getPort() const
{
	QJsonObject serverObj = rootJsonObject_.value(QString::fromStdString(std::string(ConfigKeys::ServerSection))).toObject();
	int port = serverObj.value(QString::fromStdString(std::string(ConfigKeys::PortKey))).toInt(ConfigKeys::DefaultPort);
	return static_cast<quint16>(port);
}

int ConfigManager::getMaxClients() const
{
	QJsonObject serverObj = rootJsonObject_.value(QString::fromStdString(std::string(ConfigKeys::ServerSection))).toObject();
	return serverObj.value(QString::fromStdString(std::string(ConfigKeys::MaxClientsKey))).toInt(ConfigKeys::DefaultMaxClients);
}

std::chrono::seconds ConfigManager::getIdleTimeout() const
{
	QJsonObject serverObj = rootJsonObject_.value(QString::fromStdString(std::string(ConfigKeys::ServerSection))).toObject();
	int seconds = serverObj.value(QString::fromStdString(std::string(ConfigKeys::IdleTimeoutSecKey))).toInt(ConfigKeys::DefaultIdleTimeoutSeconds);
	return std::chrono::seconds(seconds);
}

// Универсальные геттеры
QString ConfigManager::getString(const std::string_view& section, const std::string_view& key, const QString& defaultValue) const
{
	QJsonObject sectionObj = rootJsonObject_.value(	QString::fromStdString(std::string(section))).toObject();
	return sectionObj.value( QString::fromStdString(std::string(key))).toString(defaultValue);
}

int ConfigManager::getInt(const std::string_view& section, const std::string_view& key, int defaultValue) const
{
	QJsonObject sectionObj = rootJsonObject_.value(QString::fromStdString(std::string(section))).toObject();
	return sectionObj.value(QString::fromStdString(std::string(key))).toInt(defaultValue);
}

bool ConfigManager::getBool(const std::string_view& section, const std::string_view& key, bool defaultValue) const
{
	QJsonObject sectionObj = rootJsonObject_.value(QString::fromStdString(std::string(section))).toObject();
	return sectionObj.value(QString::fromStdString(std::string(key))).toBool(defaultValue);
}

QJsonObject ConfigManager::getLoggingConfig() const
{
	return rootJsonObject_.value(QString::fromStdString(std::string(ConfigKeys::LoggingSection))).toObject();
}





