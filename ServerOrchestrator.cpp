#include "ServerOrchestrator.h"

ServerOrchestrator::ServerOrchestrator(const ServerConfig &config) : config_(config){};

void ServerOrchestrator::run()
{
	lifecycle_.start(config_);
}

void ServerOrchestrator::stop()
{
	lifecycle_.stop();
}
