#include "DBFunctionLibrary.h"

void DBLib::SetWindowVisibility(HWND Window, bool bShow)
{
	ShowWindow(Window, (bShow) ? SW_SHOWDEFAULT : SW_HIDE);
}

HWND DBLib::CreateButton(DBButton& ButtonInfo)
{
	ButtonInfo.Window = CreateWindow(L"button",		  // Type
		ButtonInfo.Text.c_str(),					  // Text
		WS_VISIBLE | WS_CHILD,						  // Style
		ButtonInfo.Position.X, ButtonInfo.Position.Y, //
		ButtonInfo.Size.X, ButtonInfo.Size.Y,		  //
		ButtonInfo.Parent,							  //
		(HMENU)ButtonInfo.Id,						  // Id
		NULL, NULL);

	SetFontSize(ButtonInfo.Window, ButtonInfo.FontSize);
	return ButtonInfo.Window;
}

void DBLib::SetFontSize(HWND Window, int FontSize)
{
	LOGFONT logfont;
	ZeroMemory(&logfont, sizeof(LOGFONT));
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfHeight  = -(FontSize);
	HFONT hFont		  = CreateFontIndirect(&logfont);

	SendMessage(Window, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void DBLib::CreateText() {}

HWND DBLib::CreateComboBox(DBButton& ButtonInfo)
{
	ButtonInfo.Window = CreateWindow(L"EDIT",		  // Type
		ButtonInfo.Text.c_str(),					  // Text
		WS_BORDER | WS_VISIBLE | WS_CHILD,			  // Style
		ButtonInfo.Position.X, ButtonInfo.Position.Y, //
		ButtonInfo.Size.X, ButtonInfo.Size.Y,		  //
		ButtonInfo.Parent,							  //
		(HMENU)ButtonInfo.Id,						  // Id
		NULL, NULL);

	SetFontSize(ButtonInfo.Window, ButtonInfo.FontSize);
	return ButtonInfo.Window;
}

void DBLib::CopyToClipboard(HWND hwnd, const std::string& s)
{
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (! hg)
	{
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

void DBLib::PressKey(WORD Key)
{
	INPUT ip;
	ip.type			  = INPUT_KEYBOARD;
	ip.ki.wScan		  = 0;
	ip.ki.time		  = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.wVk		  = Key;
	ip.ki.dwFlags	  = 0;

	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}

void DBLib::PressKeys(WORD Key1, WORD Key2)
{
	INPUT ip;
	ip.type			  = INPUT_KEYBOARD;
	ip.ki.wScan		  = 0;
	ip.ki.time		  = 0;
	ip.ki.dwExtraInfo = 0;

	// Press Control
	ip.ki.wVk	  = Key1;
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));

	// Press V
	ip.ki.wVk = Key2;
	SendInput(1, &ip, sizeof(INPUT));

	// Release V
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));

	// Release Control
	ip.ki.wVk = Key1;
	SendInput(1, &ip, sizeof(INPUT));
}

void DBLib::PasteClipboard()
{
	PressKeys(VK_CONTROL, VK_V);
}
