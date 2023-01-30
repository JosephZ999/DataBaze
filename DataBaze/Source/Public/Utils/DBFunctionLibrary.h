#pragma once

#include "framework.h"
#include "Resource.h"
#include <string>
#include <vector>
#include "DBDataTypes.h"

namespace DBLib
{

void SetWindowVisibility(HWND Window, bool bShow);
HWND CreateButton(DBButton& ButtonInfo);
void SetFontSize(HWND Window, int FontSize);
void CreateText();
HWND CreateComboBox(DBButton& ButtonInfo);

void ToClipboard(HWND hwnd, const std::string& s);

} // namespace DBLib
