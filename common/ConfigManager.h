// Синглтон Майерса для загрузки и доступа к настройкам из JSON-файла
// Предоставляет удобные методы для получения значений разных типов
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H


#include <filesystem>
#include <chrono>
#include <QJsonObject>

class ConfigManager
{
public:
	// Создать единственный экземпляр
	static ConfigManager& instance();
	
	// Загрузить конфиг из файла (по умолчанию config/defaultConfig.json)
	bool loadConfigFromFile(const std::filesystem::path& filePath="config/defaultConfig.json");
	
	// Специализированные геттеры для часто используемых параметров
	//quint16 getPort() const;
	//int getMaxClients() const;
	//std::chrono::seconds getIdleTimeout() const;
private:
	ConfigManager() = default;	// Приватный конструктор
	QJsonObject rootJsonObject_;	// Корневой JSON-объект конфига
};

#endif

