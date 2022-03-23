// Example user class
#include "User.h"

User::User()
{
	preamble.length = -1;
	preamble.objectType = -1;
	uhealth = 0;
	health = 0;
	sprintf(charName, "%s", "Hi");
}

User::User(int health, char charName)
{
	preamble.length = -1;
	preamble.objectType = -1;
	uhealth = 0;
	this->health = health;
	sprintf(this->charName, "%s", charName);
}

User::~User()
{
}

PreAmble User::GetPreAmble()
{
	return preamble;
}

unsigned int User::GetuHealth()
{
	return uhealth;
}

int User::GetHealth()
{
	return health;
}

char * User::GetCharName()
{
	return charName;
}

void User::SetPreAmble(PreAmble pre)
{
	preamble = pre;
}

void User::SetuHealth(unsigned int uHealth)
{
	this->uhealth = uhealth;
}

void User::SetHealth(int health)
{
	this->health = health;
}

void User::SetCharName(char * name)
{
	sprintf(this->charName, "%s", name);
}

void User::UserConsoleEntry()
{
	cout << endl << "Please enter starting health (int): ";
	cin >> health;
	cout << "Please enter starting health (unsigned int): ";
	cin >> uhealth;
	cout << "Please enter a character name (char*): ";
	cin >> charName;
}