#include "ServerConfig.h"
#include "ServerOrchestrator.h"

int main()
{
	ServerConfig config;
	ServerOrchestrator server(config);
	server.run();

	
	return 0;
}
