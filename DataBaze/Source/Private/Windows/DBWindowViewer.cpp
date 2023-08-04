#include "DBWindowViewer.h"
#include "DBKeyCodes.h"

#include "Components/DBButtonManager.h"
#include "Components/UIVerticalBox.h"
#include "Components/UIHorizontalBox.h"
#include "UILibrary.h"
#include "DBAutofill.h"

#include <map>

static std::map<char, WORD> VKeys = {
	//
	{'Q', VK_Q}, {'W', VK_W}, {'E', VK_E}, {'R', VK_R}, {'T', VK_T}, {'Y', VK_Y}, //
	{'U', VK_U}, {'I', VK_I}, {'O', VK_O}, {'P', VK_P},							  //

	{'A', VK_A}, {'S', VK_S}, {'D', VK_D}, {'F', VK_F}, {'G', VK_G}, {'H', VK_H}, //
	{'J', VK_J}, {'K', VK_K}, {'L', VK_L},										  //

	{'Z', VK_Z}, {'X', VK_X}, {'C', VK_C}, {'V', VK_V}, //
	{'B', VK_B}, {'N', VK_N}, {'M', VK_M},				//

	{'.', VK_OEM_PERIOD}, {'1', VK_1}, {'2', VK_2}, {'3', VK_3}, {'4', VK_4}, {'5', VK_5}, //
	{'6', VK_6}, {'7', VK_7}, {'8', VK_8}, {'9', VK_9}, {'0', VK_0}						   //

};

