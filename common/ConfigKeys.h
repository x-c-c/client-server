// Содержит константы для чтения конфигурационного файла
// Это позволяет избежать опечаток в строках по всему коду
#ifndef CONFIGKEYS_H
#define CONFIGKEYS_H

#include <string_view>

namespace ConfigKeys
{
	// Секции конфигурационного файла
	constexpr std::string_view ServerSection	= "server";
	constexpr std::string_view DatabaseSection	= "database";
	constexpr std::string_view LoggingSection	= "logging";
	
	// Ключи внутри секции "server"
	constexpr std::string_view PortKey		= "port";
	constexpr std::string_view MaxClientsKey	= "max_clients";
	constexpr std::string_view IdleTimeoutKey	= "idle_timeout_sec";
	
	// Ключи внутри секции "database"
	constexpr std::string_view DatabaseFielKey	= "file";
	constexpr std::string_view DatabaseTimeoutMsKey	= "timeout_ms";
	constexpr std::string_view DatabaseWalModeKey	= "wal_mode";
	
	// Ключи внутри секции "logging"
	constexpr std::string_view LogLevelKey		= "level";
	constexpr std::string_view LogFileKey		= "file";
	constexpr std::string_view LogMaxSizeMbKey	= "max_size_mb";
	constexpr std::string_view LogMaxFilesKey	= "max_files";
	
	// Значения по умолчанию
	constexpr const char* DefaultDatabaseFile 	= "chat_server.db";
	constexpr quint16 DefaultPort			= 12345;
	constexpr int DefaultMaxClients			= 1000;
	constexpr int DefaultIdleTimeoutSeconds		= 600;
	constexpr int DefaultDatabaseTimeoutMS		= 5000;
	constexpr bool DefaultDatabaseWalMode		= true;
	constexpr const char* DefaultLogLevel		= "info";
	constexpr const char* DefaultLogFile		= "logs/chat.log";
	constexpr int DefaultLogMaxSizeMb		= 10;
	constexpr int DefaultLogMaxFiles		= 10;
}

#endif
