#pragma once

#include <string>

namespace JsonUtils
{

std::wstring& AddItem(std::wstring& InData, std::wstring ParamName, std::wstring ParamValue);
std::wstring& AddItem(std::wstring& InData, std::wstring ParamName, int ParamValue);

std::wstring& OpenList(std::wstring& InData, std::wstring ParamName);
std::wstring& CloseList(std::wstring& InData);
std::wstring& OpenArray(std::wstring& InData, std::wstring ParamName);
std::wstring& CloseArray(std::wstring& InData);

} // namespace JsonUtils
