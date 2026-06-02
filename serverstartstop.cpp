#include "serverstartstop.h"
#define LOG_CLASS_ERROR(msg)
bool ServerStartStop::start(int port)
{
	try
	{
		serverFileDescriptor = socket(domain, type, protocol);
		if (serverFileDescriptor < 0)
		{
			throw std::runtime_error(std::format("socket(domain = {}, type = {}, protocol = {})",
														 domain,
														 type,
														 protocol));
		}
		//========================change later========================================
		int opt = 1;
		setsockopt(serverFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		//============================================================================
		//this->port = port;
		sockaddr_in serverAddr{};
		serverAddr.sin_family = domain;
		serverAddr.sin_addr.s_addr = addr;
		serverAddr.sin_port = htons(port);
		
		sockaddr* addrPtr = reinterpret_cast<sockaddr*>(&serverAddr);
		socklen_t addrLen = sizeof(serverAddr);
		int bindResult = bind(serverFileDescriptor, addrPtr, addrLen);
		if (bindResult < 0)
		{
			throw std::runtime_error(std::format("bind(sockfd = {}, *addr = {}, addrLen = {})",
													   serverFileDescriptor,
													   static_cast<void*>(addrPtr),
													   addrLen));
		}
		
		int listenResult = listen(serverFileDescriptor, SOMAXCONN);	// SOMAXCONN - system's constant
		if (listenResult < 0)
		{
			throw std::runtime_error(std::format("listen(sockfd = {}, backlog = {})",
														 serverFileDescriptor,
														 SOMAXCONN));
		}		
		return true;
	}
	catch(const std::exception &error)
	{
		std::cerr << "[ ERROR ]" <<"[ " << __PRETTY_FUNCTION__ << " ] in " << error.what() << std::endl;
		if (serverFileDescriptor >= 0)
		{
			stop();
		}
		return false;
	}
}

void ServerStartStop::stop()
{
	close(serverFileDescriptor);
	serverFileDescriptor = -1;
}
