#pragma once

#include "Singleton/Singleton.h"
#include "DBInterface.h"
#include <windows.h>

class DBDataManager;
class DBWindowsManager;

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

class DBInstance : public Singleton, public DBInterface
{
public:
	DBInstance();

private:
	DBWindowsManager* WindowManager;
	DBDataManager*	  DataManager;

	bool			Initialized = false;
	FDBInstanceInit InitData;

public:
	inline DBWindowsManager* GetWindowManager() const { return WindowManager; }
	inline DBDataManager*	 GetDataManager() const { return DataManager; }

public:
	void			Initialize(FDBInstanceInit& Param);
	FDBInstanceInit GetInitData() const;
};
