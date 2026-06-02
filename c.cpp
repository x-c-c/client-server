#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>
int main()
{
	
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	
	sockaddr_in serverAddress{};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(8080);
	connect(sock, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));	
	
	inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
	
	std::string message;
	getline(std::cin, message);
	send(sock, message.c_str(), message.length(),0);
	close(sock);
	return 0;
}

