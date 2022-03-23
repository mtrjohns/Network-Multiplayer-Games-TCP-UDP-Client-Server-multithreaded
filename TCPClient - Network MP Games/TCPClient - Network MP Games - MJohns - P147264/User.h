#pragma once
#define _CRT_SECURE_NO_WARNINGS  1

#include <iostream>
#include <string>

#include "PreAmble.h"

using std::cout; using std::endl; using std::cin;

class User
{
public:
	User();
	User(int health, char charName);
	~User();

	PreAmble GetPreAmble();
	unsigned int GetuHealth();
	int GetHealth();
	char* GetCharName();

	void SetPreAmble(PreAmble pre);
	void SetuHealth(unsigned int uHealth);
	void SetHealth(int health);
	void SetCharName(char* name);

	void UserConsoleEntry();

	// Should be private below, for future revision
	PreAmble preamble;
	
	unsigned int uhealth;
	int health;
	char charName[20];
};

