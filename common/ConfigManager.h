#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QJsonObject>
#include <filesystem>
#include <chrono>

class ConfigManager
{
public:
	static ConfigManager& instance();
	bool loadConfigFromFile(const std::filesystem::path& filePath="config/defaultConfig.json");
	quint16 getPort() const;
	int getMaxClients() const;
	std::chrono::seconds getIdleTimeout() const;
private:
	ConfigManager() = default;
	QJsonObject rootJsonObject_;
};

#endif

