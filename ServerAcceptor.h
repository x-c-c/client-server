#pragma once
#include <unistd.h>
#include "ClientSocket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <format>	//require C++20


class ServerAcceptor
{
private:
	int listeningSocketFileDescriptor_ = -1;
	
public:
	ServerAcceptor() = default;
	explicit ServerAcceptor(int listeningSocketFileDescriptor);
	
	ServerAcceptor(const ServerAcceptor&) = delete;
	ServerAcceptor& operator=(const ServerAcceptor&) = delete;
	
	ServerAcceptor(ServerAcceptor&& other) noexcept;
	ServerAcceptor& operator=(ServerAcceptor&& other) noexcept;
	
	~ServerAcceptor() = default;
	
	ClientSocket acceptConnection();
	int getListeningSocketFileDescriptor() const { return listeningSocketFileDescriptor_; }
};



