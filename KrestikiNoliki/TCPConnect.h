#pragma once


#ifndef _WINSOCK2API_
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

bool InitNetworking();

void ShutdownNetworking();


bool StartServer(int port);


bool AcceptConnection();


void StopListening();


bool ConnectToServer(const char* ip, int port);


bool IsConnected();


bool SendData(const char* data, int len);

int ReceiveData(char* buffer, int maxLen);

void CloseConnection();
