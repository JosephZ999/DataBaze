#pragma once
#include <Windows.h>
#include "DBDataTypes.h"
#include "DBSettingsTypes.h"

class DBDataManager;
class DBWindowsManager;
class DBSettingsComp;

namespace cmd
{
	namespace get
	{
		HWND			  GetMainHWND();
		DBDataManager*	  GetDataManager();
		DBWindowsManager* GetWindowsManager();
		DBSettingsComp*	  GetSettingsComp();

		HINSTANCE GetHIns();
	} // namespace get

	namespace wnd
	{
		void WriterEditMember(FMemberId InId, const EPeopleType People, const DBFamilyData& Data);
		void SetViewerData(FMemberId InId, const DBFamilyData& Data);
		void CloseViewer();
		void UpdateListBoxElem(FMemberId InId);
		void OpenNextMember();

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
		std::string GetStringValue(DBIniItem Key);
	} // namespace ini
} // namespace cmd
