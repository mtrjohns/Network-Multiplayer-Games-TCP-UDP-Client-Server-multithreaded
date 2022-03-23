#pragma once
// static_cast<>, reference from - J. Glazer and S. Madhav, Multiplayer game programming. Pearson Education, Inc, 2016.

#include <cstdlib>
#include <algorithm>
#include <string>
#include <WinSock2.h> // winsock2 library
#include "PreAmble.h"

using std::free; using std::memcpy; using std::realloc;
using std::max;
using std::string;

class OutMemoryStream
{
public:
	OutMemoryStream();
	~OutMemoryStream();

	// construct OutMemoryStream  to allow c++ primitive data types without size needing to be passed
	
	// Main pack function
	// also facilitates char* directly
	void Pack(const void* buffer, unsigned int byteSize);

	// multiple type packs using main pack function
	void Pack(int buffer);
	void Pack(unsigned int buffer);
	void Pack(unsigned long buffer);
	void Pack(unsigned short buffer);

	unsigned int size;

	// get buffer length
	const unsigned int GetLength();
	char* GetBuffer();
	// Get pointer to buffer
	const char* GetBufferPtr();
	void Trim();

	void ResetBuffer();

private:
	char* buffer;

	// buffer length
	unsigned int length;
	// current position in buffer
	unsigned curPosition;

	// increase buffer size if not enough space remaining in current buffer
	void ResizeBuffer(int length);
};

