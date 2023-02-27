#include "DBInstance.h"
#include "Components/DBDataManager.h"
#include "Components/DBWindowsManager.h"
#include "Components/DBListBox.h"
#include "Components/DBButtonManager.h"

void DBInstance::Initialize(FDBInstanceInit& Param)
{
	if (Initialized) return;
	Initialized = true;
	InitData	= Param;

	//-----------------------------------------------// Components
	WindowManager = CreateComponent<DBWindowsManager>();
	DataManager	  = CreateComponent<DBDataManager>();
	ListBox		  = CreateComponent<DBListBox>();
	ButtonManager = CreateComponent<DBButtonManager>();

	assert(WindowManager);
	assert(DataManager);
	assert(ListBox);
	assert(ButtonManager);

	InitWindows(); // Create windows handlers

	//-----------------------------------------------// Components Initialization
	if (WindowManager)
	{
		WindowManager->Initialize(InitData.HIns);
	}
	if (DataManager)
	{
		DataManager->OnMemberAdded.Bind(this, &DBInstance::OnMemberAddedHandle);
	}
	if (ListBox)
	{
		const DWORD	 Style	  = WS_VISIBLE | WS_CHILD | (LBS_NOTIFY | LBS_SORT | WS_VSCROLL);
		const Size2D Position = {200, 25};
		const Size2D Size	  = {500, 300};

		ListBox->Initialize(IDC_ListBox, Style, Position, Size, InitData.MainHWND, 20);

		std::vector<std::wstring> InitList;
		DataManager->GetMembersList(InitList);
		ListBox->SetList(InitList);
	}
	if (ButtonManager)
	{
		std::vector<FButton> Buttons;

		Buttons.push_back(FButton(IDC_VIEW, {25, 25}, {150, 30}, L"Show"));
		Buttons.push_back(FButton(IDC_NEWITEM, {25, 75}, {150, 30}, L"Add New Item"));
		Buttons.push_back(FButton(IDC_LOCK, {25, 125}, {150, 30}, L"Lock"));
		Buttons.push_back(FButton(IDC_UNLOCK, {25, 175}, {150, 30}, L"Unlock"));

		Buttons.push_back(FButton(IDC_PrevFolder, {200, 320}, {45, 30}, L" < "));
		Buttons.push_back(FButton(IDC_NextFolder, {300, 320}, {45, 30}, L" > "));
		Buttons.push_back(FButton(IDC_Minimize, {655, 320}, {45, 30}, L" << "));

		ButtonManager->Initialize(InitData.MainHWND, Buttons);
	}
}

FDBInstanceInit DBInstance::GetInitData() const
{
	return InitData;
}

void DBInstance::OnMemberAddedHandle()
{
	std::wstring LastMember;
	DataManager->GetLastMemberStatus(LastMember);
	ListBox->AddItem(LastMember);
}

void DBInstance::InitWindows() {}

void DBInstance::CallCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	EDBWinCompId BtnId = static_cast<EDBWinCompId>(LOWORD(wParam));
	switch (BtnId)
	{
	case IDC_ListBox:
	{
		switch (HIWORD(wParam))
		{
		case LBN_DBLCLK:
		{
			DBFamilyData SelectedData;
			if (DataManager->LoadMember(SelectedData))
			{
				WindowManager->OpenWindowByType(EWindows::IDW_VIEWER);
				WindowManager->SetViewerData(SelectedData);
			}
			return;
		}
		case LBN_SELCHANGE:
		{
			GetDataManager()->SelectMember(GetListBox()->GetSelectedItemId());
			return;
		}
		} // switch end
	}
		// case IDC_VIEW:
		//{
		//	auto DataManager = DBSysLib::GetDataManager();
		//	if (! DataManager) return;

		//	auto WindowManager = DBSysLib::GetWindowsManager();
		//	if (! WindowManager) return;

		//	if (AnyItemSelected)
		//	{
		//		DBFamilyData SelectedData;
		//		if (! DataManager->LoadMember(SelectedData))
		//		{
		//			assert(false && "Cannot load member by index - System");
		//			return;
		//		}
		//		WindowManager->OpenWindowByType(EWindows::IDW_VIEWER);
		//		if (WindowManager->GetViewer())
		//		{
		//			WindowManager->GetViewer()->SetMemberData(SelectedData);
		//			WindowManager->EndConstruct();
		//		}
		//	}
		//	return;
		//}
		// case IDC_NEWITEM:
		//{
		//	auto WindowManager = DBSysLib::GetWindowsManager();
		//	if (! WindowManager) return;

		//	if (ListItemCount < MAX_MEMBERS_NUM)
		//	{
		//		WindowManager->OpenWindowByType(EWindows::IDW_WRITER);
		//	}
		//	return;
		//}
		// case IDC_LOCK:
		//{
		//	DBDebug::CreateMessageBox("Lock does not work");
		//	return;
		//}
		// case IDC_UNLOCK:
		//{
		//	++clicks;
		//	KillTimer(NULL, TimerId);
		//	TimerId = SetTimer(NULL, 0, 200, &DBSystem::DoubleClickTimer);

		//	if (clicks == 2)
		//	{
		//		DBDebug::CreateMessageBox("Unlock double clicked");
		//	}
		//	return;
		//}
		// case IDC_Minimize:
		//{
		//	SetMinimizedMode(! MinimizedMode);
		//	break;
		//}
		// case IDC_NextFolder:
		//{
		//	auto DataManager = DBSysLib::GetDataManager();
		//	if (! DataManager) return;

		//	if (DataManager->ChangeFolder(true))
		//	{
		//		std::wstring IdAsText = std::to_wstring(DataManager->GetFolderId());
		//		DBLib::SetText(FolderText.Window, IdAsText);
		//		InitListBox();
		//		AnyItemSelected = false;
		//	}
		//	break;
		//}
		// case IDC_PrevFolder:
		//{
		//	auto DataManager = DBSysLib::GetDataManager();
		//	if (! DataManager) return;

		//	if (DataManager->ChangeFolder(false))
		//	{
		//		std::wstring IdAsText = std::to_wstring(DataManager->GetFolderId());
		//		DBLib::SetText(FolderText.Window, IdAsText);
		//		InitListBox();
		//		AnyItemSelected = false;
		//	}
		//	break;
		//}
	} // switch end
}
