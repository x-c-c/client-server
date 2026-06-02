#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include "serverstartstop.h"
int main()
{
	ServerStartStop object;
	object.start(8080);
	
	sockaddr_in clientAddress;
	socklen_t clientLen = sizeof(clientAddress);
	int clientFd = accept(object.serverFileDescriptor,  reinterpret_cast<sockaddr*>(&clientAddress) , &clientLen);
	char buffer[1024];
	//ssize_t bytesRead = read(clientFd, buffer, sizeof(buffer) - 1);
	read(clientFd, buffer, sizeof(buffer) - 1);
	std::cout << std::string(buffer) << std::endl;
	
	close(clientFd);
	object.stop();
	return 0;
}
