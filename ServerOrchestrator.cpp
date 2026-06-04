#include "ServerOrchestrator.h"

ServerOrchestrator::ServerOrchestrator(const ServerConfig &config) : config_(config){};

void ServerOrchestrator::run()
{
	lifecycle_.start(config_);
	std::cout << "Server started on port: " << config_.getPort() << std::endl;
	
	ServerAcceptor acceptor_(lifecycle_.getServerSocketFileDescriptor());
	ClientSocket clientSocketFileDescriptor = acceptor_.acceptConnection();
	std::cout << "Client connected, clientSocketFileDescriptor: " << clientSocketFileDescriptor.getClientSocketFileDescriptor() << std::endl;
	
	char buffer[1024];
    ssize_t bytesRead = read(clientSocketFileDescriptor.getClientSocketFileDescriptor(), buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }
}

void ServerOrchestrator::stop()
{
	lifecycle_.stop();
}
