// Memory stream to pack data from host byte order to network byte order
#include "OutMemoryStream.h"

OutMemoryStream::OutMemoryStream()
{
	buffer = nullptr;
	length = 0;
	curPosition = 0;
	ResizeBuffer(32);
}


OutMemoryStream::~OutMemoryStream()
{
	//free(buffer);
}

// use this for char / char*
void OutMemoryStream::Pack(const void * buffer, unsigned int byteSize)
{
	// check buffer is big enough ( current buffer position + new type)
	unsigned int iResult = curPosition + static_cast<unsigned int>(byteSize);
	
	// printf("iResult = %d, current position = %d, bytesize = %d\n", iResult, curPosition, byteSize);
	
	if (iResult > length) // if greater than current memory allocated for buffer
	{
		// resize buffer to length *2 or iResult, whichever is larger
		ResizeBuffer(max(length * 2, iResult)); 
	}

	// copy into buffer from current buffer position
	memcpy(this->buffer + curPosition, buffer, byteSize);

	curPosition = iResult;
}

// Type int
void OutMemoryStream::Pack(int buffer)
{
	buffer = (int)htonl(buffer);
	Pack(&buffer, sizeof(buffer));
}

// Type unsigned int
void OutMemoryStream::Pack(unsigned int buffer)
{
	buffer = (unsigned int)htonl(buffer);
	Pack(&buffer, sizeof(buffer));
}

// Type long
void OutMemoryStream::Pack(unsigned long buffer)
{
	buffer = htonl(buffer);
	Pack(&buffer, sizeof(buffer));
}

// Type short
void OutMemoryStream::Pack(unsigned short buffer)
{
	buffer = htons(buffer);
	Pack(&buffer, sizeof(buffer));
}

// Return length of buffer
const unsigned int OutMemoryStream::GetLength()
{
	return length;
}

// Return pointer to buffer
char * OutMemoryStream::GetBuffer()
{
	return this->buffer;
}

const char * OutMemoryStream::GetBufferPtr()
{
	return this->buffer;
}

// resize buffer to only send required byte amount
void OutMemoryStream::Trim()
{
	ResizeBuffer(curPosition);
}

// reset buffer current position pointer to start of buffer
void OutMemoryStream::ResetBuffer()
{
	curPosition = 0;
}

void OutMemoryStream::ResizeBuffer(int length)
{
	// set new buffer length
	buffer = static_cast<char*>(realloc(buffer, length));
	// set buffer length to newly calculated length
	this->length = length;
	
	//printf("Buffer Size = %d", this->length);
}
