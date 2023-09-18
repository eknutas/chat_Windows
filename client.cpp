/* client.cpp Chat */

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 1024  // Максимальный размер буфера для данных
#define DEFAULT_PORT "7777" // Будем использовать этот номер порта
#define DEFAULT_ADDRESS "127.0.0.1"

int main()
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    char message[DEFAULT_BUFLEN];
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(DEFAULT_ADDRESS, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ptr = result;
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Connect to server.
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        std::cout << "Connection with the server failed!" << '\n';
        WSACleanup();
        return 1;
    }

    // Взаимодействие с сервером
    while (true)
    {
        // Ждем сообщения от сервера
        recv(ConnectSocket, message, sizeof(message), 0);
        std::cout << message << '\n';
        if ((strncmp(message, "Goodbye!", 8)) == 0) {
            break;
        }
        memset(message, 0, sizeof(message));
        std::cin.getline(message, 1024);
        send(ConnectSocket, message, sizeof(message), 0);
    }
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
