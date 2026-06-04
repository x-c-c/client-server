#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <format>	//require C++20
#include "ServerConfig.h"



class ServerStartStop
{
private:
	int serverSocketFileDescriptor_ = -1;
	
public:
	ServerStartStop() = default;
	
	ServerStartStop(const ServerStartStop&) = delete;
	ServerStartStop& operator=(const ServerStartStop&) = delete;
	
	ServerStartStop(ServerStartStop&& other) noexcept;
	ServerStartStop& operator=(ServerStartStop&& other) noexcept;
	
	~ServerStartStop();
	
	void start(const ServerConfig& config);
	void stop();
	
	int getServerSocketFileDescriptor() const { return serverSocketFileDescriptor_; }
};

