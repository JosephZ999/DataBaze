#include "DBInstance.h"
#include "Components/DBDataManager.h"
#include "Components/DBWindowsManager.h"
#include "Components/DBListBox.h"
#include "Components/DBButtonManager.h"
#include "Components/UIVerticalBox.h"
#include "Components/UIHorizontalBox.h"
#include "Components/UISpacer.h"
#include "Components/DBSettingsComp.h"
#include "Components/DBThreadManager.h"
#include "Plugins/SimpleThread/Public/STManager.h"

#include "DBAutofill.h"

#include "UILibrary.h"

enum EButtonBoxId
{
	BBI_Vertical = 0,
	BBI_Horizontal,
};

void DBInstance::Initialize(FDBInstanceInit& Param)
{
	if (Initialized) return;
	Initialized = true;
	InitData	= Param;

	//-----------------------------------------------// Components
	ThreadManager = STManager::GetInstance();

	WindowManager = CreateComponent<DBWindowsManager>();
	DataManager	  = CreateComponent<DBDataManager>();
	ListBox		  = CreateComponent<DBListBox>();
	ButtonManager = CreateComponent<DBButtonManager>();
	SettingsComp  = CreateComponent<DBSettingsComp>();
	Autofill	  = CreateComponent<DBAutofill>();

	assert(WindowManager);
	assert(DataManager);
	assert(ListBox);
	assert(ButtonManager);
	assert(SettingsComp);
	assert(ThreadManager);

	InitWindows(); // Create windows handlers

	//-----------------------------------------------// Components Initialization
	if (WindowManager)
	{
		WindowManager->Initialize(InitData.HIns);
	}
	if (DataManager)
	{
		DataManager->OnMemberAdded.Bind(this, &DBInstance::OnMemberAddedHandle);
		DataManager->OnMemberChanged.Bind(this, &DBInstance::OnMemberChangedHandle);
	}
	if (ListBox)
	{
		const DWORD	 Style	  = WS_VISIBLE | WS_CHILD | (LBS_NOTIFY | LBS_SORT | WS_VSCROLL);
		const Size2D Position = {200, 25};
		const Size2D Size	  = {500, 300};

		ListBox->Initialize(IDC_ListBox, Style, Position, Size, InitData.MainHWND, 28);

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

		auto VerticalBox = UILib::CreateVerticalBox(Size2D(25, 25), Size2D(150, 200));
		for (size_t i = 0; i < Buttons.size(); ++i)
		{
			HWND NewWnd = ButtonManager->AddItem(InitData.MainHWND, Buttons[i]);

			auto Slot = UILib::CreateSlot(NewWnd, Buttons[i].Size);
			VerticalBox->AddChild(Slot);
			if (i != Buttons.size() - 1)
			{
				VerticalBox->AddChild(UILib::CreateSpacer(Size2D(0, 15)));
			}
		}
		ButtonManager->AddSlot(BBI_Vertical, VerticalBox);

		//-----------------------------------------------// Horizontal Buttons
		std::vector<FWndItem> HButtons;
		HButtons.push_back(FWndItem(WT_Button, IDC_PrevFolder, {60, 30}, L" < "));

		DWORD StaticBoxStyle(WS_BORDER | SS_CENTER);
		HButtons.push_back(FWndItem(WT_Static, IDC_FolderId, {120, 30}, L"1", 20, StaticBoxStyle));
		HButtons.push_back(FWndItem(WT_Button, IDC_NextFolder, {60, 30}, L" > "));
		HButtons.push_back(FWndItem(WT_Button, IDC_Minimize, {60, 30}, L" << "));

		auto HBox = UILib::CreateHorizontalBox(Size2D(200, 340), Size2D(500, 30));
		for (size_t i = 0; i < HButtons.size(); ++i)
		{
			HWND NewWnd = ButtonManager->AddItem(InitData.MainHWND, HButtons[i]);

			auto HSlot = UILib::CreateSlot(NewWnd, HButtons[i].Size);
			HSlot->SetFill(false);
			if (i == 3)
			{
				HSlot->SetFill(true);
				HSlot->SetHAlign(HA_Right);
			}

			HBox->AddChild(HSlot);
		}
		ButtonManager->AddSlot(BBI_Horizontal, HBox);
		UpdateFolderIdText();
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
	UpdateFolderIdText();
}

void DBInstance::OnMemberChangedHandle()
{
	ResetListBox();
}

void DBInstance::InitWindows() {}

void DBInstance::ResetListBox()
{
	std::vector<std::wstring> InitList;
	DataManager->GetMembersList(InitList);
	ListBox->SetList(InitList);
}

void DBInstance::UpdateFolderIdText()
{
	if (! GetDataManager() || ! GetListBox() || ! GetButtonManager()) return;

	std::wstring IdAsText = std::to_wstring(GetDataManager()->GetSelectedFolderId());
	IdAsText.append(L" - ").append(std::to_wstring(GetListBox()->GetLastItemId()));
	HWND FolderIdHandle = GetButtonManager()->GetWndHandler(EDBWinCompId::IDC_FolderId);
	DBLib::SetText(FolderIdHandle, IdAsText);
}

void DBInstance::SetMinimizeMode(bool Enabled)
{
	bMinimizeMode = Enabled;

	// Vertical buttons pos
	const int VBoxOffset = Enabled ? -200 : 0;
	GetButtonManager()->GetSlot(BBI_Vertical)->SetOffset(Size2D(VBoxOffset, 0));

	// Horizontal buttons pos
	UILib::SlotPtr HBox			= GetButtonManager()->GetSlot(BBI_Horizontal);
	const int	   HBoxNewSizeX = Enabled ? 480 : 500;
	const int	   HBoxSizeY	= HBox->GetSize().Y;
	HBox->SetSize(Size2D(HBoxNewSizeX, HBoxSizeY));

	const int HBoxOffset = Enabled ? -190 : 0;
	HBox->SetOffset(Size2D(HBoxOffset, 0));

	// ListBox Pos and Size
	const Size2D ListInitPos  = GetListBox()->GetInitialPos();
	const Size2D ListInitSize = GetListBox()->GetInitialSize();
	const Size2D ListPos	  = Size2D(Enabled ? ListInitPos.X - 190 : ListInitPos.X, ListInitPos.Y);
	const Size2D ListSize	  = Size2D(Enabled ? ListInitSize.X - 20 : ListInitSize.X, ListInitSize.Y);
	GetListBox()->SetPosition(ListPos);
	GetListBox()->SetSize(ListSize);

	// Window Size
	const Size2D MainWndPos(15, 25);
	const Size2D MainWndSize(Enabled ? 520 : 740, 450);
	SetWindowPos(InitData.MainHWND, HWND_TOP, MainWndPos.X, MainWndPos.Y, MainWndSize.X, MainWndSize.Y, 0);
}

void DBInstance::Destroy()
{
	RemoveAllComponents();
	PendingDestroy = CanBeDestroyed();
}

// bool DBInstance::CanBeDestroyed() const
//{
//	return true;
//}

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
			if (GetDataManager()->LoadMember(SelectedData))
			{
				GetWindowManager()->OpenWindowByType(EWindows::IDW_VIEWER);
				GetWindowManager()->SetViewerData(GetDataManager()->GetMemberId(), SelectedData);
			}
			break;
		}
		case LBN_SELCHANGE:
		{
			FMemberId NewId(GetListBox()->GetSelectedItemData(), 0, GetListBox()->GetSelectedItemId());
			GetDataManager()->SelectMember(NewId);
			break;
		}
		} // switch end
		break;
	}
	case IDC_VIEW:
	{
		DBFamilyData SelectedData;
		if (GetDataManager()->LoadMember(SelectedData))
		{
			GetWindowManager()->OpenWindowByType(EWindows::IDW_VIEWER);
			GetWindowManager()->SetViewerData(GetDataManager()->GetMemberId(), SelectedData);
		}
		break;
	}
	case IDC_NEWITEM:
	{
		if (GetListBox()->GetLastItemId() < MAX_MEMBERS_NUM)
		{
			GetWindowManager()->OpenWindowByType(EWindows::IDW_WRITER);
		}
		break;
	}
	case IDC_LOCK:
	{
		switch (HIWORD(wParam))
		{
		case BN_DBLCLK:
		{
			if (GetListBox()->GetLastItemId() > 0)
			{
				GetDataManager()->LockSelectedItem(true);

				std::wstring NewText;
				if (GetDataManager()->GetMemberStatus(GetDataManager()->GetSelectedMemberId(), NewText))
				{
					GetListBox()->SetSelectedItemText(NewText);
				}
			}
			break;
		}
		}
		break;
	}
	case IDC_UNLOCK:
	{
		switch (HIWORD(wParam))
		{
		case BN_DBLCLK:
		{
			if (GetListBox()->GetLastItemId() > 0)
			{
				GetDataManager()->LockSelectedItem(false);

				std::wstring NewText;
				if (GetDataManager()->GetMemberStatus(GetDataManager()->GetSelectedMemberId(), NewText))
				{
					GetListBox()->SetSelectedItemText(NewText);
				}
			}
			break;
		}
		}
		break;
	}
	case IDC_NextFolder:
	{
		if (GetDataManager()->ChangeFolder(true))
		{
			ResetListBox();
			UpdateFolderIdText();
		}
		break;
	}
	case IDC_PrevFolder:
	{
		if (GetDataManager()->ChangeFolder(false))
		{
			ResetListBox();
			UpdateFolderIdText();
		}
		break;
	}
	case IDC_Minimize:
	{
		SetMinimizeMode(! IsMinimized());
		break;
	}
	} // switch end
}
