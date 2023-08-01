#pragma once

#include "Singleton/Singleton.h"
#include "DBInterface.h"
#include <windows.h>

class DBDataManager;
class DBWindowsManager;
class DBListBox;
class DBButtonManager;
class DBSettingsComp;

#ifdef _M_IX86
typedef UINT UINT_PTR;
#endif

struct FDBInstanceInit
{
	FDBInstanceInit() {}

	explicit FDBInstanceInit(HINSTANCE InHIns, HWND InMainWindow)
		: HIns(InHIns)
		, MainHWND(InMainWindow)
	{}
	HINSTANCE HIns	   = 0;
	HWND	  MainHWND = 0;
};

class DBInstance : public Singleton, public DBInterface
{
private:
	DBWindowsManager* WindowManager = nullptr;
	DBDataManager*	  DataManager	= nullptr;
	DBListBox*		  ListBox		= nullptr;
	DBButtonManager*  ButtonManager = nullptr;
	DBSettingsComp*	  SettingsComp	= nullptr;

	bool			Initialized = false;
	FDBInstanceInit InitData;
	bool			bMinimizeMode = false;

public:
	inline DBWindowsManager* GetWindowManager() const { return WindowManager; }
	inline DBDataManager*	 GetDataManager() const { return DataManager; }
	inline DBListBox*		 GetListBox() const { return ListBox; }
	inline DBButtonManager*	 GetButtonManager() const { return ButtonManager; }
	inline DBSettingsComp*	 GetSettingsComp() const { return SettingsComp; }

	FDBInstanceInit GetInitData() const;

public:
	void Initialize(FDBInstanceInit& Param);
	void OnMemberAddedHandle();
	void OnMemberChangedHandle();

	void CallCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	void InitWindows();
	void ResetListBox();
	void UpdateFolderIdText();
	void SetMinimizeMode(bool Enabled);
	bool IsMinimized() { return bMinimizeMode; }

public:
	virtual void Destroy() override;
	// virtual bool CanBeDestroyed() const override;
};
