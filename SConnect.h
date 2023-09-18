/* SConnect.h Chat */

#pragma once
#undef UNICODE
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

class SConnect
{
private:
	#define DEFAULT_PORT "7777"
	#define DEFAULT_BUFLEN 1024
	
public:
	SConnect() = default;
	~SConnect() = default;
	WSADATA wsaData;
	int iResult;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL;
	struct addrinfo hints;
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	int sconnection();
};
