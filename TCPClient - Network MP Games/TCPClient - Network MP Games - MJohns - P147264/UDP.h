#pragma once

// disable CRT Secure Warning Messages
#define _CRT_SECURE_NO_WARNINGS  1

#include <iostream>
#include <WinSock2.h> // winsock2 library
#include <string>
#include "Socket.h"
#include "InMemoryStream.h"
#include "OutMemoryStream.h"
#include "Message.h"

class UDP
{
public:
	UDP();
	UDP(int portNumber, int addressFamily, int packetType, int protocolType);
	~UDP();
	const int bufLength = 1470;
	int InitUDP();
	int Send();
	char buffer[1470];

	int Recv();
	int		iResult;

	WSADATA	wsaData;

	void PrintError(string input);
	int i = 0;
	int port;
	int portNumber;
	int addressFamily;
	int packetType;
	int protocolType;

	Socket listenSocket;
	Socket clientSocket;

	Message mes;
};

