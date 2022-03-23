#include "TCPClient.h"

DWORD WINAPI UDPRecvThread(LPVOID lpParameter)
{
	TCPClient* tcpClient = reinterpret_cast<TCPClient*>(lpParameter);

	tcpClient->udp->Recv();
	return 0;
}

TCPClient::TCPClient()
{
	port = -1;
	this->addressFamily = -1;
	this->packetType = -1;
	this->protocolType = -1;
	serverName = "127.0.0.1";
}

TCPClient::TCPClient(const char* serverName, int portNumber, int addressFamily, int packetType, int protocolType)
{
	port = portNumber;
	SetServerName(serverName);
	this->addressFamily = addressFamily;
	this->packetType = packetType;
	this->protocolType = protocolType;
	InitClient();
}


TCPClient::~TCPClient()
{

}

void TCPClient::InitClient()
{
	// initialise starting variables
	recvbuflen = bufLength;

	// (MAKEWORD(Major version, Minor version), LPWSADATA = setup information from windows)
	// WSA function return zero if working correctly
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// check result of WSAStartup function
	if (iResult == 0)
	{
		cout << "Client starting..." << endl;
		CreateConnectSocket(addressFamily, packetType, protocolType);
		// setup listen socket
		udp = new UDP(27019, AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		Loop();
	}
	else
	{
		// Output last error encountered
		PrintError("Client unable to start.");
	}
}

void TCPClient::CreateConnectSocket(int addressFamily, int packetType, int protocolType)
{
	// Create a listening socket to be able to connect to server
	connectSocket = Socket(serverName, port, addressFamily, packetType, protocolType);
	if (connectSocket.GetSocket() == INVALID_SOCKET)
	{
		PrintError("connecting socket invalid");
	}
}

void TCPClient::SetServerName(const char* input)
{
	if (input != NULL)
	{
		string tempString(input);
		serverName = tempString;
	}
}

string TCPClient::GetServerName()
{
	return serverName;
}

int TCPClient::CheckIntialisationError()
{
	return intialisationError;
}

int TCPClient::RecvUDP()
{
	// create thread using thread function and passing in this class
	myHandle = CreateThread(0, 0, UDPRecvThread, this, 0, &myThreadID);

	return 0;
}

void TCPClient::Loop()
{
	User* one;
	one = new User();
	mes.SetUser(one);

	int i = 0;

	// Loop while escape key has not been pressed
	while (i < 10)
	{
		i++;
		// Create a socket to connect with
		CreateConnectSocket(addressFamily, packetType, protocolType);

		// Attempt to connect to an address until one succeeds
		for (connectSocket.SetPtrToAddressList(); connectSocket.GetPtr() != NULL; connectSocket.SetPtr(connectSocket.GetPtr()->ai_next))
		{
			// Connect to server.
			iResult = connect(connectSocket.GetSocket(), connectSocket.GetPtr()->ai_addr, (int)connectSocket.GetPtr()->ai_addrlen);
			if (iResult == SOCKET_ERROR)
			{
				cout << "Unable to Connect to Server" << endl;

				connectSocket.SetSocketInvalid();
				connectSocket.CloseSocket();
				exit = true;
				continue;
			}

			break;
		}

		one->UserConsoleEntry();

		// send udp message
		udp->Send();

		SendStream(mes.WriteUser());
		
		// Function to spawn thread for UDP receive
		RecvUDP();

		// if escape key is hit, set loop bool to true
		if (GetAsyncKeyState(VK_ESCAPE))
		{

			exit = true;
		}
		connectSocket.FreeAddressInfo();
		// free up resources
		connectSocket.ShutdownSocket();
		connectSocket.CloseSocket();

	}

	delete one;
}

// WSA Error printing function
void TCPClient::PrintError(string input)
{
	// Output Server Port number and last error encountered
	cout << input << endl
		<< "Unable to connect to server: " << serverName << " " << endl
		<< "WSA Error Code: " << WSAGetLastError() << endl;

	intialisationError = -1;
	WSACleanup();
}

void TCPClient::SendStream(OutMemoryStream outStream)
{

	unsigned int iResult = send(connectSocket.GetSocket(), outStream.GetBufferPtr(), outStream.GetLength(), 0);
	
	if (iResult == SOCKET_ERROR)
	{
		cout << "User Failed to send! No Data Sent!" << endl;
	}
	else
	{
		cout << "Bytes Sent: " << outStream.GetLength() << endl;

	}
}