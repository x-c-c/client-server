// Настройка spdlog: консольный и файловый вывод с ротацией

#include "Logger.h"
#include "ConfigManager.h"
#include "ConfigKeys.h"
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sink.h>

std::shared_ptr<spdlog::logger> Logger::globalLogger_ = nullptr;

void Logger::init(const std::filesystem::path& configPath)
{
	// Загружаем конфиг
	auto& config = ConfigManager::instance();
	config.loadConfigFromFile(configPath);
	
	// Читаем параметры логирования
	QJsonObject loggingConfig = config.getLoggingConfig();
	std::string logFile = loggingConfig
		.value(QString::fromStdString(std::string(ConfigKey::LogFileKey)))
		.toString(ConfigKey::DefaultLogFile).toStdString();
	
	int maxSizeMb = loggingConfig
		.value(QString::fromStdString(std::string(ConfigKey::LogMaxSizeMbKey)))
		.toInt(ConfigKey::DefaultLogMaxSizeMb);
		
	int maxFiles = loggingConfig
		.value(QString::fromStdString(std::string(ConfigKey::LogMaxFilesKey)))
		.toInt(ConfigKey::DefaultLogMaxFiles);
		
	std::string levelStr = loggingConfig
		.value(QString::fromStdString(std::string(ConfigKey::LogLevelKey)))
		.toString(ConfigKey::DefaultLogLevel)
		.toStdString();
	
	// Создаём приёмники (sinks): цветная консоль и файл с ротацией
	auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logFile, maxSizeMb*1024*1024, maxFiles);
	
	std::vector<spdlog::sink_ptr> sinks = {consoleSink, fileSink};
	globalLogger_ = std::make_shared<spdlog::logger>("chat_logger", sinks.begin(), sinks.end());
	
	// Устанавливаем уровень логирования
	if (levelStr == "trace")	globalLogger_->set_level(spdlog::level::trace);
	else if (levelStr == "debug")	globalLogger_->set_level(spdlog::level::debug);
	else if (levelStr == "info")	globalLogger_->set_level(spdlog::level::info);
	else if (levelStr == "warn")	globalLogger_->set_level(spdlog::level::warn);
	else if (levelStr == "error")	globalLogger_->set_level(spdlog::level::err);
	else				globalLogger_->set_level(spdlog::level::info);

	// Автоматический сброс буфера при сообщениях уровня info и выше
	globalLogger_->flush_on(spdlog::level::info);
	// Делаем наш логгер глобальным для spdlog (опционально)
	sdplog::set_default_logger(globalLogger_);
}

std::shared_ptr<spdlog::logger> Logger::get()
{
	// Если логгер ещё не инициализирован (например, в тестах), создаём консольный логгер
	if (!globalLogger_)
	{
		globalLogger_ = spdlog::stdout_color_mt("default");
		globalLogger_->set_level(spdlog::level::info);
	}
	
	return globalLogger_;
}

void Logger::setLevel(LogLevel level)
{
	if (!globalLogger_) return;
	switch (level) {
		case LogLevel::Trace:	globalLogger_->set_level(spdlog::level::trace);
			break;
		case LogLevel::Debug:	globalLogger_->set_level(spdlog::level::debug);
			break;
		case LogLevel::Info:	globalLogger_->set_level(spdlog::level::info);
			break;
		case LogLevel::Warn:	globalLogger_->set_level(spdlog::level::warn);
			break;
		case LogLevel::Error:	globalLogger_->set_level(spdlog::level::err);
			break;
	}
}






