#pragma once

#include "framework.h"
#include "resource.h"

#include "DBDataTypes.h"

class DBWindowsManager;

#define MAX_LOADSTRING 100
#define IDC_LISTBOX	   9

class DBSystem
{
public:
	DBSystem(HINSTANCE HInstance, HWND InMainWindow);
	~DBSystem();
	void EndConstruct();

	// Components
	DBWindowsManager* WindowManager = nullptr;
	ButtonContainer	  Buttons;

	// Global variables
	HWND	MainWindow;
	const Size2D BtnSize = {150, 30};

	// End variables

	void CallCommand(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam);
	void CallPaint(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam);

	void Update_BtnVisibility();
	void Update_ListBoxScale();

	void HideButton(EDBWinCompId Id);
	void ShowButton(EDBWinCompId Id);

	bool IsPortraitModeEnabled();

	void SetFontSize(HWND Window, int Size);

private:
	HINSTANCE		HIns;
	HWND			ListBox = 0;
	DBListContainer ListContainer;

	Size2D WindowSize;
	void InitListBox();

	static VOID CALLBACK DoubleClickTimer(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime);
};
