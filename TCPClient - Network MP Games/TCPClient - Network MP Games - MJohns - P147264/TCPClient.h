#pragma once

// disable CRT Secure Warning Messages
#define _CRT_SECURE_NO_WARNINGS  1

#include <iostream>
#include <WinSock2.h> // winsock2 library
#include <string>
#include <time.h>

#include "Socket.h"
#include "User.h"
#include "OutMemoryStream.h"
#include "Message.h"
#include "UDP.h"

using std::cout; using std::endl; using std::cin;
using std::string;

const unsigned int bufLength = 512;

class TCPClient
{
public:
	TCPClient();
	TCPClient(const char*, int, int, int, int);
	~TCPClient();

	void InitClient();

	void CreateConnectSocket(int addressFamily, int packetType, int protocolType);

	void SetServerName(const char* serverName);
	string GetServerName();

	int CheckIntialisationError();

	int RecvUDP();

	void Loop();

	void PrintError(string input);

	DWORD myThreadID;
	HANDLE myHandle;

	void SendStream(OutMemoryStream outStream);
	UDP* udp;
private:
	int port;
	bool exit = false;

	WSADATA	wsaData;
	string	recvbuf;
	string	sendbuf;
	char sendbufchar[bufLength];
	int		iResult;
	int		recvbuflen;
	int		intialisationError;

	string serverName;
	
	Socket connectSocket;
	int addressFamily;
	int packetType;
	int protocolType;

	Message mes;
};

