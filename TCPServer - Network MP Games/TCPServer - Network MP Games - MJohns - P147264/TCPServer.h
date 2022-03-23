#pragma once

// disable CRT Secure Warning Messages
#define _CRT_SECURE_NO_WARNINGS  1

#include <iostream>
#include <WinSock2.h> // winsock2 library
#include <string>
#include "Socket.h"
#include "User.h"
#include "InMemoryStream.h"
#include "Message.h"
#include "UDP.h"

using std::cout; using std::endl; using std::string;

const unsigned int bufLength = 1470;

class TCPServer
{
public:
	TCPServer();
	TCPServer(int);
	TCPServer(int, int, int, int);
	~TCPServer();

	int InitServer();

	void CreateClientSocket();

	int GetIntialisationError();

	int RecvUDP();

	int Listen();

	int ListenLoop();
	UDP* udp;
private:

	int clientWin = 0;
	int port;
	bool exit = false;

	int portNumber;
	int addressFamily;
	int packetType;
	int protocolType;

	WSADATA	wsaData;
	string	recvbuf;
	char recvbufchar[32];
	int		iResult;
	int		recvbuflen;
	int		iRcvdBytes;
	int		intialisationError;
	char tempReceiveBuffer[bufLength];

	User* one;

	// Sockets
	Socket listenSocket;
	Socket clientSocket;

	Message mes;

	DWORD myThreadID;
	HANDLE myHandle;
	CRITICAL_SECTION g_cs;

	void PrintError(string);
	void ReceiveStream(Socket socket);
};