#pragma once
#include <iostream>
#include "TCPClient.h"

#define _CRT_SECURE_NO_WARNINGS  1

using std::cout; using std::endl;

int main(int argc, char* argv[])
{
	TCPClient tcpClient;
	if (argc < 2) // if user has not passed in an argument to application
	{
		cout << endl <<  "Usage: " << argv[0] << endl << "Server Name" << endl
			<< "Using loopback address 127.0.0.1" << endl;
		
		// set server name to loopback address
		tcpClient = TCPClient("127.0.0.1", 27015, AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	else
	{
		// set server name to argument passed in by user
		cout << "Server Name Argument: " << argv[1] << endl;
		
		// Create Client
		tcpClient = TCPClient(argv[1], 27015, AF_INET, SOCK_STREAM, IPPROTO_TCP);

		cout << "Server Name Set to: " << tcpClient.GetServerName() << endl;
	}

	// Terminate Server if unable to intialise
	if (tcpClient.CheckIntialisationError() == -1)
	{
		return 1;
	}

	return 0;
}