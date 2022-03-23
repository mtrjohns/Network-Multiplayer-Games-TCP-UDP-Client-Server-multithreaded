#include "Socket.h"

Socket::Socket()
{
	// set to Invalid socket, enables error checking against socket
	currentSocket = INVALID_SOCKET;
	addressList = nullptr;
}

Socket::~Socket()
{

}

Socket::Socket(int portNumber, int addressFamily, int packetType, int protocolType)
{
	// set to Invalid socket, enables error checking against socket
	currentSocket = INVALID_SOCKET;
	addressList = nullptr;
	port = portNumber;
	//ptr = nullptr;
	InitialiseSocket(addressFamily, packetType, protocolType);
}

Socket::Socket(SOCKET input)
{
	SetSocket(input);
}

void Socket::InitialiseSocket(int addressFamily, int packetType, int protocolType)
{
	cout << "Initialising Sockets" << endl;

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

	ResolveAddress();

	currentSocket = socket(addressList->ai_family, addressList->ai_socktype, addressList->ai_protocol);
	if (currentSocket == INVALID_SOCKET)
	{
		PrintError("Socket creation failed");
	}
}

void Socket::InitialiseServerSocketBind()
{
	SetSocketOptions();

	BindSocket();

//	FreeAddressInfo();
}

void Socket::ResolveAddress()
{
	getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &addressList);
}

void Socket::SetSocket(SOCKET input)
{
	this->currentSocket = input;
}

void Socket::SetSocketOptions()
{
	// ASK MIKE WTF THIS DOES
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

SOCKET Socket::GetSocket()
{
	return currentSocket;
}

addrinfo* Socket::GetAddressList()
{
	return addressList;
}

addrinfo* Socket::GetHints()
{
	return &hints;
}

void Socket::FreeAddressInfo()
{
	// clear addressList memory
	freeaddrinfo(addressList);
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
	WSACleanup();
}