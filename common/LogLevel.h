// Перечисление уровней логирования и вспомогательная функция преобразования в строку

#ifndef LOGLEVEL_H
#define LOGLEVEL_H
#include <string_view>
enum class LogLevel
{
	Trace,
	Debug,
	Info,
	Warn,
	Error
};

inline std::string_view logLevelToString(LogLevel level)
{
	switch (level)
	{
		case LogLevel::Trace:	return "trace";
		case LogLevel::Debug:	return "debug";
		case LogLevel::Info:	return "info";
		case LogLevel::Warn:	return "warn";
		case LogLevel::Error:	return "error";
		default:		return "info";
	}
}

#endif
