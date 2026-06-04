#include "ServerAcceptor.h"
ServerAcceptor::ServerAcceptor(int listeningSocketFileDescriptor) : listeningSocketFileDescriptor_(listeningSocketFileDescriptor) {}
ServerAcceptor::ServerAcceptor(ServerAcceptor&& other) noexcept : listeningSocketFileDescriptor_(other.listeningSocketFileDescriptor_)
{
	other.listeningSocketFileDescriptor_ = -1;
}

ServerAcceptor& ServerAcceptor::operator=(ServerAcceptor&& other) noexcept
{
	if (this != &other)
	{
		listeningSocketFileDescriptor_ = other.listeningSocketFileDescriptor_;
		other.listeningSocketFileDescriptor_ = -1;
	}
	return *this;
}

ClientSocket ServerAcceptor::acceptConnection()
{
	if (listeningSocketFileDescriptor_ < 0)
	{
		throw std::runtime_error(std::format("acceptConnection() called but listening socket is invalid (listeningSocketFileDescriptor = {})",
											  listeningSocketFileDescriptor_));
	}
	sockaddr_in clientAddress{};
	sockaddr* addrPtr = reinterpret_cast<sockaddr*>(&clientAddress);
	socklen_t addrLen = sizeof(clientAddress);
	int clientSocketFileDescriptor = accept(listeningSocketFileDescriptor_, addrPtr, &addrLen);
	if (clientSocketFileDescriptor < 0)
	{
		throw std::runtime_error(std::format("accept(sockfd = {}, *addr = {}, &addrLen = {})",
												   listeningSocketFileDescriptor_,
												   static_cast<void*>(addrPtr),
												   addrLen));
	}
	return ClientSocket(clientSocketFileDescriptor);
}
