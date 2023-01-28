#include "Source/Public/Utils/DBFunctionLibrary.h"

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
