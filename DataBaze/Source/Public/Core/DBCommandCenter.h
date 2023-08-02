#pragma once
#include <Windows.h>
#include "DBDataTypes.h"
#include "DBSettingsTypes.h"

class DBDataManager;
class DBWindowsManager;
class DBSettingsComp;
class DBThreadManager;

namespace cmd
{
	namespace get
	{
		HWND			  GetMainHWND();
		DBDataManager*	  GetDataManager();
		DBWindowsManager* GetWindowsManager();
		DBSettingsComp*	  GetSettingsComp();
		DBThreadManager*  GetThreadManager();

		HINSTANCE GetHIns();
	} // namespace get

	namespace wnd
	{
		void WriterEditMember(FMemberId InId, const EPeopleType People, const DBFamilyData& Data);
		void SetViewerData(FMemberId InId, const DBFamilyData& Data);
		void CloseViewer();
		void UpdateListBoxElem(FMemberId InId);

	} // namespace wnd

	namespace data
	{
		void SaveMemberCode(FMemberId InId, const std::wstring& FileName, const std::wstring& Data);
		void SetLockMember(bool InLock, FMemberId InId);
	} // namespace data

	namespace ini
	{
		int	  GetIntValue(DBIniItem Key);
		bool  GetBoolValue(DBIniItem Key);
		float GetFloatValue(DBIniItem Key);
	} // namespace ini
} // namespace cmd
