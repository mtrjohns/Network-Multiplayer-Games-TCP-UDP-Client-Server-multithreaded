#include "Socket.h"

Socket::Socket()
{
	// set to Invalid socket, enables error checking against socket
	SetSocketInvalid();
	addressList = nullptr;
	serverName= "";
	addressFamily = -1;
	packetType = -1;
	protocolType = -1;
}

Socket::~Socket()
{

}

Socket::Socket(string serverName, int portNumber, int addressFamily, int packetType, int protocolType)
{
	// set to Invalid socket, enables error checking against socket
	SetSocketInvalid();
	addressList = nullptr;
	port = portNumber;
	ptr = nullptr;
	this->serverName = serverName;
	this->addressFamily = addressFamily;
	this->packetType = packetType;
	this->protocolType = protocolType;
	InitialiseSocket();
}

Socket::Socket(int portNumber, int addressFamily, int packetType, int protocolType)
{
	// set to Invalid socket, enables error checking against socket
	SetSocketInvalid();
	addressList = nullptr;
	port = portNumber;
	ptr = nullptr;
	this->addressFamily = addressFamily;
	this->packetType = packetType;
	this->protocolType = protocolType;
	InitialiseSocket();
}

Socket::Socket(SOCKET input)
{
	SetSocket(input);
}

// Initialise using a string type server name - client use
int Socket::InitialiseSocket()
{
	cout << "Initialising Socket" << endl;

	ZeroMemory(&hints, sizeof(hints));
	// Address Family, AF-UNSPEC, AF_INET = IPv4, AF_INET6 = IPv6
	hints.ai_family = addressFamily;

	// Packet Type, SOCK_Stream = TCP, SOCK_DGRAM = UDP,
	// SOCK_RAW = application layer crafted packet,
	// SOCK_SEQPACKET = packets might be required to be read in full upon receipt
	hints.ai_socktype = packetType;

	// Protocol, Transport or Network layer protocols
	// IPPROTO_UDP - requires SOCK_DGRAM
	// IPPROTO_TCP - requires SOCK_STREAM
	// IPPROTO_IP / 0 - uses default protocol for type set
	hints.ai_protocol = protocolType;

	// AI_PASSIVE flag - address structure intended to be binded
	hints.ai_flags = AI_PASSIVE;

	if (serverName != "")
	{
		ResolveAddress(serverName);
	}
	else
	{
		ResolveAddress();
	}

	currentSocket = socket(addressList->ai_family, addressList->ai_socktype, addressList->ai_protocol);
	if (currentSocket == INVALID_SOCKET)
	{
		PrintError("Socket creation failed");
		SetSocketInvalid();
		closesocket(currentSocket);
		return -1;
	}
	else
	{
		return 0;
	}
}

void Socket::InitialiseListenSocketBind()
{
	SetSocketOptions();

	BindSocket();

	FreeAddressInfo();
}

// Resolve address - use this for listen sockets
int Socket::ResolveAddress()
{
	if (getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &addressList) != 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

// Resolve Address  - use this for client sockets
int Socket::ResolveAddress(const char* serverName)
{
	if (getaddrinfo(serverName, std::to_string(port).c_str(), &hints, &addressList) != 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

// Resolve Address  - use this for client sockets
int Socket::ResolveAddress(string serverName)
{
	if (getaddrinfo(serverName.c_str(), std::to_string(port).c_str(), &hints, &addressList) != 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

void Socket::SetSocket(SOCKET input)
{
	this->currentSocket = input;
}

void Socket::SetSocketInvalid()
{
	// set to Invalid socket, enables error checking against socket
	currentSocket = INVALID_SOCKET;
}

void Socket::SetSocketOptions()
{
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);

	if (setsockopt(currentSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&bOptVal, bOptLen) == SOCKET_ERROR)
	{
		PrintError("Socket creation failed");
		CloseSocket();
		WSACleanup();
	}
}

void Socket::BindSocket()
{
	// Check binding of socket was completed correctly
	if (bind(currentSocket, addressList->ai_addr, (int)addressList->ai_addrlen) == SOCKET_ERROR)
	{
		PrintError("Socket Binding Failed");
	}
}

void Socket::Connect()
{
	if (connect(currentSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
	{
		PrintError("Unable to Connect to Socket!");
		//cout << "Unable to Connect to Server" << endl;
	}
}

SOCKET Socket::GetSocket()
{
	return currentSocket;
}

addrinfo* Socket::GetAddressList()
{
	return addressList;
}

addrinfo* Socket::GetPtr()
{
	return ptr;
}

void Socket::SetPtrToAddressList()
{
	ptr = addressList;
}

void Socket::SetPtr(addrinfo* inPtr)
{
	ptr = inPtr;
}

addrinfo* Socket::GetHints()
{
	return &hints;
}

void Socket::FreeAddressInfo()
{
	if (addressList != nullptr)
	{
		// clear addressList memory
		freeaddrinfo(addressList);
	}
}

// Shutdown and wait for ACK from client
void Socket::ShutdownSocket()
{
	// Shutdown connection, SD_SEND sets no more sending allowed
	if (shutdown(currentSocket, SD_SEND) == SOCKET_ERROR)
	{
		PrintError("Shutdown Failed");
	}
}

void Socket::CloseSocket()
{
	closesocket(currentSocket);
}

void Socket::PrintError(string input)
{
	cout << input << endl
		 << "WSA Error Code: " << WSAGetLastError() << endl;

	CloseSocket();
	SetSocketInvalid();
	WSACleanup();
}