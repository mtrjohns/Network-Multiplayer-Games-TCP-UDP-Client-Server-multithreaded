////////////////////////////////////////////////////////////
////		Program Main Starting Point					////
////////////////////////////////////////////////////////////
#include "TCPServer.h"

// Thread to receive on UDP
DWORD WINAPI UDPRecvThread(LPVOID lpParameter)
{
	TCPServer* tcpServer = reinterpret_cast<TCPServer*>(lpParameter);

	tcpServer->udp->Recv();

	return 0;
}

// TCPServer Constructor
TCPServer::TCPServer()
{
	myHandle = nullptr;
	port = -1;
	addressFamily = -1;
	packetType = -1;
	protocolType = -1;
	InitServer();
}

// TCPServer overloaded constructor (Port Number)
TCPServer::TCPServer(int portNumber)
{
	myHandle = nullptr;
	port = portNumber;
	addressFamily = -1;
	packetType = -1;
	protocolType = -1;
	InitServer();
}

// TCPServer overloaded constructor (Port Number, Address Family, Packet Type, Protocol Type)
TCPServer::TCPServer(int portNumber, int addressFamily, int packetType, int protocolType)
{
	myHandle = nullptr;
	port = portNumber;
	this->addressFamily = addressFamily;
	this->packetType = packetType;
	this->protocolType = protocolType;
	InitServer();
}

TCPServer::~TCPServer()
{
	if (myHandle != nullptr)
	{
		CloseHandle(myHandle);
	}
	// cleanup resources on server termination
	WSACleanup();
}

// Sever initialisation function
int TCPServer::InitServer()
{
	// initialise starting variables
	recvbuflen = bufLength;
	iRcvdBytes = 0;

	// (MAKEWORD(Major version, Minor version), LPWSADATA = setup information from windows)
	// WSA function return zero if working correctly
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// check result of WSAStartup function
	if (iResult == 0)
	{
		cout << "Server is Starting up..." << endl;

		listenSocket = Socket(port, addressFamily, packetType, protocolType);

		if (listenSocket.GetSocket() == INVALID_SOCKET)
		{
			PrintError("server socket invalid");
			return -1;
		}
		else
		{
			// setup listen socket
			udp = new UDP(27019, AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			one = new User();
			// set socket options, bind socket and freeaddressinfo
			listenSocket.InitialiseListenSocketBind();
		}

		// Start server loop
		ListenLoop();

		return 0;
	}
	else
	{
		// Output last error encountered
		PrintError("Server unable to start.");
		return -1;
	}
}

void TCPServer::CreateClientSocket()
{
	clientSocket = Socket(listenSocket);
}

int TCPServer::GetIntialisationError()
{
	return intialisationError;
}

int TCPServer::RecvUDP()
{
	// create thread using thread function and passing in this class
	myHandle = CreateThread(0, 0, UDPRecvThread, this, 0, &myThreadID);

	return 0;
}

int TCPServer::Listen()
{
	return 0;
}

int TCPServer::ListenLoop()
{
	cout << endl << "Listening for incoming connections... (Press Esc key to quit)" << endl;

	int i = 0;

	// Loop while escape key has not been pressed
	while (i < 10)
	{
		i++;

		// Server must listen on listensocket for incoming connections
		// (created listen socket, maximum connections allowed)
		if (listen(listenSocket.GetSocket(), SOMAXCONN) == SOCKET_ERROR)
		{
			PrintError("Listening failed");
		}

		// Create temporary socket to accept connections
		CreateClientSocket();

		// Accept client connection
		// blocking happens here
		// should be on a thread of its own if not a turn based game
		clientSocket = accept(listenSocket.GetSocket(), NULL, NULL);

		// check if accepting client connection was successful
		if (clientSocket.GetSocket() == INVALID_SOCKET)
		{
			PrintError("Accepting client connection failed");
		}


		// Send UDP data
		udp->Send();

		ReceiveStream(clientSocket.GetSocket());

		mes = Message(tempReceiveBuffer, bufLength);
		mes.ObjectReadSelector();
		one = mes.GetUser();
		//printf("buffer contents = %s", tempReceiveBuffer[mes.GetNewBufferPosition()]);

		// Shutdown connection
		clientSocket.ShutdownSocket();

		// Free up socket - Close Socket
		clientSocket.CloseSocket();

		// Function to spawn thread for UDP receive
		RecvUDP();

		// if escape key is hit, set loop bool to true
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			exit = true;
		}
	}

	return 0;
}

// WSA Error printing function
void TCPServer::PrintError(string input)
{
	// Output Server Port number and last error encountered
	cout << input << endl
		<< "Server Port Number: " << port << endl
		<< "WSA Error Code: " << WSAGetLastError() << endl;

	intialisationError = -1;
	listenSocket.CloseSocket();
	WSACleanup();
}

void TCPServer::ReceiveStream(Socket socket)
{
	unsigned int iResult = recv(socket.GetSocket(), tempReceiveBuffer, bufLength, 0);

	// if number of bytes received higher than 0
	if (iResult > 0)
	{
		cout << "Bytes Received: " << iResult << endl;
	}
	else
	{
		cout << "No Data Received" << endl;
	}
}