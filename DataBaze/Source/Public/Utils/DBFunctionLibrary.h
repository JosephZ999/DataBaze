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
void CreateAWindow();
HWND CreateButton(DBWindow& ButtonInfo);
void SetFontSize(HWND Window, int FontSize);
void CreateText(HWND hWnd);
HWND CreateEditBox(DBWindow& ButtonInfo, DWORD Style);

void CopyToClipboard(HWND hwnd, const std::string& s);
void PressKey(WORD Key);
void PressKeys(WORD Key1, WORD Key2);
void PasteClipboard();

} // namespace DBLib
