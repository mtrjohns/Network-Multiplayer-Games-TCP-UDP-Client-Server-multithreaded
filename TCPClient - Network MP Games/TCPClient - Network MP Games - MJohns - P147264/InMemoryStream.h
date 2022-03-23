#pragma once

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <WinSock2.h> // winsock2 library

using std::free; using std::memcpy;
using std::cout; using std::endl;
using std::string;

class InMemoryStream
{
public:
	InMemoryStream();
	InMemoryStream(char* buffer, unsigned int byteSize);
	~InMemoryStream();

	unsigned int GetRemainingDataSize();
	const unsigned int GetLength();
	const char* GetBufferPtr();
	void ResetCurrentPosition();
	void UnPack(void* buffer, unsigned int byteSize);

	// multiple type packs using main pack function
	void UnPack(int &buffer);
	void UnPack(unsigned int &buffer);
	void UnPack(unsigned long &buffer);
	void UnPack(unsigned short &buffer);

private:
	char* buffer;
	unsigned int length;
	unsigned int curPosition;
};

