#pragma once
#include <Windows.h>
#include "DBDataTypes.h"

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
	} // namespace get

	namespace wnd
	{
		void WriterEditMember(int FamilyId, const EPeopleType People);
	}
} // namespace cmd
