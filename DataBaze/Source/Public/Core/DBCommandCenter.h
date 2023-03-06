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
		void WriterEditMember(int FamilyId, int FolderId, const EPeopleType People, const DBFamilyData& Data);
		void SetViewerData(int FamilyId, int FolderId, const DBFamilyData& Data);
	} // namespace wnd

	namespace data
	{
		void SaveMemberCode(int MemberId, int FolderId, const std::wstring& FileName, const std::wstring& Data);
	}
} // namespace cmd
