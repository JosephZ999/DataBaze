#pragma once

#include "framework.h"
#include "Resource.h"
#include <string>
#include <vector>
#include "DBDataTypes.h"
#include "DBKeyCodes.h"

namespace DBLib
{

void SetWindowVisibility(HWND Window, bool bShow);
HWND CreateWindowComponent(
	EDBWindowCompType Type, std::wstring& Text, DWORD Style, Size2D Pos, Size2D Size, HWND Parent, HMENU Id, HINSTANCE HIns = NULL);
HWND CreateButton(DBWindow& ButtonInfo);
HWND CreateEditBox(DBWindow& ButtonInfo, DWORD Style);
HWND CreateStaticBox(DBWindow& WindowInfo, DWORD Style);
void SetFontSize(HWND Window, int FontSize);
void CreateText(HWND hWnd);
void SetText(HWND Window, std::wstring Text);

Size2D GetScreenSize();
Size2D GetScreenSize(HWND Window);
RECT   GetWindowRect(HWND Window);
} // namespace DBLib

namespace DBInput
{
void CopyToClipboard(HWND hwnd, const std::string& String);
void PressKey(WORD Key);
void PressKeys(WORD Key1, WORD Key2);
void PasteClipboard();
} // namespace DBInput

namespace DBPaths
{
std::wstring GetProjectPath();
std::wstring GetDataPath();
std::wstring GetDataFolderPath(int Folder);
std::wstring GetImagePath(int Folder, int ImageId);

void CreatePath(std::wstring NewPath);
} // namespace DBPaths

namespace DBConvert
{
void StringToWString(const std::string& String, std::wstring& OutWString);
void WStringToString(const std::wstring& WString, std::string& OutString);
int	 StringToInt(const std::string& InString);
} // namespace DBConvert

namespace DBDebug
{
void CreateMessageBox(std::string Text);
}

template <typename To, typename From> //
To* Cast(From* Param)
{
	return dynamic_cast<To*>(Param);
}
