#pragma once

#include "Singleton/Singleton.h"
#include <windows.h>

struct FDBInstanceInit
{
	FDBInstanceInit() {}

	explicit FDBInstanceInit(HINSTANCE InHIns, HWND InMainWindow)
		: HIns(InHIns)
		, MainHWND(InMainWindow)
	{
	}
	HINSTANCE HIns	   = 0;
	HWND	  MainHWND = 0;
};

class DBInstance : public Singleton
{
public:
private:
	bool			Initialized = false;
	FDBInstanceInit InitData;

public:
	void Initialize(FDBInstanceInit& Param);
	FDBInstanceInit GetInitData() const;
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
