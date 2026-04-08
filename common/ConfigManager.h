// Синглтон Майерса для загрузки и доступа к настройкам из JSON-файла
// Предоставляет удобные методы для получения значений разных типов

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <filesystem>
#include <QHostAddress>
#include <QJsonObject>
#include <chrono>

class ConfigManager
{
public:
	// Получить единственный экземпляр
	static ConfigManager& instance();

	// Загрузить конфиг из файла (по умолчанию config/defaultConfig.json)
	bool loadConfigFromFile(const std::filesystem::path& filePath = "config/defaultConfig.json");

	// Специализированные геттеры для часто используемых параметров
	quint16 getPort() const;
	int getMaxClients() const;
	std::chrono::seconds getIdleTimeout() const;

	// Универсальные геттеры по секции и ключу
	QString getString(const std::string_view& section, const std::string_view& key, const QString& defaultValue) const;
	int getInt(const std::string_view& section, const std::string_view& key, int defaultValue) const;
	bool getBool(const std::string_view& section, const std::string_view& key, bool defaultValue) const;

	// Получить весь объект секции логирования (используется Logger'ом)
	QJsonObject getLoggingConfig() const;

private:
	ConfigManager() = default;	// Приватный конструктор
	QJsonObject rootJsonObject_;	// Корневой JSON-объект конфига
};

#endif

