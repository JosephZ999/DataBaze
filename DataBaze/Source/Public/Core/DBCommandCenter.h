#pragma once
#include <Windows.h>

class DBDataManager;
class DBWindowsManager;

namespace cmd
{
	namespace get
	{
		HWND			  GetMainHWND();
		DBDataManager*	  GetDataManager();
		DBWindowsManager* GetWindowsManager();
		HINSTANCE		  GetHIns();
	} // namespace DBSysLib
} // namespace cmd
