

#include "TCPConnect.h"
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

static WSADATA wsaData;
static SOCKET listenSocket = INVALID_SOCKET; 
static SOCKET clientSocket = INVALID_SOCKET;  
static bool isInitialized = false;


bool InitNetworking()
{
    if (!isInitialized)
    {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "WSAStartup failed" << std::endl;
            return false;
        }
        isInitialized = true;
    }
    return true;
}


void ShutdownNetworking()
{
    if (clientSocket != INVALID_SOCKET)
    {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
    }
    if (listenSocket != INVALID_SOCKET)
    {
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
    }
    if (isInitialized)
    {
        WSACleanup();
        isInitialized = false;
    }
}

// --- Серверная часть ---

bool StartServer(int port)
{
    if (!InitNetworking()) return false;


    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET)
    {
        std::cerr << "socket() failed, error: " << WSAGetLastError() << std::endl;
        return false;
    }


    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);


    if (bind(listenSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        std::cerr << "bind() failed, error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
        return false;
    }


    if (listen(listenSocket, 1) == SOCKET_ERROR)
    {
        std::cerr << "listen() failed, error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
        return false;
    }


    u_long mode = 1;
    ioctlsocket(listenSocket, FIONBIO, &mode);

    std::cout << "Server listening on port " << port << std::endl;
    return true;
}

// Проверка, есть ли входящее соединение. Если есть – принимает и возвращает true.
bool AcceptConnection()
{
    if (listenSocket == INVALID_SOCKET) return false;
    if (clientSocket != INVALID_SOCKET) return true; 

    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    SOCKET newClient = accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
    if (newClient != INVALID_SOCKET)
    {
    
        u_long mode = 1;
        ioctlsocket(newClient, FIONBIO, &mode);
        clientSocket = newClient;
        std::cout << "Client connected!" << std::endl;
        return true;
    }
    else if (WSAGetLastError() != WSAEWOULDBLOCK)
    {

        std::cerr << "accept() error: " << WSAGetLastError() << std::endl;
    }
    return false;
}

// --- Клиентская часть ---
// Подключение к серверу (IP, порт). Возвращает true, если соединение установлено или уже есть.
bool ConnectToServer(const char* ip, int port)
{
    if (!InitNetworking()) return false;

    if (clientSocket != INVALID_SOCKET) return true; 

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "client socket() failed" << std::endl;
        return false;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);


    u_long mode = 1;
    ioctlsocket(clientSocket, FIONBIO, &mode);

    int res = connect(clientSocket, (sockaddr*)&addr, sizeof(addr));
    if (res == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK)
        {
            std::cerr << "connect() failed, error: " << err << std::endl;
            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;
            return false;
        }
        std::cout << "Connecting in progress..." << std::endl;
        return false;
    }
    return true;
}

// Проверить, установлено ли соединение 
bool IsConnected()
{
    if (clientSocket == INVALID_SOCKET) return false;
    char buf[1];
    int ret = recv(clientSocket, buf, 0, MSG_PEEK);
    if (ret == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK || err == WSAECONNRESET)
            return false;
    }
    return true; 
}

// --- Отправка и приём данных ---
// Отправка данных (буфер, длина). Возвращает true, если отправлены все байты.
bool SendData(const char* data, int len)
{
    if (clientSocket == INVALID_SOCKET) return false;
    int sent = send(clientSocket, data, len, 0);
    if (sent == SOCKET_ERROR)
    {
        std::cerr << "send() error: " << WSAGetLastError() << std::endl;
        return false;
    }
    return sent == len;
}


int ReceiveData(char* buffer, int maxLen)
{
    if (clientSocket == INVALID_SOCKET) return -2;
    int received = recv(clientSocket, buffer, maxLen, 0);
    if (received > 0) return received;
    if (received == 0) return 0; 
    int err = WSAGetLastError();
    if (err == WSAEWOULDBLOCK) return -1; 
    return -2; 
}

// Закрыть текущее соединение 
void CloseConnection()
{
    if (clientSocket != INVALID_SOCKET)
    {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
    }
 
}

// Для сервера: закрыть слушающий сокет 
void StopListening()
{
    if (listenSocket != INVALID_SOCKET)
    {
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
    }
}