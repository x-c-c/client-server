#include "ServerStartStop.h"

ServerStartStop::ServerStartStop(ServerStartStop&& other) noexcept : serverSocketFileDescriptor_(other.serverSocketFileDescriptor_)
{
	other.serverSocketFileDescriptor_ = -1;
}

ServerStartStop& ServerStartStop::operator=(ServerStartStop&& other) noexcept
{
	if (this != &other)
	{
		stop();
		serverSocketFileDescriptor_ = other.serverSocketFileDescriptor_;
		other.serverSocketFileDescriptor_ = -1;
	}
	return *this;
}

ServerStartStop::~ServerStartStop()
{
	stop();
}

void ServerStartStop::start(const ServerConfig &config)
{
	if (serverSocketFileDescriptor_ >= 0)
	{
		stop();
	}
	
	serverSocketFileDescriptor_ = socket(config.getDomain(), config.getType(), config.getProtocol());
	if (serverSocketFileDescriptor_ < 0)
	{
		throw std::runtime_error(std::format("socket(domain = {}, type = {}, protocol = {} )",
													 config.getDomain(),
													 config.getType(),
													 config.getProtocol() ));
	}
	//========================change later========================================
	int opt = 1;
	if (setsockopt(serverSocketFileDescriptor_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		throw std::runtime_error("setsockopt(...)");	// many troubles with lib <format>
	}
	//============================================================================
	sockaddr_in serverAddr{};
	serverAddr.sin_family = config.getDomain();
	serverAddr.sin_addr.s_addr = config.getAddr();
	serverAddr.sin_port = htons(config.getPort());
	
	sockaddr* addrPtr = reinterpret_cast<sockaddr*>(&serverAddr);
	socklen_t addrLen = sizeof(serverAddr);
	int bindResult = bind(serverSocketFileDescriptor_, addrPtr, addrLen);
	if (bindResult < 0)
	{
		throw std::runtime_error(std::format("bind(sockfd = {}, *addr = {}, addrLen = {})",
												   serverSocketFileDescriptor_,
												   static_cast<void*>(addrPtr),
												   addrLen));
	}
	
	int listenResult = listen(serverSocketFileDescriptor_, SOMAXCONN);	// SOMAXCONN - system's constant
	if (listenResult < 0)
	{
		throw std::runtime_error(std::format("listen(sockfd = {}, backlog = {})",
													 serverSocketFileDescriptor_,
													 SOMAXCONN));
	}		

/*
	catch(const std::exception &error)
	{
		std::cerr << "[ ERROR ]" <<"[ " << __PRETTY_FUNCTION__ << " ] in " << error.what() << std::endl;
		if (serverSocketFileDescriptor_ >= 0)
		{
			stop();
		}
	}
*/
}

void ServerStartStop::stop()
{
	if (serverSocketFileDescriptor_ >= 0)
	{
		close(serverSocketFileDescriptor_);
		serverSocketFileDescriptor_ = -1;
	}
	
}
