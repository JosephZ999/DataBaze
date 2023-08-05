#include "DBFunctionLibrary.h"

#include <string>
#include <codecvt>
#include <locale>

#include "DBInstance.h"

#pragma warning(disable : 4996)

typedef EDBWindowType CompType;

void DBLib::SetWindowVisibility(HWND Window, bool bShow)
{
	ShowWindow(Window, (bShow) ? SW_SHOWDEFAULT : SW_HIDE);
}

HWND DBLib::CreateWindowComponent(
	EDBWindowType Type, std::wstring Text, DWORD Style, Size2D Pos, Size2D Size, HWND Parent, HMENU Id, HINSTANCE HIns)
{
	std::wstring CompType;
	switch (Type)
	{ // clang-format off
	case WT_Static:		CompType = L"static";	break;
	case WT_Button:		CompType = L"button";	break;
	case WT_Edit:		CompType = L"edit";		break;
	case WT_ComboBox:	CompType = L"combobox"; break;
	case WT_ListBox:	CompType = L"listbox";	break;
	} // clang-format on

	return CreateWindow(CompType.c_str(), // Type
		Text.c_str(),					  // Text
		Style,							  // Style
		Pos.X, Pos.Y, Size.X, Size.Y,	  //
		Parent,							  //
		Id,								  // Id
		HIns, NULL);
}

HWND DBLib::CreateButton(DBWindow& ButtonInfo)
{
	DWORD BStyle	  = WS_VISIBLE | WS_CHILD;
	ButtonInfo.Window = CreateWindowComponent(
		CompType::WT_Button, ButtonInfo.Text, BStyle, ButtonInfo.Position, ButtonInfo.Size, ButtonInfo.Parent, (HMENU)ButtonInfo.Id);

	SetFontSize(ButtonInfo.Window, ButtonInfo.FontSize);
	return ButtonInfo.Window;
}

HWND DBLib::CreateButton(const HWND OwnerWnd, const FWndItem& ButtonInfo)
{
	DWORD BStyle = WS_VISIBLE | WS_CHILD;
	HWND  Handle = CreateWindowComponent(
		 CompType::WT_Button, ButtonInfo.Text, BStyle, ButtonInfo.Position, ButtonInfo.Size, OwnerWnd, (HMENU)ButtonInfo.Id);

	SetFontSize(Handle, ButtonInfo.FontSize);
	return Handle;
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

void DBLib::SetText(HWND Window, std::wstring Text)
{
	if (Window)
	{
		SendMessage(Window, WM_SETTEXT, 0, (LPARAM)Text.c_str());
	}
}

HWND DBLib::CreateEditBox(DBWindow& ButtonInfo, DWORD Style)
{
	ButtonInfo.Window = CreateWindowComponent(CompType::WT_Edit, ButtonInfo.Text, Style, ButtonInfo.Position, ButtonInfo.Size,
		ButtonInfo.Parent, (HMENU)ButtonInfo.Id, ButtonInfo.HIns);
	SetFontSize(ButtonInfo.Window, ButtonInfo.FontSize);
	return ButtonInfo.Window;
}

HWND DBLib::CreateStaticBox(DBWindow& WindowInfo, DWORD Style)
{
	WindowInfo.Window = CreateWindowComponent(CompType::WT_Static, WindowInfo.Text, Style, WindowInfo.Position, WindowInfo.Size,
		WindowInfo.Parent, (HMENU)WindowInfo.Id, WindowInfo.HIns);

	SetFontSize(WindowInfo.Window, WindowInfo.FontSize);
	return WindowInfo.Window;
}

void DBInput::CopyToClipboard(HWND hwnd, const std::string& String)
{
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, String.size() + 1);
	if (hg)
	{
		memcpy(GlobalLock(hg), String.c_str(), String.size() + 1);
		GlobalUnlock(hg);
		SetClipboardData(CF_TEXT, hg);
		CloseClipboard();
		GlobalFree(hg);
	}
	else
	{
		CloseClipboard();
	}
}

