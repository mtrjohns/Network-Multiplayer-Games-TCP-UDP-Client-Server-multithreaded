#pragma once
#include <iostream>
#include <string>
#include <WinSock2.h> // winsock2 library
#include <ws2tcpip.h> // Used for address translation

using std::cout; using std::endl; using std::string;

class Socket
{
public:
	Socket();
	Socket(int, int, int, int);
	Socket(SOCKET);
	~Socket();

	// Socket Initialisation Function
	void InitialiseSocket(int, int, int);
	void InitialiseServerSocketBind();
	void ResolveAddress();

	void SetSocket(SOCKET);
	void SetSocketOptions();
	void BindSocket();
	SOCKET GetSocket();

	addrinfo* GetAddressList();
	addrinfo* GetHints();
	void FreeAddressInfo();
	void ShutdownSocket();
	void CloseSocket();

private:
	int port = -1;
	struct addrinfo* addressList;
	struct addrinfo* ptr;
	struct addrinfo hints;

	SOCKET currentSocket;

	void PrintError(string input);
};