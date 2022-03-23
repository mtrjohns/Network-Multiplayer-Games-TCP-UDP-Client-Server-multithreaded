#pragma once
#include "PreAmble.h"
#include "OutMemoryStream.h"
#include "InMemoryStream.h"
#include "User.h"
#include "Socket.h"

class Message
{
public:
	Message();
	Message(char * buffer, unsigned int length);
	Message(User user);
	void ObjectReadSelector();
	void ObjectWriteSelector();
	~Message();

	OutMemoryStream GetOutStream();
	InMemoryStream GetInStream();

	User* GetUser();
	unsigned int GetNewBufferPosition();

	void ReadPreamble();

	OutMemoryStream WriteUser();
	User* ReadUser();

	OutMemoryStream SendUDP(const char * send, unsigned int size);

	InMemoryStream ReceiveUDP(char * send, unsigned int size);


private:
	unsigned int newBufferPosition;
	char* buffer;
	unsigned int bufferLength;
	PreAmble pre;
	User user;
	User user1;
	OutMemoryStream outStream;
	InMemoryStream inStream;

};

