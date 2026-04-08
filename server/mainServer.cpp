#include <QCoreApplication>
#include "common/ConfigManager.h"
#include "common/Logger.h"
int main(int argc, char* argv[])
{
	QCoreApplication  application(argc, argv);
	
	// Загружаем конфигурацию
	if(!ConfigManager::instance().loadConfigFromFile("config/defaultConfig.json"))
	{
		qCritical() << "Failed to load config file";
		return 1;
	}
	
	// Инициализируем логгер
	Logger::init("config/defaultConfig.json");
	LOG_INFO("Server starting");
	
	
	
	return application.exec();
}
