//Messages to process from network input
#include "Message.h"

Message::Message()
{
	this->buffer = buffer;
	bufferLength = 0;
}

Message::Message(char * buffer, unsigned int length)
{
	this->buffer = buffer;
	bufferLength = length;
	inStream = InMemoryStream(buffer, length);
}

Message::Message(User user)
{
	this->user = user;
}

// Select which object to write or read from using object type in preamble
void Message::ObjectReadSelector()
{
	// Check object type and length
	ReadPreamble();

	switch (pre.objectType)
	{
		case 0: // Is User object
			ReadUser(); // read user object
			if (bufferLength > 28)
			{
				// currently hardcoded buffer position
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
	// add more objects here with case
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
	return &user;
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
	// calculate size of data to be sent
	pre.objectType = 0;
	pre.length = 0;
	pre.length = sizeof(user.preamble.length)
				+ sizeof(user.preamble.objectType)
				+ sizeof(user.uhealth)
				+ sizeof(user.health)
				+ sizeof(user.charName);

	user.preamble.length = pre.length;
	user.preamble.objectType = pre.objectType;

	// Pack data to outstream
	outStream.Pack(user.preamble.length);
	outStream.Pack(user.preamble.objectType);

	outStream.Pack(user.uhealth);
	outStream.Pack(user.health);

	// Possible to add a compression method here to shorten array amount
	// to only the character amount required for the entered name
	outStream.Pack(user.charName, sizeof(user.charName));

	cout << endl << "Sending on TCP to Client: " << endl;

	cout << "Preamble Length = " << user.preamble.length << endl
		<< "Preamble Object Type = " << user.preamble.objectType << endl
		<< "inUser uHealth = " << user.uhealth << endl
		<< "inUser Health = " << user.health << endl
		<< "inUser charName = " << user.charName << endl << endl;

	return outStream;
}

User * Message::ReadUser()
{
	user.preamble.length = pre.length;
	user.preamble.objectType = pre.objectType;
	inStream.UnPack(user.uhealth);
	inStream.UnPack(user.health);
	inStream.UnPack(user.charName, sizeof(user.charName));
	
	cout << endl << "Received on TCP from Client: " << endl;

	cout << "Preamble Length = " << user.preamble.length << endl
		<< "Preamble Object Type = " << user.preamble.objectType << endl
		<< "uHealth = " << user.uhealth << endl
		<< "Health = " << user.health << endl
		<< "charName = " << user.charName << endl << endl;

	return &user;
}

OutMemoryStream Message::SendUDP(const char* send, unsigned int size)
{
	outStream = OutMemoryStream();
	outStream.Pack(send, size);
	//cout << "Outstream UDP = " << send << endl;
	return outStream;
}


InMemoryStream Message::ReceiveUDP(char* send, unsigned int size)
{
	inStream = InMemoryStream(send, size);
	inStream.ResetCurrentPosition();
	//cout << "Instream UDP = " << inStream.GetBufferPtr() << endl;
	return inStream;
}