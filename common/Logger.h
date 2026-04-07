// Обёртка над spdlog для централизованной настройки и удобных макросов

#ifndef LOGGER_H
#define LOGGER_H

#include "LogLevel.h"
#include <filesystem>
#include <spdlog/spdlog.h>
#include <memory>

class Logger
{
public:
	// Инициализировать глобальный логгер, прочитав настройки из конфига
	static void init(const std::filesystem::path& configPath);
	
	// Получить текущий экземпляр логгера (создаёт консольный логгер по умолчанию, если не инициализирован)
	static std::shared_ptr<spdlog::logger> get();
	
	// Изменить уровень логирования во время выполнения
	static void setLevel(LogLevel level);
private:
	static std::shared_ptr<spdlog::logger> globalLogger_;
};

// Макросы для удобного логирования с автоматическим указанием файла/строки
#define LOG_TRACE(...)	::Logger::get()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)	::Logger::get()->trace(__VA_ARGS__)
#define LOG_INFO(...)	::Logger::get()->trace(__VA_ARGS__)
#define LOG_WARN(...)	::Logger::get()->trace(__VA_ARGS__)
#define LOG_ERROR(...)	::Logger::get()->trace(__VA_ARGS__)


#endif
