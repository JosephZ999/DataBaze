#include "DBCommandCenter.h"
#include "Core/DBInstance.h"

// Components
#include "Components/DBDataManager.h"
#include "Components/DBWindowsManager.h"

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

HINSTANCE cmd::get::GetHIns()
{
	return GetCoreIns()->GetInitData().HIns;
}

void cmd::wnd::WriterEditMember(EPeopleType People)
{
	get::GetWindowsManager()->OpenWindowByType(IDW_WRITER);
	DBFamilyData SelectedData;
	get::GetDataManager()->LoadMember(SelectedData);
	get::GetWindowsManager()->SetWriterData(SelectedData, People);
}
