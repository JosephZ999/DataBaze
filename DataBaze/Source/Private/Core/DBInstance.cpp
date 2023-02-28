#include "DBInstance.h"
#include "Components/DBDataManager.h"
#include "Components/DBWindowsManager.h"
#include "Components/DBListBox.h"
#include "Components/DBButtonManager.h"
#include "Components/UIVerticalBox.h"
#include "Components/UIHorizontalBox.h"
#include "Components/UISpacer.h"
#include "UILibrary.h"

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

		//-----------------------------------------------// Make Vertical Buttons
		std::vector<FWndItem> Buttons;
		Buttons.push_back(FWndItem(WT_Button, IDC_VIEW, L"Show"));
		Buttons.push_back(FWndItem(WT_Button, IDC_NEWITEM, L"Add New Item"));
		Buttons.push_back(FWndItem(WT_Button, IDC_LOCK, L"Lock"));
		Buttons.push_back(FWndItem(WT_Button, IDC_UNLOCK, L"Unlock"));

		UIVerticalBox* VerticalBox = UILib::CreateVerticalBox(Size2D(25, 25), Size2D(150, 200));
		for (size_t i = 0; i < Buttons.size(); ++i)
		{
			HWND   NewWnd = ButtonManager->AddItem(InitData.MainHWND, Buttons[i]);
			UISlot Slot(NewWnd, Buttons[i].Size);

			VerticalBox->AddChild(Slot);

			if (i != Buttons.size() - 1)
			{
				VerticalBox->AddChild(UISpacer(Size2D(0, 15)));
			}
		}
		ButtonManager->AddSlot(VerticalBox);

		//-----------------------------------------------// Horizontal Buttons
		std::vector<FWndItem> HButtons;
		HButtons.push_back(FWndItem(WT_Button, IDC_PrevFolder, {60, 30}, L" < "));

		DWORD StaticBoxStyle(WS_BORDER | SS_CENTER);
		HButtons.push_back(FWndItem(WT_Static, IDC_FolderId, {60, 30}, L"1", 20, StaticBoxStyle));
		HButtons.push_back(FWndItem(WT_Button, IDC_NextFolder, {60, 30}, L" > "));
		HButtons.push_back(FWndItem(WT_Button, IDC_Minimize, {60, 30}, L" << "));

		UIHorizontalBox* HBox = UILib::CreateHorizontalBox(Size2D(200, 320), Size2D(500, 30));
		for (size_t i = 0; i < HButtons.size(); ++i)
		{
			HWND NewWnd = ButtonManager->AddItem(InitData.MainHWND, HButtons[i]);

			UISlot Slot(NewWnd, HButtons[i].Size);
			Slot.SetFill(false);
			if (i == 3)
			{
				Slot.SetFill(true);
				Slot.SetHAlign(HA_Right);
			}

			HBox->AddChild(Slot);
		}
		ButtonManager->AddSlot(HBox);
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
