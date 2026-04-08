#include <QApplication>
#include "common/ConfigManager.h"
#include "ServerModel.h"
#include "ServerView.h"
#include "ServerController.h"
#include "common/Logger.h"
int main(int argc, char* argv[])
{
	QApplication  application(argc, argv);
	
	// Загружаем конфигурацию
	if(!ConfigManager::instance().loadConfigFromFile("config/defaultConfig.json"))
	{
		qCritical() << "Failed to load config file";
		return 1;
	}
	
	// Инициализируем логгер
	Logger::init("config/defaultConfig.json");
	LOG_INFO("Server starting");
	
	// Создаём модель, контроллер и представление
	ServerModel model;
	ServerView view;
	ServerController controller(&model);
	controller.setView(&view);
	view.show();
	return application.exec();
}
