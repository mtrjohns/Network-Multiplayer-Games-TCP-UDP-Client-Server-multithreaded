// Memory stream to unpack data from network byte order to host byte order
#include "InMemoryStream.h"

InMemoryStream::InMemoryStream()
{
	this->buffer = nullptr;
	length = 0;
	curPosition = 0;
}

InMemoryStream::InMemoryStream(char* buffer, unsigned int byteSize)
{
	this->buffer = buffer;
	length = byteSize;
	curPosition = 0;
}

InMemoryStream::~InMemoryStream()
{

}

unsigned int InMemoryStream::GetRemainingDataSize()
{
	return length - curPosition;
}

// Return length of buffer
const unsigned int InMemoryStream::GetLength()
{
	int temp = length - curPosition;
	temp = length - temp;
	return temp;
}

// Return pointer to buffer
const char * InMemoryStream::GetBufferPtr()
{
	return this->buffer;
}

// reset buffer current position pointer to start of buffer
void InMemoryStream::ResetCurrentPosition()
{
	curPosition = 0;
}

// Default unpack function
void InMemoryStream::UnPack(void * buffer, unsigned int byteSize)
{
	// new position is current plus input bytesize
	unsigned int iResult = curPosition + byteSize;
	if (iResult > length)
	{
		cout << "Buffer Empty" << endl;
	}

	// memcpy old data and new data
	memcpy(buffer, this->buffer + curPosition, byteSize);

	curPosition = iResult;
}

// Unpack int using default unpack function
void InMemoryStream::UnPack(int &buffer)
{
	UnPack(&buffer, sizeof(buffer));
	buffer = (int)ntohl((unsigned long)buffer);
}

// Unpack unsigned int using default unpack function
void InMemoryStream::UnPack(unsigned int &buffer)
{
	UnPack(&buffer, sizeof(buffer));
	buffer = (unsigned int)ntohl((unsigned long)buffer);
}

// Unpack unsigned long using default unpack function
void InMemoryStream::UnPack(unsigned long &buffer)
{
	UnPack(&buffer, sizeof(buffer));
	buffer = ntohl(buffer);
}

// Unpack short using default unpack function
void InMemoryStream::UnPack(unsigned short &buffer)
{
	UnPack(&buffer, sizeof(buffer));
	buffer = ntohs(buffer);
}