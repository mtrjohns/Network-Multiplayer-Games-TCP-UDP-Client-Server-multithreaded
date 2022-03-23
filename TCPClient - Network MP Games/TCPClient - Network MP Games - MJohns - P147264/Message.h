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

	void ObjectReadSelector();
	void ObjectWriteSelector();
	~Message();

	OutMemoryStream GetOutStream();
	InMemoryStream GetInStream();

	User* GetUser();
	void SetUser(User*);
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
	User *user;
	OutMemoryStream outStream;
	InMemoryStream inStream;

};

