
#include "DBWindowWriter.h"
#include <commdlg.h>
#include <comdef.h>
#include "DBDataManager.h"
#include "DBButtonManager.h"

#include "UILibrary.h"

#define EDIT_STYLE_BASE WS_BORDER | WS_VISIBLE | WS_CHILD | ES_CENTER | ES_UPPERCASE | ES_WANTRETURN

LRESULT DBWindowWriter::CallProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			SetFocus(ButtonManager->GetWndHandler(IDC_W_Edit));
			SelectWriteData(PeopleType);
			break;
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hWnd, &ps);

		Size2D WindowSize  = DBLib::GetScreenSize(hWnd);
		RECT   Rect		   = {0, 0, WindowSize.X, WindowSize.Y};
		RECT   Rect2	   = {15, 15, 565, 310};
		HBRUSH BrushColor  = CreateSolidBrush(RGB(200, 200, 220));
		HBRUSH BrushColor2 = CreateSolidBrush(RGB(240, 240, 240));

		FillRect(hdc, &Rect, BrushColor);
		FillRect(hdc, &Rect2, BrushColor2);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_CLOSE:
	{
		OnClose.Broadcast();
		return 0;
	}
	case WM_HOTKEY:
	{
		if (CheckFormat())
		{
			WriteData();
			SelectWriteData(PeopleType);
		}
		if (! bEditMode)
		{
			SendMessage(ButtonManager->GetWndHandler(IDC_W_Edit), WM_SETTEXT, 0, (LPARAM)L"");
			break;
		}
		UpdateEditText();
		break;
	}
	case WM_DRAWITEM:
	{
		/*
		HDC	   hdc		  = GetDC(WriterInfoBox.Window);
		RECT   Rect		  = {0, 0, 520, 120};
		HBRUSH BrushColor = CreateSolidBrush(RGB(250, 0, 0));

		FillRect(hdc, &Rect, BrushColor);
		SetTextColor(hdc, RGB(0, 0, 250));
		*/
		break;
	}
	} // switch end

	// Edit control options
	switch (LOWORD(wParam))
	{
	case IDC_W_Edit:
	{
		if (HIWORD(wParam) == EN_SETFOCUS)
		{
			RegisterHotKey(hWnd, HKW_Enter, 0, VK_RETURN);
		}
		if (HIWORD(wParam) == EN_KILLFOCUS)
		{
			UnregisterHotKey(hWnd, HKW_Enter);
		}
		break;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

DBWindowWriter::DBWindowWriter(HWND OwningWnd)
{
	WindowHandle = OwningWnd;

	auto DataManager = cmd::get::GetDataManager();
	if (! DataManager) return;

	LastImageId = DataManager->ReadImageId();

	ButtonManager = CreateComponent<DBButtonManager>();
	if (ButtonManager)
	{
		auto VBox = UILib::CreateVerticalBox(Size2D(25, 25), Size2D(530, 275));

		FWndItem Item1(WT_Static, IDC_W_Info, Size2D(0, 100), L"", 24, 0);
		FWndItem Item2(WT_Edit, IDC_W_Edit, Size2D(100, 40), L"", 28, ES_CENTER | ES_UPPERCASE | ES_WANTRETURN);
		FWndItem Item3(WT_Static, IDC_W_MoreInfo, Size2D(100, 100), L"", 18, 0);

		HWND Wnd1 = ButtonManager->AddItem(WindowHandle, Item1);
		HWND Wnd2 = ButtonManager->AddItem(WindowHandle, Item2);
		HWND Wnd3 = ButtonManager->AddItem(WindowHandle, Item3);

		auto Slot1 = UILib::CreateSlot(Wnd1, Item1.Size);
		Slot1->SetVAlign(VA_Top);
		Slot1->SetFill(false);
		auto Slot2 = UILib::CreateSlot(Wnd2, Item2.Size);
		Slot2->SetVAlign(VA_Top);
		Slot2->SetFill(false);
		auto Slot3 = UILib::CreateSlot(Wnd3, Item3.Size);

		VBox->AddChild(Slot1);
		VBox->AddChild(Slot2);
		VBox->AddChild(UILib::CreateSpacer(Size2D(0, 20)));
		VBox->AddChild(Slot3);

		ButtonManager->AddSlot(VBox);
	}
}

void DBWindowWriter::SelectWriteData(EPeopleType PT)
{
	if (bFinish) return;

	switch (PT)
	{
	case PT_Parent:
	{
		if (MembersData.Parents.size() < 1)
		{
			MembersData.Parents.push_back(DBPeopleData());
		}
		DataToChange = &MembersData.Parents[0];
		break;
	}
	case PT_Spouse:
	{
		if (Status != EMeritialStatus::MS_Married)
		{
			NextPeople();
			break;
		}

		if (MembersData.Parents.size() < 2)
		{
			MembersData.Parents.push_back(DBPeopleData());
		}
		DataToChange = &MembersData.Parents[1];
		break;
	}
	case PT_Child_1: SelectChild(1); break;
	case PT_Child_2: SelectChild(2); break;
	case PT_Child_3: SelectChild(3); break;
	case PT_Child_4: SelectChild(4); break;
	case PT_Child_5: SelectChild(5); break;
	case PT_Child_6: SelectChild(6); break;
	case PT_Child_7: SelectChild(7); break;
	case PT_Child_8: SelectChild(8); break;
	case PT_Child_9: SelectChild(9); break;
	}
	UpdateInfo();
	UpdateEditStyle();
	UpdateEditText();
}

void DBWindowWriter::EditPeople(int FamilyId, int FolderId, const DBFamilyData& Data, EPeopleType People)
{
	CurrentFamilyId = FamilyId;
	CurrentFolderId = FolderId;
	bEditMode		= true;
	PeopleType		= People;
	MembersData		= Data;
	UpdateInfo();
	UpdateEditStyle();
	UpdateEditText();
}

void DBWindowWriter::WriteData()
{
	if (! DataToChange) return;

	TCHAR buff[256];
	GetWindowText(ButtonManager->GetWndHandler(IDC_W_Edit), buff, 256);

	std::string OutValue;
	DBConvert::WStringToString(buff, OutValue);
	SetItem(OutValue);

	NextLine();
	switch (PeopleType)
	{
	case PT_Parent:
	{
		switch (PeopleData)
		{
		case PD_NotChildInfo: NextLine(); break;
		case PD_OnlyParentInfo: NextLine(); break;
		case PD_Max:
		{
			FinishWriting();
			NextPeople();
			break;
		}
		}
		break;
	}
	case PT_Spouse:
	{
		switch (PeopleData)
		{
		case PD_NotChildInfo: NextLine(); break;
		case PD_OnlyParentInfo:
		{
			FinishWriting();
			NextPeople();
			break;
		}
		} // switch
		break;
	}
	} // switch

	const bool IsParent = PeopleType == EPeopleType::PT_Parent;
	const bool IsSpouse = PeopleType == EPeopleType::PT_Spouse;
	const bool IsChild	= ! (IsParent || IsSpouse);

	if (PeopleData == PD_ImageFile)
	{
		if (bEditMode)
		{
			NextLine();
			NextLine();
			return;
		}
		OpenImage();
		if (IsParent || IsSpouse)
		{
			NextLine();
			NextLine();
		}
		else
		{
			++EnteredChildrenNum;
			FinishWriting();
			NextPeople();
		}
	}
}

void DBWindowWriter::UpdateInfo()
{
	std::wstring InfoText;
	std::wstring MoreInfoText;

	InfoText.append(L"<   ");
	switch (PeopleType)
	{ // clang-format off
	case PT_Parent:		InfoText.append(L"Parent 1");	break;
	case PT_Spouse:		InfoText.append(L"Parent 2");	break;
	case PT_Child_1:	InfoText.append(L"Child 1");	break;
	case PT_Child_2:	InfoText.append(L"Child 2");	break;
	case PT_Child_3:	InfoText.append(L"Child 3");	break;
	case PT_Child_4:	InfoText.append(L"Child 4");	break;
	case PT_Child_5:	InfoText.append(L"Child 5");	break;
	case PT_Child_6:	InfoText.append(L"Child 6");	break;
	case PT_Child_7:	InfoText.append(L"Child 7");	break;
	case PT_Child_8:	InfoText.append(L"Child 8");	break;
	case PT_Child_9:	InfoText.append(L"Child 9");	break;
	} // clang-format on
	InfoText.append(L"   >\n");

	switch (PeopleData)
	{
	case PD_Name:
	{
		InfoText.append(L"Name");
		break;
	}
	case PD_FamilyName:
	{
		InfoText.append(L"Family Name");
		break;
	}
	case PD_Gender:
	{
		InfoText.append(L"Gender");
		MoreInfoText.append(L"1 or M - Mele\n2 or F - Famele");
		break;
	}
	case PD_BirthMonth:
	{
		InfoText.append(L"Birth Month");
		break;
	}
	case PD_BirthDay:
	{
		InfoText.append(L"Birth Day");
		break;
	}
	case PD_BirthYear:
	{
		InfoText.append(L"Birth Year");
		break;
	}
	case PD_BornCountry:
	{
		InfoText.append(L"Country where Born");
		MoreInfoText.append(L"Example - UZB");
		break;
	}
	case PD_EducationDegree:
	{
		InfoText.append(L"Education");
		MoreInfoText.append(
			L"H.School no degree - 1 or N, H.School - 2 or S, Vocational - 3 or V, University Courses - 4 or UC, "
			L"University - 5 or U, Graduate level Courses - 6 or GC, Masters - 7 or M, Doctoral Courses - 8 or SD, Doctorate - 9 or D");
		break;
	}
	case PD_WhereLive:
	{
		InfoText.append(L"Country Where Live Today");
		MoreInfoText.append(L"Example - RUS");
		break;
	}
	case PD_MaritalStatus:
	{
		InfoText.append(L"Marital Status");
		MoreInfoText.append(L"Unmarried - 1 or U, Married - 2 or M, Spouse is a US citizen - 3 or US, Divorced - 4 or D, Widowed - 5 or W");
		break;
	}
	case PD_ChildrenNum:
	{
		InfoText.append(L"Children Num");
		MoreInfoText.append(L"Number 0-9");
		break;
	}
	case PD_MailCountry:
	{
		InfoText.append(L"Mailing Country");
		MoreInfoText.append(L"Example - UZB");
		break;
	}
	case PD_MailRegion:
	{
		InfoText.append(L"Mailing Region");
		MoreInfoText.append(L"Example - Samarkand");
		break;
	}
	case PD_MailCity:
	{
		InfoText.append(L"Mailing City");
		MoreInfoText.append(L"Example - Samarkand");
		break;
	}
	case PD_MailStreet:
	{
		InfoText.append(L"Mailing Street");
		MoreInfoText.append(L"Example - Sattepo street");
		break;
	}
	case PD_MailHomeNumber:
	{
		InfoText.append(L"Mailing Home Number");
		MoreInfoText.append(L"Example - 4/25");
		break;
	}
	case PD_MailZipCode:
	{
		InfoText.append(L"Mailing ZipCode");
		MoreInfoText.append(L"Disabling - Any enter whose length is below 3");
		break;
	}
	}

	DBLib::SetText(ButtonManager->GetWndHandler(IDC_W_Info), InfoText);
	DBLib::SetText(ButtonManager->GetWndHandler(IDC_W_MoreInfo), MoreInfoText);
}

void DBWindowWriter::SelectChild(size_t Index)
{
	if (Index > ChildrenNum)
	{
		NextPeople();
		return;
	}

	if (MembersData.Children.size() < Index)
	{
		MembersData.Children.push_back(DBPeopleData());
	}
	DataToChange = &MembersData.Children[Index - 1];
}

bool DBWindowWriter::CheckFormat()
{
	HWND  EditBoxWnd = ButtonManager->GetWndHandler(IDC_W_Edit);
	TCHAR buff[256];
	GetWindowText(EditBoxWnd, buff, 256);
	std::string Text;
	DBConvert::WStringToString(buff, Text);

	if (Text.size() == 0) return false;

	switch (PeopleData)
	{
	case PD_Name:
	{
		return true;
	}
	case PD_FamilyName:
	{
		return true;
	}
	case PD_Gender:
	{
		if (Text == "M" || Text == "1")
		{
			DBLib::SetText(EditBoxWnd, L"1");
			return true;
		}
		if (Text == "F" || Text == "2")
		{
			DBLib::SetText(EditBoxWnd, L"2");
			return true;
		}
		return false;
	}
	case PD_BirthMonth:
	{
		int Value = DBConvert::StringToInt(Text);
		if (Value > 0 && Value <= 12)
		{
			return true;
		}
		return false;
	}
	case PD_BirthDay:
	{
		int Value = DBConvert::StringToInt(Text);
		if (Value > 0 && Value <= 31)
		{
			return true;
		}
		return false;
	}
	case PD_BirthYear:
	{
		int Value = DBConvert::StringToInt(Text);
		if (Value > 1900 && Value <= 2100)
		{
			return true;
		}
		return false;
	}
	case PD_BornCountry:
	{
		return true;
	}
	case PD_EducationDegree:
	{
		if (Text == "1" || Text == "N")
		{
			DBLib::SetText(EditBoxWnd, L"1");
			return true;
		}
		if (Text == "2" || Text == "S")
		{
			DBLib::SetText(EditBoxWnd, L"2");
			return true;
		}
		if (Text == "3" || Text == "V")
		{
			DBLib::SetText(EditBoxWnd, L"3");
			return true;
		}
		if (Text == "4" || Text == "UC")
		{
			DBLib::SetText(EditBoxWnd, L"4");
			return true;
		}
		if (Text == "5" || Text == "U")
		{
			DBLib::SetText(EditBoxWnd, L"5");
			return true;
		}
		if (Text == "6" || Text == "GC")
		{
			DBLib::SetText(EditBoxWnd, L"6");
			return true;
		}
		if (Text == "7" || Text == "M")
		{
			DBLib::SetText(EditBoxWnd, L"7");
			return true;
		}
		if (Text == "8" || Text == "SD")
		{
			DBLib::SetText(EditBoxWnd, L"8");
			return true;
		}
		if (Text == "9" || Text == "D")
		{
			DBLib::SetText(EditBoxWnd, L"9");
			return true;
		}
		return false;
	}
	case PD_WhereLive:
	{
		return true;
	}
	case PD_MaritalStatus:
	{
		if (Text == "1" || Text == "U")
		{
			DBLib::SetText(EditBoxWnd, L"1");
			return true;
		}
		if (Text == "2" || Text == "M")
		{
			DBLib::SetText(EditBoxWnd, L"2");
			return true;
		}
		if (Text == "3" || Text == "US")
		{
			DBLib::SetText(EditBoxWnd, L"3");
			return true;
		}
		if (Text == "4" || Text == "D")
		{
			DBLib::SetText(EditBoxWnd, L"4");
			return true;
		}
		if (Text == "5" || Text == "W")
		{
			DBLib::SetText(EditBoxWnd, L"5");
			return true;
		}
		return false;
	}
	case PD_ChildrenNum:
	{
		int Value = DBConvert::StringToInt(Text);
		if (Value >= 0 && Value < 10)
		{
			return true;
		}
		return false;
	}
	case PD_MailCountry:
	{
		return true;
	}
	case PD_MailRegion:
	{
		return true;
	}
	case PD_MailCity:
	{
		return true;
	}
	case PD_MailStreet:
	{
		return true;
	}
	case PD_MailHomeNumber:
	{
		return true;
	}
	case PD_MailZipCode:
	{
		if (Text.size() < 3)
		{
			DBLib::SetText(EditBoxWnd, L"");
			return true;
		}
		return true;
	}
	}
	return false;
}

void DBWindowWriter::UpdateEditStyle()
{
	switch (PeopleData)
	{	// clang-format off
	case PD_Gender:				SetEditboxStyle(EDIT_STYLE_BASE, 1); break;
	case PD_BirthMonth:			SetEditboxStyle(ES_NUMBER, 2);	break;
	case PD_BirthDay:			SetEditboxStyle(ES_NUMBER, 2);	break;
	case PD_BirthYear:			SetEditboxStyle(ES_NUMBER, 4);	break;
	case PD_EducationDegree:	SetEditboxStyle(EDIT_STYLE_BASE, 2);	break;
	case PD_MaritalStatus:		SetEditboxStyle(EDIT_STYLE_BASE, 1);	break;
	case PD_ChildrenNum:		SetEditboxStyle(ES_NUMBER, 1);		break;
	case PD_MailZipCode:		SetEditboxStyle(ES_NUMBER);			break;
	default:					SetEditboxStyle(EDIT_STYLE_BASE);	break;
		// clang-format on
	}	// Switch PeopleData
}

void DBWindowWriter::UpdateEditText()
{
	if (bEditMode)
	{
		std::wstring Text;
		std::string	 SText;

		switch (PeopleData)
		{
		case PD_MaritalStatus: SText = std::to_string(MembersData.MaritalStatus); break;
		case PD_ChildrenNum: SText = std::to_string(MembersData.Children.size()); break;
		case PD_MailCountry: SText = MembersData.MailCountry; break;
		case PD_MailRegion: SText = MembersData.MailRegion; break;
		case PD_MailCity: SText = MembersData.MailCity; break;
		case PD_MailStreet: SText = MembersData.MailStreet; break;
		case PD_MailHomeNumber: SText = MembersData.MailHomeNumber; break;
		case PD_MailZipCode: SText = std::to_string(MembersData.MailZipCode); break;
		default:
		{
			if (bFinish)
			{
				return;
			}
			SText = std::string(DataToChange->GetAsString(PeopleData));
		}
		}
		DBConvert::StringToWString(SText, Text);
		HWND EditBox = ButtonManager->GetWndHandler(EDBWinCompId::IDC_W_Edit);
		DBLib::SetText(EditBox, Text);
	}
}

void DBWindowWriter::NextPeople()
{
	PeopleData = EPeopleData::PD_Name;
	PeopleType = static_cast<EPeopleType>(PeopleType + 1);
}

void DBWindowWriter::NextLine()
{
	PeopleData = static_cast<EPeopleData>(PeopleData + 1);
	UpdateEditText();
	UpdateEditStyle();
}

void DBWindowWriter::OpenImage()
{
	// Display the Open dialog box.

	bool ImageSaved = false;
	while (! ImageSaved)
	{
		OPENFILENAME ofn;		  // common dialog box structure
		wchar_t		 szFile[512]; // buffer for file name
		// HANDLE    hf;	      // file handle

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner	= WindowHandle;
		ofn.lpstrFile	= (LPWSTR)szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0]	= '\0';
		ofn.nMaxFile		= sizeof(szFile);
		ofn.lpstrFilter		= L"JPEG\0*.JP*G";
		ofn.nFilterIndex	= 1;
		ofn.lpstrFileTitle	= NULL;
		ofn.nMaxFileTitle	= 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// CDN_FILEOK

		if (GetOpenFileName(&ofn) == TRUE)
		{
			ImageSaved = SaveImage(ofn.lpstrFile);
		}
	}
}

bool DBWindowWriter::SaveImage(const std::wstring& InImagePath)
{
	auto DataManager = cmd::get::GetDataManager();
	if (! DataManager) return false;

	FImagePath NewImage;
	NewImage.Initial = InImagePath;

	const int ImageId = GetNextImageId() + ImagesToCopy.size();
	NewImage.Final	  = DBPaths::GetImagePath(DataManager->GetFolderId(), ImageId);
	NewImage.ImageId  = ImageId;
	ImagesToCopy.push_back(NewImage);

	// Save path to member data
	std::wstring CroppedPath = NewImage.Final;
	CroppedPath.erase(0, DBPaths::GetProjectPath().size()); // crop path

	// Convertion to string
	std::string DataToSave;
	DBConvert::WStringToString(CroppedPath, DataToSave);

	// Saving
	DataToChange->ImageFile = DataToSave;
	return true;
}

void DBWindowWriter::CopySavedImages()
{
	int LastImageId = 0;
	for (auto& Elem : ImagesToCopy)
	{
		CopyFile(Elem.Initial.c_str(), Elem.Final.c_str(), false);
		LastImageId = Elem.ImageId;
	}
	auto DataManager = cmd::get::GetDataManager();
	if (! DataManager) return;

	DataManager->WriteImageId(LastImageId);
}

int DBWindowWriter::GetNextImageId() const
{
	return LastImageId + 1;
}

void DBWindowWriter::SetItem(std::string& Info)
{
	if (! DataToChange) return;

	switch (PeopleData)
	{

	case PD_Name:
	{
		DataToChange->Name = Info;
		break;
	}
	case PD_FamilyName:
	{
		DataToChange->FamilyName = Info;
		break;
	}
	case PD_Gender:
	{
		DataToChange->Gender = DBConvert::StringToInt(Info);
		break;
	}
	case PD_BirthMonth:
	{
		DataToChange->BirthMonth = DBConvert::StringToInt(Info);
		break;
	}
	case PD_BirthDay:
	{
		DataToChange->BirthDay = DBConvert::StringToInt(Info);
		break;
	}
	case PD_BirthYear:
	{
		DataToChange->BirthYear = DBConvert::StringToInt(Info);
		break;
	}
	case PD_BornCountry:
	{
		DataToChange->BirthCountry = Info;
		break;
	}
	case PD_EducationDegree:
	{
		DataToChange->EducationDegree = DBConvert::StringToInt(Info);
		break;
	}
	case PD_ImageFile:
	{
		DataToChange->ImageFile = Info;
		break;
	}
	case PD_WhereLive:
	{
		DataToChange->WhereLive = Info;
		break;
	}
	case PD_MaritalStatus:
	{
		MembersData.MaritalStatus = DBConvert::StringToInt(Info);
		Status					  = MembersData.GetStatus(); // local
		break;
	}
	case PD_ChildrenNum:
	{
		MembersData.ChildrenNum = DBConvert::StringToInt(Info);
		ChildrenNum				= MembersData.ChildrenNum; // local
		break;
	}
	case PD_MailCountry:
	{
		MembersData.MailCountry = Info;
		break;
	}
	case PD_MailRegion:
	{
		MembersData.MailRegion = Info;
		break;
	}
	case PD_MailCity:
	{
		MembersData.MailCity = Info;
		break;
	}
	case PD_MailStreet:
	{
		MembersData.MailStreet = Info;
		break;
	}
	case PD_MailHomeNumber:
	{
		MembersData.MailHomeNumber = Info;
		break;
	}
	case PD_MailZipCode:
	{
		MembersData.MailZipCode = DBConvert::StringToInt(Info);
		break;
	}
	}
}

void DBWindowWriter::SetEditboxStyle(LONG Style, int TextLimit)
{
	SetWindowLongPtr(ButtonManager->GetWndHandler(IDC_W_Edit), GWL_STYLE, EDIT_STYLE_BASE | Style);
	SendMessage(ButtonManager->GetWndHandler(IDC_W_Edit), EM_SETLIMITTEXT, TextLimit, 0);
}

void DBWindowWriter::FinishWriting()
{
	const bool IsParent = PeopleType == EPeopleType::PT_Parent;
	const bool IsSpouse = PeopleType == EPeopleType::PT_Spouse;
	const bool IsChild	= ! (IsParent || IsSpouse);

	if (IsParent)
	{
		bFinish = Status != EMeritialStatus::MS_Married && ChildrenNum == EnteredChildrenNum;
	}

	if (IsSpouse)
	{
		bFinish = ChildrenNum == EnteredChildrenNum;
	}

	if (IsChild)
	{
		bFinish = ChildrenNum == EnteredChildrenNum;
	}

	if (bEditMode)
	{
		auto DataManager = cmd::get::GetDataManager();
		if (! DataManager) return;

		DataManager->SetMember(CurrentFamilyId, CurrentFolderId, MembersData);
		cmd::wnd::SetViewerData(CurrentFamilyId, CurrentFolderId, MembersData);

		bFinish = true;
		OnClose.Broadcast();
		return;
	}
	else if (bFinish)
	{
		auto DataManager = cmd::get::GetDataManager();
		if (! DataManager) return;

		DataManager->AddMember(MembersData);
		if (MembersData.Parents.size() > 1)
		{
			MembersData.SwitchParents();
			DataManager->AddMember(MembersData);
		}
		CopySavedImages();
		OnClose.Broadcast();
	}
}
