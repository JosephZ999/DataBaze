#pragma once

#include "Singleton/Singleton.h"

class DBMainSubsystem : public Singleton
{
public:
	int var1 = 19;
	int var2 = 0;
};

/*
Singletons:

DBInstance
{
	MainWindow;
	AllComponents;

	// other global variables
}


*/