
#include "DBWindowWriter.h"
#include "DBFunctionLibrary.h"
#include <commdlg.h>
#include <comdef.h>
#include "DBWindowsManager.h"
#include "DBDataManager.h"

#define EDIT_STYLE_BASE WS_BORDER | WS_VISIBLE | WS_CHILD | ES_CENTER | ES_UPPERCASE | ES_WANTRETURN

DBWindowWriter* WriterObj = nullptr;
DBWindow		WriterEditBox;
DBWindow		WriterInfoBox;
DBWindow		WriterMoreInfoBox;

LRESULT CALLBACK WndWriterProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		WriterEditBox.Id	   = EDBWinCompId::IDC_W_Edit;
		WriterEditBox.Parent   = hWnd;
		WriterEditBox.Position = {25, 150};
		WriterEditBox.Size	   = {530, 40};
		WriterEditBox.Text	   = {};
		WriterEditBox.FontSize = 28;
		WriterEditBox.HIns	   = GetModuleHandle(NULL);

		DBLib::CreateEditBox(WriterEditBox, WS_VISIBLE | WS_CHILD | ES_CENTER | ES_UPPERCASE | ES_WANTRETURN);

		WriterInfoBox.Id	   = EDBWinCompId::IDC_W_Info;
		WriterInfoBox.Parent   = hWnd;
		WriterInfoBox.Position = {25, 25};
		WriterInfoBox.Size	   = {525, 125};
		WriterInfoBox.Text	   = L"Info Info Info Info Info Info Info Info Info Info Info Info Info Info";
		WriterInfoBox.FontSize = 24;
		WriterInfoBox.HIns	   = GetModuleHandle(NULL);

		DBLib::CreateStaticBox(WriterInfoBox, WS_VISIBLE | WS_CHILD);

		WriterMoreInfoBox.Id	   = EDBWinCompId::IDC_W_MoreInfo;
		WriterMoreInfoBox.Parent   = hWnd;
		WriterMoreInfoBox.Position = {50, 220};
		WriterMoreInfoBox.Size	   = {475, 100};
		WriterMoreInfoBox.Text	   = L"";
		WriterMoreInfoBox.FontSize = 18;
		WriterMoreInfoBox.HIns	   = GetModuleHandle(NULL);

		DBLib::CreateStaticBox(WriterMoreInfoBox, WS_VISIBLE | WS_CHILD);
	}
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
			if (! WriterObj) break;

			SetFocus(WriterEditBox.Window);

			WriterObj->SelectWriteData(WriterObj->PeopleType);
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
		RECT   Rect2	   = {15, 15, 565, 340};
		HBRUSH BrushColor  = CreateSolidBrush(RGB(200, 200, 220));
		HBRUSH BrushColor2 = CreateSolidBrush(RGB(240, 240, 240));

		FillRect(hdc, &Rect, BrushColor);
		FillRect(hdc, &Rect2, BrushColor2);

		EndPaint(hWnd, &ps);
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
	case WM_KEYDOWN:
	{
		// MessageBox(NULL, L"Sorry. It does't work", L"Dialog Box", MB_OK);
		break;
	}
	case WM_CLOSE:
	{
		DBLib::SetWindowVisibility(hWnd, false);
		return 0;
	}
	case WM_HOTKEY:
	{
		if (! WriterObj) break;

		if (WriterObj->CheckFormat())
		{
			WriterObj->WriteData();
			WriterObj->SelectWriteData(WriterObj->PeopleType);
		}

		SendMessage(WriterEditBox.Window, WM_SETTEXT, 0, (LPARAM)L"");
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

	/*
	char str[30];
	sprintf_s(str, sizeof(str), "Unfocus");
	OutputDebugStringA(str);
	*/

	return DefWindowProc(hWnd, message, wParam, lParam);
}

