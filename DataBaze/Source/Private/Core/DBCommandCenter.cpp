#include "DBCommandCenter.h"
#include "Core/DBInstance.h"
#include <assert.h>

// Components
#include "Components/DBDataManager.h"
#include "Components/DBWindowsManager.h"
#include "Components/DBListBox.h"
#include "Components/DBSettingsComp.h"
#include "Components/DBThreadManager.h"
static DBInstance* CoreInstance = nullptr;

//-----------------------------------------------// System library

DBInstance* GetCoreIns()
{
	if (! CoreInstance)
	{
		return CoreInstance = SingletonManager::Get<DBInstance>();
	}
	return CoreInstance;
}

HWND cmd::get::GetMainHWND()
{
	return GetCoreIns()->GetInitData().MainHWND;
}

DBDataManager* cmd::get::GetDataManager()
{
	return GetCoreIns()->GetDataManager();
}

DBWindowsManager* cmd::get::GetWindowsManager()
{
	return GetCoreIns()->GetWindowManager();
}

DBSettingsComp* cmd::get::GetSettingsComp()
{
	return GetCoreIns()->GetSettingsComp();
}

DBThreadManager * cmd::get::GetThreadManager()
{
	return GetCoreIns()->GetThreadManager();
}

HINSTANCE cmd::get::GetHIns()
{
	return GetCoreIns()->GetInitData().HIns;
}

void cmd::wnd::WriterEditMember(FMemberId InId, const EPeopleType People, const DBFamilyData& Data)
{
	get::GetWindowsManager()->OpenWindowByType(IDW_WRITER);
	get::GetWindowsManager()->SetWriterData(InId, Data, People);
}

void cmd::wnd::SetViewerData(FMemberId InId, const DBFamilyData& Data)
{
	// get::GetWindowsManager()->OpenWindowByType(IDW_VIEWER);
	get::GetWindowsManager()->SetViewerData(InId, Data);
}

void cmd::wnd::CloseViewer()
{
	get::GetWindowsManager()->CloseWindowByType(IDW_VIEWER);
}

void cmd::wnd::UpdateListBoxElem(FMemberId InId)
{
	std::wstring MemberStatus;
	get::GetDataManager()->GetMemberStatus(InId.MemberId, MemberStatus);
	GetCoreIns()->GetListBox()->SetItemText(InId.ListItem, MemberStatus);
}

void cmd::data::SaveMemberCode(FMemberId InId, const std::wstring& FileName, const std::wstring& Data)
{
	get::GetDataManager()->SaveMemberCode(InId, FileName, Data);
}

void cmd::data::SetLockMember(bool InLock, FMemberId InId)
{
	get::GetDataManager()->LockMember(InLock, InId);
}

int cmd::ini::GetIntValue(DBIniItem Key)
{
	if (GetCoreIns())
	{
		auto SettingsComp = GetCoreIns()->GetSettingsComp();
		if (SettingsComp)
		{
			return SettingsComp->GetIntValue(Key);
		}
	}
	assert(false);
	return 0;
}

bool cmd::ini::GetBoolValue(DBIniItem Key)
{
	if (GetCoreIns())
	{
		auto SettingsComp = GetCoreIns()->GetSettingsComp();
		if (SettingsComp)
		{
			return SettingsComp->GetBoolValue(Key);
		}
	}
	assert(false);
	return false;
}

float cmd::ini::GetFloatValue(DBIniItem Key)
{
	if (GetCoreIns())
	{
		auto SettingsComp = GetCoreIns()->GetSettingsComp();
		if (SettingsComp)
		{
			return SettingsComp->GetFloatValue(Key);
		}
	}
	assert(false);
	return 0.f;
}