void DBInput::CopyToClipboard(HWND hwnd, const std::wstring& WString)
{
	if (OpenClipboard(hwnd))
	{
		EmptyClipboard();
		HGLOBAL hClipboardData;
		hClipboardData = GlobalAlloc(GMEM_DDESHARE, sizeof(WCHAR) * (wcslen(WString.c_str()) + 1));
		WCHAR* pchData;
		pchData = (WCHAR*)GlobalLock(hClipboardData);
		wcscpy(pchData, WString.c_str());
		GlobalUnlock(hClipboardData);
		SetClipboardData(CF_UNICODETEXT, hClipboardData);
		CloseClipboard();
	}
}

void DBInput::PressKey(WORD Key)
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

void DBInput::PressKeys(WORD Key1, WORD Key2)
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

void DBInput::PasteClipboard()
{
	PressKeys(VK_CONTROL, VK_V);
}

Size2D DBLib::GetScreenSize()
{
	return GetScreenSize(GetDesktopWindow());
}

Size2D DBLib::GetScreenSize(HWND Window)
{
	RECT Rect;
	if (GetWindowRect(Window, &Rect))
	{
		return Size2D(Rect.right, Rect.bottom);
	}
	return Size2D();
}

RECT DBLib::GetWindowRect(HWND Window)
{
	RECT Rect;
	if (GetWindowRect(Window, &Rect))
	{
		return Rect;
	}
	return RECT();
}

// namespace // Path ------------------------------------->

std::wstring DBPaths::GetProjectPath()
{
	TCHAR buffer[MAX_PATH] = {0};
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}

std::wstring DBPaths::GetDataPath()
{
	return DBPaths::GetProjectPath().append(L"\\Data");
}

std::wstring DBPaths::GetDataFolderPath(int Folder)
{
	return DBPaths::GetDataPath().append(L"\\Folder_").append(std::to_wstring(Folder));
}

std::wstring DBPaths::GetImagePath(int Folder, int ImageId)
{
	const std::wstring Prefix	 = (ImageId < 10) ? L"00" : ((ImageId < 100) ? L"0" : L"");
	const std::wstring ImageName = std::wstring(Prefix).append(std::to_wstring(ImageId)).append(L".jpg");
	return GetDataFolderPath(Folder).append(L"\\").append(ImageName);
}

std::wstring DBPaths::GetConfirmationPath(int FolderId)
{
	const std::wstring ConfirmPath = DBPaths::GetDataPath().append(L"\\Confirmation");
	const std::wstring FolderPath  = std::wstring(ConfirmPath).append(L"\\Folder_").append(std::to_wstring(FolderId));
	_wmkdir(ConfirmPath.c_str());
	_wmkdir(FolderPath.c_str());
	return FolderPath;
}

std::wstring DBPaths::GenerateConfirmFileName(const DBFamilyData& MemberData, const FMemberId& MemberId)
{
	const std::string MemberName  = MemberData.Parents[0].Name;
	const std::string MemberFName = MemberData.Parents[0].FamilyName;
	const std::string StringName =
		std::string("\\").append(std::to_string(MemberId.MemberId)).append(" - ").append(MemberFName).append(" ").append(MemberName);

	std::wstring WStr;
	DBConvert::StringToWString(StringName, WStr);
	return WStr;
}

void DBPaths::CreatePath(std::wstring NewPath)
{
	_wmkdir(NewPath.c_str());
}

// Converter
using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter;

void DBConvert::StringToWString(const std::string& String, std::wstring& OutWString)
{
	OutWString = strconverter.from_bytes(String);
}

void DBConvert::WStringToString(const std::wstring& WString, std::string& OutString)
{
	OutString = strconverter.to_bytes(WString);
}

int DBConvert::StringToInt(const std::string& InString)
{
	if (InString.size() == 0)
	{
		return 0;
	}
	return std::stoi(InString);
}

void DBDebug::CreateMessageBox(std::string Text)
{
	std::wstring WText;
	DBConvert::StringToWString(Text, WText);
	MessageBox(NULL, WText.c_str(), L"Dialog Box", MB_OK);
}

void DBDebug::CreateMessageBox(std::wstring Text)
{
	MessageBox(NULL, Text.c_str(), L"Dialog Box", MB_OK);
}