DBWindowWriter::DBWindowWriter(DBInterface* InOwner)
{
	SetOwner(InOwner);
	// auto Manager = static_cast<DBWindowsManager*>(GetOwner());
	auto Manager = Cast<DBWindowsManager>(GetOwner());
	if (Manager)
	{
		WindowHandle = Manager->GetWriterHandle();
		WriterObj	 = this;
	}
}

void DBWindowWriter::OnConstruct()
{
	// we can reset all data
}

void DBWindowWriter::SelectWriteData(EPeopleType PT)
{
	if (Finish) return;

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
	WriterObj->UpdateInfo();
	WriterObj->UpdateEditStyle();
}

void DBWindowWriter::WriteData()
{
	if (! DataToChange) return;

	TCHAR buff[256];
	GetWindowText(WriterEditBox.Window, buff, 256);

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
		InfoText.append(L"Family name");
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
		InfoText.append(L"Country where born");
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
		InfoText.append(L"Country where live today");
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
		InfoText.append(L"Mailing country");
		MoreInfoText.append(L"Example - UZB");
		break;
	}
	case PD_MailCity:
	{
		InfoText.append(L"Mailing city");
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
		InfoText.append(L"Mailing home number");
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

	DBLib::SetText(WriterInfoBox.Window, InfoText);
	DBLib::SetText(WriterMoreInfoBox.Window, MoreInfoText);
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
	TCHAR buff[256];
	GetWindowText(WriterEditBox.Window, buff, 256);
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
			DBLib::SetText(WriterEditBox.Window, L"1");
			return true;
		}
		if (Text == "F" || Text == "2")
		{
			DBLib::SetText(WriterEditBox.Window, L"2");
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
		if (Value > 1900 && Value <= 2008)
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
			DBLib::SetText(WriterEditBox.Window, L"1");
			return true;
		}
		if (Text == "2" || Text == "S")
		{
			DBLib::SetText(WriterEditBox.Window, L"2");
			return true;
		}
		if (Text == "3" || Text == "V")
		{
			DBLib::SetText(WriterEditBox.Window, L"3");
			return true;
		}
		if (Text == "4" || Text == "UC")
		{
			DBLib::SetText(WriterEditBox.Window, L"4");
			return true;
		}
		if (Text == "5" || Text == "U")
		{
			DBLib::SetText(WriterEditBox.Window, L"5");
			return true;
		}
		if (Text == "6" || Text == "GC")
		{
			DBLib::SetText(WriterEditBox.Window, L"6");
			return true;
		}
		if (Text == "7" || Text == "M")
		{
			DBLib::SetText(WriterEditBox.Window, L"7");
			return true;
		}
		if (Text == "8" || Text == "SD")
		{
			DBLib::SetText(WriterEditBox.Window, L"8");
			return true;
		}
		if (Text == "9" || Text == "D")
		{
			DBLib::SetText(WriterEditBox.Window, L"9");
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
			DBLib::SetText(WriterEditBox.Window, L"1");
			return true;
		}
		if (Text == "2" || Text == "M")
		{
			DBLib::SetText(WriterEditBox.Window, L"2");
			return true;
		}
		if (Text == "3" || Text == "US")
		{
			DBLib::SetText(WriterEditBox.Window, L"3");
			return true;
		}
		if (Text == "4" || Text == "D")
		{
			DBLib::SetText(WriterEditBox.Window, L"4");
			return true;
		}
		if (Text == "5" || Text == "W")
		{
			DBLib::SetText(WriterEditBox.Window, L"5");
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
			DBLib::SetText(WriterEditBox.Window, L"");
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
	case PD_Gender:				SetEditboxStyle(1);				break;
	case PD_BirthMonth:			SetEditboxStyle(ES_NUMBER, 2);	break;
	case PD_BirthDay:			SetEditboxStyle(ES_NUMBER, 2);	break;
	case PD_BirthYear:			SetEditboxStyle(ES_NUMBER, 4);	break;
	case PD_EducationDegree:	SetEditboxStyle(2);				break;
	case PD_MaritalStatus:		SetEditboxStyle(1);				break;
	case PD_ChildrenNum:		SetEditboxStyle(ES_NUMBER, 1);	break;
	case PD_MailZipCode:		SetEditboxStyle(ES_NUMBER);		break;
	default:					SetEditboxStyle();				break;
		// clang-format on
	}	// Switch PeopleData
}

