#pragma once
#include <iostream>
#include "TCPServer.h"

using std::cout; using std::endl;


int main(int argc, char* argv[])
{
	cout << "Server IP = 127.0.0.1" << endl;

	TCPServer tcpServer = TCPServer(27015, AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Terminate Server if unable to intialise
	if (tcpServer.GetIntialisationError() == -1)
	{
		return 1;
	}

	return 0;
} 