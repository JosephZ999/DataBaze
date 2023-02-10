#include "DBJsonUtils.h"

using namespace std;

wstring& JsonUtils::AddItem(wstring& InData, wstring ParamName, wstring ParamValue)
{
	InData.append(L"\"").append(ParamName).append(L"\" : ");
	InData.append(L"\"").append(ParamValue).append(L"\"");
	return InData;
}

wstring& JsonUtils::AddItem(wstring& InData, wstring ParamName, int ParamValue)
{
	InData.append(L"\"").append(ParamName).append(L"\" : ");
	InData.append(to_wstring(ParamValue));
	return InData;
}

std::wstring& JsonUtils::OpenList(std::wstring& InData, std::wstring ParamName)
{
	return InData.append(L"{\"").append(ParamName).append(L"\": {");
}

std::wstring& JsonUtils::CloseList(std::wstring& InData)
{
	return InData.append(L"}}");
}

std::wstring& JsonUtils::OpenArray(std::wstring& InData, std::wstring ParamName)
{
	return InData.append(L"{\"").append(ParamName).append(L"\": [");
}

std::wstring& JsonUtils::CloseArray(std::wstring& InData)
{
	return InData.append(L"]}");
}
