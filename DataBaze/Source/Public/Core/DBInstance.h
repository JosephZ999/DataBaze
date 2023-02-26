#pragma once

#include "Singleton/Singleton.h"
#include "DBInterface.h"
#include <windows.h>

class DBDataManager;
class DBWindowsManager;
class DBListBox;

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
private:
	DBWindowsManager* WindowManager = nullptr;
	DBDataManager*	  DataManager	= nullptr;
	DBListBox*		  ListBox		= nullptr;

	bool			Initialized = false;
	FDBInstanceInit InitData;

public:
	inline DBWindowsManager* GetWindowManager() const { return WindowManager; }
	inline DBDataManager*	 GetDataManager() const { return DataManager; }

public:
	void			Initialize(FDBInstanceInit& Param);
	FDBInstanceInit GetInitData() const;

	void OnMemberAddedHandle();

private:
	void InitWindows();
};