void DBWindowWriter::NextPeople()
{
	PeopleData = EPeopleData::PD_Name;
	PeopleType = static_cast<EPeopleType>(PeopleType + 1);
}

void DBWindowWriter::NextLine()
{
	PeopleData = static_cast<EPeopleData>(PeopleData + 1);
}

void DBWindowWriter::OpenImage()
{
	// Display the Open dialog box.

	bool ImageCopied = false;
	while (! ImageCopied)
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
			//	hf = CreateFile(ofn.lpstrFile,	 // File Name
			//	GENERIC_READ,				 //
			//	0,							 //
			//	(LPSECURITY_ATTRIBUTES)NULL, //
			//	OPEN_EXISTING,				 //
			//	FILE_ATTRIBUTE_NORMAL,		 //
			//	(HANDLE)NULL);

			ImagePath = ofn.lpstrFile;
			// MessageBox(NULL, ImagePath.c_str(), L"Dialog Box", MB_OK);

			ImageCopied = CopyImage();
		}
	}
}

bool DBWindowWriter::CopyImage()
{
	assert(DataToChange);

	if (! GetSystem()) return false;
	auto DataManager = GetSystem()->GetComponent<DBDataManager>();

	if (! DataManager) return false;

	std::wstring FilePath  = DataManager->GenerateImagePath();
	std::wstring FileName  = DataManager->GenerateImageName();
	std::wstring FinalPath = FilePath.append(FileName);

	std::wstring NewPath		   = FinalPath;
	int			 SizeToProjectPath = DBPaths::GetProjectPath().size();
	NewPath.erase(0, SizeToProjectPath);
	std::string NewStringPath;

	DBConvert::WStringToString(NewPath, NewStringPath);
	DataToChange->ImageFile = NewStringPath;

	// MessageBox(NULL, FinalPath.c_str(), L"Dialog Box", MB_OK);

	if (CopyFile(ImagePath.c_str(), FinalPath.c_str(), false))
	{
		return true;
	}
	else
	{
		std::wstring ErrorCode(L"Error Code: ");
		ErrorCode.append(std::to_wstring(GetLastError()));
		OutputDebugString(ErrorCode.c_str());
		MessageBox(NULL, L"Error", L"Dialog Box", MB_OK);
		// ERROR_PATH_NOT_FOUND //  error codes
		return false;
	}
	return false;
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
	SetWindowLongPtr(WriterEditBox.Window, GWL_STYLE, EDIT_STYLE_BASE | Style);
	SendMessage(WriterEditBox.Window, EM_SETLIMITTEXT, TextLimit, 0);
}

void DBWindowWriter::FinishWriting()
{
	const bool IsParent = PeopleType == EPeopleType::PT_Parent;
	const bool IsSpouse = PeopleType == EPeopleType::PT_Spouse;
	const bool IsChild	= ! (IsParent || IsSpouse);

	if (IsParent)
	{
		Finish = Status != EMeritialStatus::MS_Married && ChildrenNum == EnteredChildrenNum;
	}

	if (IsSpouse)
	{
		Finish = ChildrenNum == EnteredChildrenNum;
	}

	if (IsChild)
	{
		Finish = ChildrenNum == EnteredChildrenNum;
	}

	if (Finish)
	{
		if (! GetSystem()) return;

		auto DataManager = GetSystem()->GetComponent<DBDataManager>();
		if (! DataManager) return;

		DataManager->AddMember(MembersData);
		if (MembersData.Parents.size() > 1)
		{
			MembersData.SwitchParents();
			DataManager->AddMember(MembersData);
		}

		OnWriteSuccess.Broadcast();
	}
}
