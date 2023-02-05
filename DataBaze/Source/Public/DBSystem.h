#pragma once

#include "framework.h"
#include "resource.h"

#include "DBInterface.h"
#include "DBDataTypes.h"

class DBWindowsManager;
class DBDataManager;

#define MAX_LOADSTRING 100
#define IDC_LISTBOX	   9

#ifdef _M_IX86
typedef UINT UINT_PTR;
#endif

class DBSystem : public DBInterface
{
public:
	DBSystem(HINSTANCE HInstance, HWND InMainWindow);
	~DBSystem();

	// Global variables
	HWND		 MainWindow;
	const Size2D BtnSize = {150, 30};

private:
	// Components
	DBWindowsManager* WindowManager = nullptr;
	DBDataManager*	  DataManager	= nullptr;
	ButtonContainer	  Buttons;

public:
	void EndConstruct();

	// Delegate callbacks
	void OnDataUpdated();

	void CallCommand(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam);
	void CallPaint(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam);

	virtual DBInterface* GetSystem() override;

	void Update_BtnVisibility();
	void Update_ListBoxScale();

	void HideButton(EDBWinCompId Id);
	void ShowButton(EDBWinCompId Id);

	bool IsPortraitModeEnabled();


private:
	HINSTANCE		HIns;
	HWND			ListBox = 0;
	DBListContainer ListContainer;

	Size2D WindowSize;
	void   InitListBox();
	void   CreateListBox();
	void   CreateButtons();

	static VOID CALLBACK DoubleClickTimer(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime);
};
