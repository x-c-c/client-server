#pragma once
#include <iostream>
#include "ServerConfig.h"
#include "ServerStartStop.h"
#include "ServerAcceptor.h"
class ServerOrchestrator
{
private:
	ServerConfig config_;
	ServerStartStop lifecycle_;
public:
	ServerOrchestrator(const ServerConfig &config);
	~ServerOrchestrator() = default;
	void run();
	void stop();
	
};
