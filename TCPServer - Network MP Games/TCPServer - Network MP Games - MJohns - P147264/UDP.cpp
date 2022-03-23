/// Controls UDP Functions for sending and receiving

#include "UDP.h"

UDP::UDP()
{
	port = -1;
	addressFamily = -1;
	packetType = -1;
	protocolType = -1;
}

UDP::UDP(int portNumber, int addressFamily, int packetType, int protocolType)
{
	port = portNumber;
	this->addressFamily = addressFamily;
	this->packetType = packetType;
	this->protocolType = protocolType;
	InitUDP();
}

UDP::~UDP()
{

}

int UDP::InitUDP()
{
	// (MAKEWORD(Major version, Minor version), LPWSADATA = setup information from windows)
	// WSA function return zero if working correctly
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// check result of WSAStartup function
	if (iResult == 0)
	{
		cout << "UDP Sockets Starting up..." << endl;

		listenSocket = Socket("127.0.0.1", 27019, addressFamily, packetType, protocolType);

		clientSocket = Socket("127.0.0.1", 27018, addressFamily, packetType, protocolType);

		if (listenSocket.GetSocket() == INVALID_SOCKET)
		{
			PrintError("server socket invalid");
			return -1;
		}
		else
		{
			// set socket options, bind socket and freeaddressinfo
			listenSocket.InitialiseListenSocketBind();
		}
	}
	return 0;
}

int UDP::Send()
{
	const char* buf = "Hello UDP, From Server";
	cout << "Sending on UDP to Client = " << buf << endl;
	OutMemoryStream out;
	out = OutMemoryStream();
	out = mes.SendUDP(buf, 23);
	sendto(clientSocket.GetSocket(), out.GetBufferPtr(), out.GetLength(), 0, clientSocket.GetAddressList()->ai_addr, (int)clientSocket.GetAddressList()->ai_addrlen);
	return 0;
}

int UDP::Recv()
{
	char buf[23] = "";

	// receive from udp function
	recvfrom(listenSocket.GetSocket(), buf, 23, 0, NULL, NULL);
	InMemoryStream in;
	in = InMemoryStream(buf, 23);
	in.ResetCurrentPosition();
	cout << "Recieved on UDP Socket = " << in.GetBufferPtr() << endl;

	return 0;
}

// WSA Error printing function
void UDP::PrintError(string input)
{
	// Output Server Port number and last error encountered
	cout << input << endl
		<< "Server Port Number: " << port << endl
		<< "WSA Error Code: " << WSAGetLastError() << endl;

	//intialisationError = -1;
	listenSocket.CloseSocket();
	WSACleanup();
}

