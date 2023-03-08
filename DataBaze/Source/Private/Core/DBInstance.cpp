#include "DBInstance.h"
#include "Components/DBDataManager.h"
#include "Components/DBWindowsManager.h"
#include "Components/DBListBox.h"
#include "Components/DBButtonManager.h"
#include "Components/UIVerticalBox.h"
#include "Components/UIHorizontalBox.h"
#include "Components/UISpacer.h"
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
		DataManager->OnMemberChanged.Bind(this, &DBInstance::OnMemberChangedHandle);
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

		auto HBox = UILib::CreateHorizontalBox(Size2D(200, 320), Size2D(500, 30));
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
	std::wstring IdAsText = std::to_wstring(GetDataManager()->GetFolderId());
	IdAsText.append(L" - ").append(std::to_wstring(GetListBox()->GetLastItemId()));
	HWND FolderIdHandle = GetButtonManager()->GetWndHandler(EDBWinCompId::IDC_FolderId);
	DBLib::SetText(FolderIdHandle, IdAsText);
}

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
				GetWindowManager()->SetViewerData(GetDataManager()->GetSelectedMemberId(), GetDataManager()->GetFolderId(), SelectedData);
			}
			break;
		}
		case LBN_SELCHANGE:
		{
			GetDataManager()->SelectMember(GetListBox()->GetSelectedItemId());
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
			GetWindowManager()->SetViewerData(GetDataManager()->GetSelectedMemberId(), GetDataManager()->GetFolderId(), SelectedData);
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
			DBDebug::CreateMessageBox("Lock does not work");
			break;
		}
		case BN_CLICKED:
		{
			// DBDebug::CreateMessageBox("Lock does not work");
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
			DBDebug::CreateMessageBox("Lock does not work");
			break;
		}
		case BN_CLICKED:
		{
			// DBDebug::CreateMessageBox("Lock does not work");
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
		// case IDC_Minimize:
		//{
		//	SetMinimizedMode(! MinimizedMode);
		//	break;
		//}
	} // switch end
}
