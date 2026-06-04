#pragma once
#include <unistd.h>


class ClientSocket
{
private:
	int clientSocketFileDescriptor_ = -1;
	
public:
	ClientSocket() = default;
	explicit ClientSocket(int fileDescriptor) noexcept;
	
	ClientSocket(const ClientSocket&) = delete;
	ClientSocket& operator=(const ClientSocket&) = delete;
	
	ClientSocket(ClientSocket&& other) noexcept;
	ClientSocket& operator=(ClientSocket&& other) noexcept;
	
	~ClientSocket();
	
	void stop();
	int getClientSocketFileDescriptor() const { return clientSocketFileDescriptor_; }
};


