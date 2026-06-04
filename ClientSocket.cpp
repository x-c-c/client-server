#include "ClientSocket.h"
ClientSocket::ClientSocket(int fileDescriptor) noexcept : clientSocketFileDescriptor_(fileDescriptor) {}
ClientSocket::ClientSocket(ClientSocket&& other) noexcept : clientSocketFileDescriptor_(other.clientSocketFileDescriptor_)
{
	other.clientSocketFileDescriptor_ = -1;
}

ClientSocket& ClientSocket::operator=(ClientSocket&& other) noexcept
{
	if (this != &other)
	{
		stop();
		clientSocketFileDescriptor_ = other.clientSocketFileDescriptor_;
		other.clientSocketFileDescriptor_ = -1;
	}
	return *this;
}

ClientSocket::~ClientSocket()
{
	stop();
}

void ClientSocket::stop()
{
	if (clientSocketFileDescriptor_ >= 0)
	{
		close(clientSocketFileDescriptor_);
		clientSocketFileDescriptor_ = -1;
	}
	
}

