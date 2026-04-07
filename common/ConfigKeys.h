#ifndef CONFIGKEYS_H
#define CONFIGKEYS_H

#include <string_view>

namespace ConfigKeys
{

	constexpr std::string_view ServerSection = "server";
	
	constexpr std::string_view DatabaseSection = "database";
	constexpr std::string_view DatabaseTypeKey = "type";
	constexpr std::string_view DatabaseFielKey = "file";
	constexpr std::string_view DatabaseTimeoutMsKey = "timeout_ms";
	constexpr std::string_view DatabaseWalModeKey = "wal_mode";
	constexpr const char* DefaultDatabaseType = "sqlite";
	constexpr const char* DefaultDatabaseFile = "chat_server.db"	// потом надо определиться какое в итоге будет название
	constexpr int DefaultDatabaseTimeoutMS = 5000;
	constexpr bool DefaultDatabaseWalMode = true;

}

#endif CONFIGKEYS_H
