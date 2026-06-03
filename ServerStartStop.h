#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <format>	//require C++20


class ServerStartStop
{
private:
	//int serverFileDescriptor = -1;
	//socket parameters
	int domain = AF_INET;
	int type = SOCK_STREAM;	//socket type
	int protocol = IPPROTO_TCP;
	in_addr_t addr = INADDR_ANY;
public:
	ServerStartStop() = default;
	ServerStartStop(int domain, int type, int protocol) : domain(domain), type(type), protocol(protocol) {};
	int serverFileDescriptor = -1;
	bool start(int port = 8080);
	void stop();
};