LRESULT DBWindowViewer::CallProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATE:
	{
		// Window activation/deactivation handling
		if (LOWORD(wParam) != WA_INACTIVE)
		{
			RegisterHotKey(hWnd, HKV_Close, 0, VK_F4);
		}
		else
		{
			UnregisterHotKey(hWnd, HKV_Close);
		}
		break;
	}
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			RegisterHotKey(hWnd, HKV_Command_1, MOD_CONTROL, VK_NUMPAD1);
			RegisterHotKey(hWnd, HKV_Command_2, MOD_CONTROL, VK_NUMPAD2);
			RegisterHotKey(hWnd, HKV_Command_3, MOD_CONTROL, VK_NUMPAD3);
			RegisterHotKey(hWnd, HKV_Next, MOD_CONTROL, VK_SPACE);
			RegisterHotKey(hWnd, HKV_Reset, MOD_CONTROL, VK_NUMPAD0);
		}
		break;
	}
	case WM_CLOSE:
	{
		UnregisterHotKey(hWnd, HKV_Command_1);
		UnregisterHotKey(hWnd, HKV_Command_2);
		UnregisterHotKey(hWnd, HKV_Command_3);
		UnregisterHotKey(hWnd, HKV_Next);
		UnregisterHotKey(hWnd, HKV_Reset);
		UnregisterHotKey(hWnd, HKV_Close);

		OnClose.Broadcast();
		return 0;
	}

	case WM_HOTKEY:
	{
		switch (wParam)
		{
		case HKV_Command_1:
		{
			if (AutoFillObj)
			{
				AutoFillObj->StartFilling(EAutofillStep::Page1);
			}
			break;
		}
		case HKV_Command_2:
		{
			if (AutoFillObj)
			{
				AutoFillObj->StartFilling(EAutofillStep::Page2);
			}
			break;
		}
		case HKV_Command_3:
		{
			if (AutoFillObj)
			{
				AutoFillObj->StartFilling(EAutofillStep::SaveResult);
			}
			break;
		}
		case HKV_Next:
		{
			if (AutoFillObj)
			{
				AutoFillObj->StartFilling();
			}
			break;
		}
		case HKV_Reset:
		{
			if (AutoFillObj)
			{
				AutoFillObj->StartFilling(EAutofillStep::Max);
			}
			break;
		}
		case HKV_Close:
		{
			// SendMessage(hWnd, WM_CLOSE, 0, 0);
			OnClose.Broadcast();
			UnregisterHotKey(hWnd, HKV_Close);
			break;
		}
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC			hdc = BeginPaint(hWnd, &ps);

		Size2D WindowSize = DBLib::GetScreenSize(hWnd);
		RECT   Rect		  = {0, 0, WindowSize.X, WindowSize.Y};
		HBRUSH BrushColor = CreateSolidBrush(RGB(200, 220, 200));
		FillRect(hdc, &Rect, BrushColor);

		EndPaint(hWnd, &ps);
		break;
	}
	}

	EDBWinCompId BtnId = static_cast<EDBWinCompId>(LOWORD(wParam));
	switch (BtnId)
	{
	case EDBWinCompId::IDC_V_Edit:
	{
		cmd::wnd::WriterEditMember(MemberId, CurrentPeople, MemberData);
		break;
	}
	case EDBWinCompId::IDC_V_Prev:
	{
		ChangePeople(false);
		break;
	}
	case EDBWinCompId::IDC_V_Next:
	{
		ChangePeople(true);
		break;
	}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

DBWindowViewer::DBWindowViewer(HWND OwningWnd)
{
	WindowHandle = OwningWnd;

	AutoFillObj = std::make_shared<DBAutofill>();

	assert(AutoFillObj);

	ButtonManager = CreateComponent<DBButtonManager>();
	if (ButtonManager)
	{
		auto VBox = UILib::CreateVerticalBox(Size2D(25, 25), Size2D(535, 380));

		// Info boxes
		{
			auto HBox1 = UILib::CreateHorizontalBox(Size2D(), Size2D());
			HBox1->SetFill(true);

			FWndItem SlotElem1(WT_Static, IDC_V_InfoTitle, Size2D(200, 300), L"", 18, SS_RIGHT);
			FWndItem SlotElem2(WT_Edit, IDC_V_Info, Size2D(335, 300), L"", 18, ES_MULTILINE | ES_READONLY);
			HWND	 SlotWnd1 = ButtonManager->AddItem(WindowHandle, SlotElem1);
			HWND	 SlotWnd2 = ButtonManager->AddItem(WindowHandle, SlotElem2);

			auto Slot1 = UILib::CreateSlot(SlotWnd1, SlotElem1.Size);
			Slot1->SetHAlign(HA_Left);
			Slot1->SetFill(false);

			auto Slot2 = UILib::CreateSlot(SlotWnd2, SlotElem2.Size);

			HBox1->AddChild(Slot1);
			// HBox1->AddChild(UILib::CreateSpacer(Size2D(10, 0)));
			HBox1->AddChild(Slot2);
			VBox->AddChild(HBox1);
		}

		// Buttons
		{
			auto HBox2 = UILib::CreateHorizontalBox(Size2D(), Size2D(0, 35));
			HBox2->SetFill(false);
			HBox2->SetVAlign(VA_Bottom);

			FWndItem SlotElem1(WT_Button, IDC_V_Prev, Size2D(100, 35), L"< Prev");
			FWndItem SlotElem2(WT_Button, IDC_V_Edit, Size2D(100, 35), L"Edit");
			FWndItem SlotElem3(WT_Button, IDC_V_Next, Size2D(100, 35), L"Next >");
			HWND	 SlotWnd1 = ButtonManager->AddItem(WindowHandle, SlotElem1);
			HWND	 SlotWnd2 = ButtonManager->AddItem(WindowHandle, SlotElem2);
			HWND	 SlotWnd3 = ButtonManager->AddItem(WindowHandle, SlotElem3);

			auto Slot1 = UILib::CreateSlot(SlotWnd1, SlotElem1.Size);
			Slot1->SetVAlign(VA_Bottom);
			Slot1->SetHAlign(HA_Left);

			auto Slot2 = UILib::CreateSlot(SlotWnd2, SlotElem2.Size);
			Slot2->SetVAlign(VA_Bottom);
			Slot2->SetHAlign(HA_Center);

			auto Slot3 = UILib::CreateSlot(SlotWnd3, SlotElem3.Size);
			Slot3->SetVAlign(VA_Bottom);
			Slot3->SetHAlign(HA_Right);

			HBox2->AddChild(Slot1);
			HBox2->AddChild(Slot2);
			HBox2->AddChild(Slot3);
			VBox->AddChild(UILib::CreateSpacer(Size2D(15, 15)));
			VBox->AddChild(HBox2);
		}

		ButtonManager->AddSlot(0, VBox);
	}
}

void DBWindowViewer::SetMemberData(FMemberId InId, const DBFamilyData& InData)
{
	MemberId   = InId;
	MemberData = InData;
	ListItemId = InId.ListItem;
	PrintData();
	InitializeSteps();

	if (AutoFillObj)
	{
		AutoFillObj->Init(InData, InId, WindowHandle);
	}
}

void DBWindowViewer::InitializeSteps()
{
	StepMap.clear();
	CurrentStep = EAutoFillStep::AFS_Part1;

	StepMap.push_back(AFS_Part1);

	if (MemberData.IsHasASpouse() || MemberData.IsHasChildren())
	{
		StepMap.push_back(AFS_Part2);
		SecondFormAvailable = true;
	}
	else
	{
		SecondFormAvailable = false;
	}
	// StepMap.push_back(AFS_Check);
	StepMap.push_back(AFS_SaveResult);
}

void DBWindowViewer::AutoFill()
{
	if (StepMap.size() == 0 || MemberData.bLocked || FillingInProgress) return;

	CurrentStep = StepMap[0];
	StepMap.erase(StepMap.begin());

	FillingInProgress = true;
	switch (CurrentStep)
	{
	case AFS_Part1: Autofill_Form1(); break;
	case AFS_Part2: Autofill_Form2(); break;
	case AFS_Check: Autofill_Check(); break;
	case AFS_SaveResult: Autofill_SaveResult(); break;
	}
	Sleep(1000);
	FillingInProgress = false;
}

void DBWindowViewer::Autofill_Form1()
{
	if (MemberData.Parents.size() == 0 || MemberData.bLocked) return;

	FillPeopleData(MemberData.Parents[0], true);

	PasteMailInfo();
	SelectEducationDegree();
	SelectMeritialStatus();
	PasteString(std::to_string(MemberData.ChildrenNum));
	PressTab(1);
	DBInput::PressKey(VK_SPACE);
}

void DBWindowViewer::Autofill_Form2()
{
	if (MemberData.bLocked || ! SecondFormAvailable) return;

	if (MemberData.IsHasASpouse())
	{
		FillPeopleData(MemberData.Parents[1], false);
	}

	if (MemberData.IsHasChildren())
	{
		for (auto& Child : MemberData.Children)
		{
			FillPeopleData(Child, false);
		}
	}
	DBInput::PressKey(VK_SPACE);
}

void DBWindowViewer::Autofill_Check()
{
	if (MemberData.bLocked) return;

	DBInput::PressKey(VK_END);
}

void DBWindowViewer::Autofill_SaveResult()
{
	if (MemberData.bLocked) return;

	CopyAndSaveCode();
	cmd::data::SetLockMember(true, MemberId);
	cmd::wnd::UpdateListBoxElem(MemberId);
	cmd::wnd::CloseViewer();
}

void DBWindowViewer::FillPeopleData(const DBPeopleData& InPeople, bool bPartOne)
{
	PasteString(InPeople.FamilyName);
	PressTab(1);

	PasteString(InPeople.Name);
	PressTab(3);
	DBInput::PressKey(VK_SPACE);
	PressTab(1);

	// Gender
	if (InPeople.IsMele())
	{
		DBInput::PressKey(VK_SPACE);
		PressTab(2);
	}
	else
	{
		PressTab(1);
		DBInput::PressKey(VK_SPACE);
		PressTab(1);
	}

	// Birth day
	PasteString(std::to_string(InPeople.BirthMonth));
	PressTab(1);

	PasteString(std::to_string(InPeople.BirthDay));
	PressTab(1);

	PasteString(std::to_string(InPeople.BirthYear));
	PressTab(1);

	// Fill City
	if (InPeople.IsBirthCityValid())
	{
		PasteString(InPeople.BirthCity);
		PressTab(2);
	}
	else
	{
		PressTab(1);
		DBInput::PressKey(VK_SPACE);
		PressTab(1);
	}

	// country
	WriteString(InPeople.BirthCountry);
	PressTab(1);

	if (bPartOne)
	{
		PressTab(4);
	}

	// Image
	PressTab(3);
	DBInput::PressKey(VK_SPACE); // Open Image Dialog Window

	Sleep(1000);
	PasteImagePath(InPeople);
	DBInput::PressKey(VK_RETURN);
	Sleep(1000);

	PressTab(1);
}

void DBWindowViewer::ChangePeople(bool Next)
{
	const bool HasSpouse = MemberData.Parents.size() > 1;
	const bool HasChild	 = MemberData.Children.size() > 0;
	if (													   //
		(! Next && CurrentPeople == EPeopleType::PT_Parent) || //
		(Next && ! HasSpouse && ! HasChild)					   //
	)
	{
		return;
	}

	bool		bChanged = false;
	EPeopleType NextP	 = (EPeopleType)((int)CurrentPeople + (Next ? 1 : -1));
	switch (NextP)
	{
	case PT_Parent:
	{
		CurrentPeople = NextP;
		bChanged	  = true;
		break;
	}
	case PT_Spouse:
	{
		if (HasSpouse)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		else if (HasChild)
		{
			CurrentPeople = Next ? PT_Child_1 : PT_Parent;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_1:
	{
		if (MemberData.Children.size() >= 1)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_2:
	{
		if (MemberData.Children.size() >= 2)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_3:
	{
		if (MemberData.Children.size() >= 3)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_4:
	{
		if (MemberData.Children.size() >= 4)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_5:
	{
		if (MemberData.Children.size() >= 5)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_6:
	{
		if (MemberData.Children.size() >= 6)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_7:
	{
		if (MemberData.Children.size() >= 7)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	}

	if (bChanged)
	{
		PrintData();
		InitializeSteps();
	}
}

void DBWindowViewer::PrintData()
{
	if (MemberData.Parents.size() == 0) return;

	// Clear text
	DBLib::SetText(ButtonManager->GetWndHandler(IDC_V_InfoTitle), L"");
	DBLib::SetText(ButtonManager->GetWndHandler(IDC_V_Info), L"");
	Title.clear();
	Info.clear();

	HasSpouse = MemberData.Parents.size() > 1;
	HasChild  = MemberData.Children.size() > 0;

	PrintPeople(MemberData[CurrentPeople], CurrentPeople > EPeopleType::PT_Spouse);

	if (CurrentPeople == EPeopleType::PT_Parent)
	{
		PrintMail();
	}

	// convert to wstr
	std::wstring InfoAsWStr;
	DBConvert::StringToWString(Info, InfoAsWStr);

	DBLib::SetText(ButtonManager->GetWndHandler(IDC_V_InfoTitle), Title);
	DBLib::SetText(ButtonManager->GetWndHandler(IDC_V_Info), InfoAsWStr);
}

void DBWindowViewer::PrintPeople(const DBPeopleData& People, bool IsChild)
{
	std::vector<FillData> DataArray;

	DataArray.push_back({L"Name : ", People.Name});
	DataArray.push_back({L"Family Name : ", People.FamilyName});
	DataArray.push_back({L"Birth Data (d/m/y) : ", People.GetBirthDataAsString()});
	DataArray.push_back({L"Gender : ", People.GetGenderAsString()});
	DataArray.push_back({L"Birth City : ", People.BirthCity});
	DataArray.push_back({L"Birth Country : ", People.BirthCountry});

	if (! IsChild)
	{
		DataArray.push_back({L"Ñountry Wher Live : ", People.WhereLive});
		DataArray.push_back({L"Education : ", People.GetEducationAsString()});
	}
	DataArray.push_back({L"Image Path : ", std::string(People.ImageFile).erase(0, 6)});

	for (auto Elem : DataArray)
	{
		Title.append(Elem.Title).append(L"\n");
		Info.append(Elem.Info).append("\r\n");
	}
}

void DBWindowViewer::PrintMail()
{
	std::vector<FillData> DataArray;

	DataArray.push_back({L"Mail Country : ", MemberData.MailCountry});
	DataArray.push_back({L"Mail Region : ", MemberData.MailRegion});
	DataArray.push_back({L"Mail City : ", MemberData.MailCity});
	DataArray.push_back({L"Mail Street : ", MemberData.MailStreet});
	DataArray.push_back({L"Mail Home : ", MemberData.MailHomeNumber});
	DataArray.push_back({L"Mail Code : ", std::to_string(MemberData.MailZipCode)});

	for (auto Elem : DataArray)
	{
		Title.append(Elem.Title).append(L"\n");
		Info.append(Elem.Info).append("\r\n");
	}
}

void DBWindowViewer::PressTab(size_t Times)
{
	if (Times == 0) return;

	for (size_t i = 0; i < Times; ++i)
	{
		DBInput::PressKey(VK_TAB);
	}
}

void DBWindowViewer::CopyAndSaveCode()
{
	if (OpenClipboard(NULL))
	{
		HANDLE		 clip = GetClipboardData(CF_UNICODETEXT);
		std::wstring Code = std::wstring((wchar_t*)clip);
		CloseClipboard();

		cmd::data::SaveMemberCode(MemberId, //
			GenerateFileName(),				//
			Code);
	}
}

inline void DBWindowViewer::PasteString(const std::string Text)
{
	Sleep(20);
	DBInput::CopyToClipboard(WindowHandle, Text);
	Sleep(20);
	DBInput::PasteClipboard();
	Sleep(20);
}

inline void DBWindowViewer::WriteEMail()
{
	WriteString("DEVILNOISY999");
	DBInput::PressKeys(VK_SHIFT, VK_2);
	WriteString("GMAIL.COM");
}

void DBWindowViewer::PasteImagePath(const DBPeopleData& People)
{
	Sleep(20);
	const std::string SImage = People.ImageFile;

	std::wstring WImage;
	DBConvert::StringToWString(SImage, WImage);

	std::wstring FilePath = DBPaths::GetDataFolderPath(MemberId.FolderId).append(WImage);
	DBInput::CopyToClipboard(0, FilePath);

	DBInput::PressKeys(VK_CONTROL, VK_V);
}

void DBWindowViewer::WriteString(std::string Text)
{
	for (std::string::iterator it = Text.begin(); it != Text.end(); ++it)
	{
		if (VKeys.find(*it) != VKeys.end())
		{
			DBInput::PressKey(VKeys[*it]);
		}
		else
		{
			DBDebug::CreateMessageBox("Viewer: Error Writing Country");
			break;
		}
	}
}

void DBWindowViewer::PasteMailInfo()
{
	PressTab(1);
	PasteString(MemberData.MailStreet);
	PressTab(1);

	if (MemberData.MailHomeNumber.size() > 0 && DBConvert::StringToInt(MemberData.MailHomeNumber) != 0)
	{
		PasteString(MemberData.MailHomeNumber);
	}

	PressTab(1);
	PasteString(MemberData.MailCity);
	PressTab(1);
	PasteString(MemberData.MailRegion);
	PressTab(1);

	if (MemberData.IsHasZipCode())
	{
		PasteString(std::to_string(MemberData.MailZipCode));
		PressTab(2);
	}
	else
	{
		PressTab(1);
		DBInput::PressKey(VK_SPACE);
		PressTab(1);
	}

	WriteString(MemberData.MailCountry);
	PressTab(1);

	WriteString(MemberData.Parents[0].WhereLive);
	PressTab(2);

	WriteEMail();
	PressTab(1);
	WriteEMail();
	PressTab(1);
}

void DBWindowViewer::SelectEducationDegree()
{
	const int Max = 10;
	for (int i = 0; i < Max; ++i)
	{
		if (i == MemberData.Parents[0].EducationDegree - 1)
		{
			DBInput::PressKey(VK_SPACE);
		}
		PressTab(1);
	}
}

void DBWindowViewer::SelectMeritialStatus()
{
	const int Max = 6;
	for (int i = 0; i < Max; ++i)
	{
		if (i == MemberData.MaritalStatus - 1)
		{
			DBInput::PressKey(VK_SPACE);
		}
		PressTab(1);
	}
}

std::wstring DBWindowViewer::GenerateFileName()
{
	const std::string MemberName  = MemberData.Parents[0].Name;
	const std::string MemberFName = MemberData.Parents[0].FamilyName;
	const std::string StringName =
		std::string("\\").append(std::to_string(MemberId.MemberId)).append(" - ").append(MemberFName).append(" ").append(MemberName);

	std::wstring WStr;
	DBConvert::StringToWString(StringName, WStr);
	return WStr;
}
