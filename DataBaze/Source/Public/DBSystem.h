#pragma once

#include "framework.h"
#include "resource.h"

#include "Source/DBDataTypes.h"


class DBSystem
{
public:
	DBSystem(HINSTANCE* HInstance);
	~DBSystem();

	ButtonContainer Buttons;

	void CallCommand(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam);
	void CallPaint(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam);

	void Update_BtnVisibility();
	void Update_ListBoxScale();

	void HideButton(DBButtonId Id);
	void ShowButton(DBButtonId Id);

	bool IsPortraitModeEnabled();

	void SetFontSize(HWND Window, int Size);

private:
	HINSTANCE*		HIns;
	HWND			ListBox;
	DBListContainer ListContainer;

	Size2D WindowSize;

	void CreateButton(const HWND& ParentWindow, std::wstring Text, DBButtonId Id, Size2D Pos, Size2D Size);
	void InitListBox();
};