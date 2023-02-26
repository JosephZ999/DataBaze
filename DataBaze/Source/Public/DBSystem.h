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
	typedef std::vector<std::wstring> ListElem;

public:
	DBSystem(HINSTANCE HInstance, HWND InMainWindow);

	// Global variables
	HWND	  MainWindow;
	DBSystem* ThisObj = nullptr;

private:
	HINSTANCE		HIns;
	HWND			ListBox = 0;
	DBListContainer ListContainer;
	Size2D			WindowSize;
	bool			MinimizedMode	= false;
	bool			AnyItemSelected = false;

	// Components
	FWindowContainer  Buttons;

	ListElem ListData;
	int		 ListBoxLastItem = 0;
	int		 ListItemCount	 = 0;

public:
	void EndConstruct();

	void CallCommand(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam);

	virtual DBInterface* GetSystem() const override;

private:
	void CreateButtons();
	void SetMinimizedMode(bool Enabled);
	void UpdateCount();

	static VOID CALLBACK DoubleClickTimer(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime);
};
