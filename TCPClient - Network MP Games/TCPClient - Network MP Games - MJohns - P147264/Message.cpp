#include "Message.h"

Message::Message()
{
	this->buffer = buffer;
	bufferLength = 0;
	user = nullptr;
}

Message::Message(char * buffer, unsigned int length)
{
	this->buffer = buffer;
	bufferLength = length;
	inStream = InMemoryStream(buffer, length);
	user = nullptr;
}

// Select which object to write or read from using object type in preamble
void Message::ObjectReadSelector()
{
	// Check object type and length
	ReadPreamble();

	switch (pre.objectType)
	{
	case 0: // Is User object
		ReadUser();
		if (bufferLength > 28)
		{
			newBufferPosition = 28;
		}
		break;
	default:
		break;
	}
}

void Message::ObjectWriteSelector()
{
	// Check object type and length
	ReadPreamble();

	switch (pre.objectType)
	{
	case 0: // Is User object
		WriteUser();
		break;
	default:
		break;
	}
}

Message::~Message()
{

}

OutMemoryStream Message::GetOutStream()
{
	return outStream;
}

InMemoryStream Message::GetInStream()
{
	return inStream;
}

User* Message::GetUser()
{
	return user;
}

void Message::SetUser(User *user)
{
	this->user = user;
}

unsigned int Message::GetNewBufferPosition()
{
	return newBufferPosition;
}

void Message::ReadPreamble()
{
	inStream.UnPack(pre.length);
	inStream.UnPack(pre.objectType);
}

OutMemoryStream Message::WriteUser()
{
	outStream.ResetBuffer();
	// calculate size of data to be sent
	pre.objectType = 0;
	pre.length = 0;
	pre.length = sizeof(user->preamble.length)
				+ sizeof(user->preamble.objectType)
				+ sizeof(user->uhealth)
				+ sizeof(user->charName);

	user->preamble.length = pre.length;
	user->preamble.objectType = pre.objectType;

	outStream.Pack(user->preamble.length);
	outStream.Pack(user->preamble.objectType);

	outStream.Pack(user->uhealth);
	outStream.Pack(user->health);

	// Possible to add a compression method here to shorten array amount
	// to only the character amount required for the entered name
	outStream.Pack(user->charName, sizeof(user->charName));

	cout << endl << "Sending on TCP to Server: " << endl;

	cout << "Preamble Length = " << user->preamble.length << endl
		<< "Preamble Object Type = " << user->preamble.objectType << endl
		<< "inUser uHealth = " << user->uhealth << endl
		<< "inUser Health = " << user->health << endl
		<< "inUser charName = " << user->charName << endl << endl;

	return outStream;
}

User * Message::ReadUser()
{
	user->preamble.length = pre.length;
	user->preamble.objectType = pre.objectType;
	inStream.UnPack(user->uhealth);
	inStream.UnPack(user->health);
	inStream.UnPack(user->charName, sizeof(user->charName));

	cout << endl << "Received on TCP from Server: " << endl;

	cout << "Preamble Length = " << user->preamble.length << endl
		<< "Preamble Object Type = " << user->preamble.objectType << endl
		<< "uHealth = " << user->uhealth << endl
		<< "Health = " << user->health << endl
		<< "charName = " << user->charName << endl << endl;

	return user;
}


OutMemoryStream Message::SendUDP(const char* send, unsigned int size)
{
	outStream = OutMemoryStream();
	outStream.Pack(send, size);
	outStream.ResetBuffer();

//	cout << "Outstream UDP = " << outStream.GetBufferPtr() << endl;
	return outStream;
}

InMemoryStream Message::ReceiveUDP(char* send, unsigned int size)
{
	// pack message
	inStream = InMemoryStream(send, size);

//	cout << "Instream UDP = " << send << endl;
	return inStream;
}