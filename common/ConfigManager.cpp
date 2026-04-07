#include "ConfigManager.h"
#include "ConfigKeys"
#include <fstream>
#include <sstream>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>

ConfigManager& ConfigManager::instance()
{
	static ConfigManager instance;
	return instance;
}

bool ConfigManager::loadConfigFromFile(const std::filesystem::path& filePath)
{
	std::ifstream fileStream(filePath);
	if (!fileStream.is_open())
	{
		qCritical() << "Cannot open config file: " << filePath.c_str();
		return false;
	}
	
	std::stringstream stringStream;
	stringStream << fileStream.rdbuf();
	std::string fileContent = stringStream.str();
	
	QJsonParseError parseError;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(QByteArray::fromStdString(fileContent), &parseError);
	if (parseError != QJsonParseError::NoError)
	{
		qCritical() << "JSON parse error: " << parseError.errorString();
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
















