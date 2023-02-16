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

void CopyToClipboard(HWND hwnd, const std::string& String);
void PressKey(WORD Key);
void PressKeys(WORD Key1, WORD Key2);
void PasteClipboard();
Size2D GetScreenSize();
Size2D GetScreenSize(HWND Window);

} // namespace DBLib

namespace DBPaths
{
std::wstring GetProjectPath();
}

namespace DBConvert
{
void StringToWString(const std::string& String, std::wstring& OutWString);
void WStringToString(const std::wstring& WString, std::string& OutString);
int StringToInt(const std::string& InString);
} // namespace DBConvert

template <typename To, typename From> //
To* Cast(From* Param)
{
	return dynamic_cast<To*>(Param);
}
