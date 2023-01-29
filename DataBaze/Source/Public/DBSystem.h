#pragma once

#include "framework.h"
#include "resource.h"

#include "DBSystem.h"

class DBWindowsManager;

#define MAX_LOADSTRING 100
#define IDC_LISTBOX	   9

class DBSystem
{
public:
	DBSystem(HINSTANCE* HInstance, HWND InMainWindow);
	~DBSystem();
	void EndConstruct();

	// Components
	DBWindowsManager* WindowManager = nullptr;
	ButtonContainer	  Buttons;

	// Global variables
	HWND	MainWindow;
	Size2D* BtnSize = nullptr;

	// End variables

	void CallCommand(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam);
	void CallPaint(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam);

	void Update_BtnVisibility();
	void Update_ListBoxScale();

	void HideButton(EDBButtonId Id);
	void ShowButton(EDBButtonId Id);

	bool IsPortraitModeEnabled();

	void SetFontSize(HWND Window, int Size);

	void SetWindowsViewer(HWND Window);
	void SetWindowsWriter(HWND Window);

private:
	HINSTANCE*		HIns;
	HWND			ListBox = 0;
	DBListContainer ListContainer;

	Size2D WindowSize;

	void CreateButton(const HWND& ParentWindow, std::wstring Text, EDBButtonId Id, Size2D Pos, Size2D Size);
	void InitListBox();

	static VOID CALLBACK DoubleClickTimer(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime);
};
