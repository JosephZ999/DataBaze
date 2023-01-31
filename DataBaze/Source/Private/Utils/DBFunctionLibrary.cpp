#include "DBFunctionLibrary.h"

typedef EDBWindowCompType CompType;

void DBLib::SetWindowVisibility(HWND Window, bool bShow)
{
	ShowWindow(Window, (bShow) ? SW_SHOWDEFAULT : SW_HIDE);
}

HWND DBLib::CreateWindowComponent(EDBWindowCompType Type, std::wstring& Text, DWORD Style, Size2D Pos, Size2D Size, HWND Parent, HMENU Id)
{
	std::wstring CompType;
	switch (Type)
	{ // clang-format off
	case WCT_Static:	CompType = L"static";	break;
	case WCT_Button:	CompType = L"button";	break;
	case WCT_Edit:		CompType = L"edit";		break;
	case WCT_ComboBox:	CompType = L"combobox"; break;
	} // clang-format on

	return CreateWindow(CompType.c_str(), // Type
		Text.c_str(),					  // Text
		Style,							  // Style
		Pos.X, Pos.Y, Size.X, Size.Y,	  //
		Parent,							  //
		Id,								  // Id
		NULL, NULL);
}

HWND DBLib::CreateButton(DBWindow& ButtonInfo)
{
	DWORD BStyle	  = WS_VISIBLE | WS_CHILD;
	ButtonInfo.Window = CreateWindowComponent(
		CompType::WCT_Button, ButtonInfo.Text, BStyle, ButtonInfo.Position, ButtonInfo.Size, ButtonInfo.Parent, (HMENU)ButtonInfo.Id);

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

void DBLib::CreateText(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC			hdc		   = BeginPaint(hWnd, &ps);
	TCHAR		greeting[] = _T("Hello, Windows desktop!");
	TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
	EndPaint(hWnd, &ps);
}

HWND DBLib::CreateEditBox(DBWindow& ButtonInfo, DWORD Style)
{
	ButtonInfo.Window = CreateWindowComponent(
		CompType::WCT_Edit, ButtonInfo.Text, Style, ButtonInfo.Position, ButtonInfo.Size, ButtonInfo.Parent, (HMENU)ButtonInfo.Id);
	SetFontSize(ButtonInfo.Window, ButtonInfo.FontSize);
	return ButtonInfo.Window;
}

HWND DBLib::CreateStaticBox(DBWindow& WindowInfo, DWORD Style)
{
	WindowInfo.Window = CreateWindowComponent(
		CompType::WCT_Static, WindowInfo.Text, Style, WindowInfo.Position, WindowInfo.Size, WindowInfo.Parent, (HMENU)WindowInfo.Id);

	SetFontSize(WindowInfo.Window, WindowInfo.FontSize);
	return WindowInfo.Window;
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
