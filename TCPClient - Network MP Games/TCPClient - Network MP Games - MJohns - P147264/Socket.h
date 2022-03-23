#pragma once
#define _CRT_SECURE_NO_WARNINGS  1
#include <iostream>
#include <string>
#include <WinSock2.h> // winsock2 library
#include <ws2tcpip.h> // Used for address translation



using std::cout; using std::endl; using std::string;

class Socket
{
public:
	Socket();
	Socket(string, int, int, int, int);
	Socket(int, int, int, int);
	Socket(SOCKET);
	~Socket();

	// Socket Initialisation Function
	int InitialiseSocket();
	void InitialiseListenSocketBind();
	int ResolveAddress();
	int ResolveAddress(const char * serverName);
	int ResolveAddress(string serverName);

	void SetSocket(SOCKET);
	void SetSocketInvalid();
	void SetSocketOptions();
	void BindSocket();
	void Connect();
	SOCKET GetSocket();

	addrinfo* GetAddressList();
	addrinfo* GetPtr();
	void SetPtrToAddressList();
	void SetPtr(addrinfo * inPtr);
	addrinfo* GetHints();
	void FreeAddressInfo();
	void ShutdownSocket();
	void CloseSocket();
private:
	int port = -1;

	string serverName;
	int addressFamily;
	int packetType;
	int protocolType;
	struct addrinfo* addressList;
	struct addrinfo* ptr;
	struct addrinfo hints;

	SOCKET currentSocket;

	void PrintError(string input);
};