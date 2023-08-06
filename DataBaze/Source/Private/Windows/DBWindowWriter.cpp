
#include "DBWindowWriter.h"
#include "DBDataManager.h"
#include "DBButtonManager.h"

#include <commdlg.h>
#include <comdef.h>
#include <map>

#include "UILibrary.h"

#define EDIT_STYLE_BASE WS_BORDER | WS_VISIBLE | WS_CHILD | ES_CENTER | ES_UPPERCASE | ES_WANTRETURN

LRESULT DBWindowWriter::CallProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SHOWWINDOW:
	{
		if (wParam == FALSE)
		{
			UnregisterHotKey(hWnd, HKW_Enter);
			UnregisterHotKey(hWnd, HKW_Revert);
			UnregisterHotKey(hWnd, HKW_CloseWnd);
		}
	}
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			SetFocus(ButtonManager->GetWndHandler(IDC_W_Edit));
			if (! bEditMode)
			{
				SelectWriteData(PT_Parent);
			}
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
		UnregisterHotKey(hWnd, HKW_Enter);
		UnregisterHotKey(hWnd, HKW_Revert);
		UnregisterHotKey(hWnd, HKW_CloseWnd);
		OnClose.Broadcast();
		return 0;
	}
	case WM_HOTKEY:
	{
		SetFocus(ButtonManager->GetWndHandler(IDC_W_Edit));
		switch (wParam)
		{
		case HKW_Enter:
		{
			if (CheckFormat())
			{
				WriteData();
				SelectWriteData(PeopleType);
			}
			UpdateEditText();
			break;
		}
		case HKW_Revert:
		{
			Revert();
			break;
		}
		case HKW_CloseWnd:
		{
			OnClose.Broadcast();
			UnregisterHotKey(hWnd, HKW_Enter);
			UnregisterHotKey(hWnd, HKW_Revert);
			UnregisterHotKey(hWnd, HKW_CloseWnd);
			break;
		}
		}
		break;
	}
	case WM_ACTIVATE:
	{
		// Window activation/deactivation handling
		if (LOWORD(wParam) != WA_INACTIVE)
		{
			RegisterHotKey(hWnd, HKW_Enter, 0, VK_RETURN);
			RegisterHotKey(hWnd, HKW_Revert, 0, VK_ESCAPE);
			RegisterHotKey(hWnd, HKW_CloseWnd, 0, VK_F4);
		}
		else
		{
			UnregisterHotKey(hWnd, HKW_Enter);
			UnregisterHotKey(hWnd, HKW_Revert);
			UnregisterHotKey(hWnd, HKW_CloseWnd);
		}
		break;
	}
	} // switch end
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

		ButtonManager->AddSlot(0, VBox);
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
	}
	UpdateInfo();
	UpdateEditStyle();
}

void DBWindowWriter::EditPeople(FMemberId InId, const DBFamilyData& Data, EPeopleType People)
{
	MemberId	= InId;
	bEditMode	= true;
	PeopleType	= People;
	MembersData = Data;
	SelectWriteData(PeopleType);
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
			if (Status != EMeritialStatus::MS_Married)
			{
				NextPeople();
			}
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
	default:
	{
		if (bEditMode)
		{
			if (PeopleType != PT_None)
			{
				switch (PeopleData)
				{
				case PD_NotChildInfo:
				{
					FinishWriting();
					NextPeople();
				}
				case PD_EducationDegree:
				{
					FinishWriting();
					NextPeople();
				}
				case PD_ImageFile:
				{
					if (bEditMode && DataToChange->ImageFile.length() != 0)
					{
						FinishWriting();
						return;
					}
				}
				}
			}
		}
	}
	} // switch

	const bool IsParent = PeopleType == EPeopleType::PT_Parent;
	const bool IsSpouse = PeopleType == EPeopleType::PT_Spouse;
	const bool IsChild	= ! (IsParent || IsSpouse);

	if (PeopleData == PD_ImageFile)
	{
		if (DataToChange->ImageFile.length() != 0)
		{
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
			return;
		}
		if (OpenImage())
		{
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
		else
		{
			Revert();
		}
	}
}

void DBWindowWriter::UpdateInfo()
{
	if (PeopleData == EPeopleData::PD_ImageFile) return;

	std::wstring InfoText;
	std::wstring MoreInfoText;

	std::map<EPeopleType, std::wstring> PeopleTitleText = {
		{PT_Parent, L"Parent 1"}, //
		{PT_Spouse, L"Parent 2"}, //
		{PT_Child_1, L"Child 1"}, //
		{PT_Child_2, L"Child 2"}, //
		{PT_Child_3, L"Child 3"}, //
		{PT_Child_4, L"Child 4"}, //
		{PT_Child_5, L"Child 5"}, //
		{PT_Child_6, L"Child 6"}, //
		{PT_Child_7, L"Child 7"}, //
	};

	if (PeopleTitleText.find(PeopleType) != PeopleTitleText.end())
	{
		InfoText.append(L"<   ").append(PeopleTitleText[PeopleType]).append(L"   >\n");
	}

	std::map<EPeopleData, std::wstring> DataTitleText = {
		{PD_Name, L"Name"},							 //
		{PD_FamilyName, L"Family Name"},			 //
		{PD_Gender, L"Gender"},						 //
		{PD_BirthMonth, L"Birth Month"},			 //
		{PD_BirthDay, L"Birth Day"},				 //
		{PD_BirthYear, L"Birth Year"},				 //
		{PD_BornCity, L"City where Born"},			 //
		{PD_BornCountry, L"Country where Born"},	 //
		{PD_EducationDegree, L"Education"},			 //
		{PD_WhereLive, L"Country Where Live Today"}, //
		{PD_MaritalStatus, L"Marital Status"},		 //
		{PD_ChildrenNum, L"Children Num"},			 //
		{PD_MailCountry, L"Mail Country"},			 //
		{PD_MailRegion, L"Mail Region"},			 //
		{PD_MailCity, L"Mail City"},				 //
		{PD_MailStreet, L"Mail Street"},			 //
		{PD_MailHomeNumber, L"Mail Home Number"},	 //
		{PD_MailZipCode, L"Mail Zip"}				 //
	};

	if (DataTitleText.find(PeopleData) != DataTitleText.end())
	{
		InfoText.append(DataTitleText[PeopleData]);
	}

	std::map<EPeopleData, std::wstring> DataInfoText = {
		{PD_Gender, L"1 or M - Mele\n2 or F - Famele"}, //
		{PD_BornCountry, L"Example - UZB"},				//
		{PD_EducationDegree,
			L"H.School no degree - 1 or N, H.School - 2 or S, Vocational - 3 or V, University Courses - 4 or UC, "
			L"University - 5 or U, Graduate level Courses - 6 or GC, Masters - 7 or M, Doctoral Courses - 8 or SD, Doctorate - 9 or D"}, //
		{PD_WhereLive, L"Example - RUS"},																								 //
		{PD_MaritalStatus,
			L"Unmarried - 1 or U, Married - 2 or M, Spouse is a US citizen - 3 or US, Divorced - 4 or D, Widowed - 5 or W"}, //
		{PD_ChildrenNum, L"Number 0-9"},																					 //
		{PD_MailCountry, L"Example - UZB"},																					 //
		{PD_MailRegion, L"Example - Samarkand"},																			 //
		{PD_MailCity, L"Example - Samarkand"},																				 //
		{PD_MailStreet, L"Example - Sattepo street"},																		 //
		{PD_MailHomeNumber, L"Example - 4/25"},																				 //
		{PD_MailZipCode, L"Disabling - Any enter whose length is below 3"},													 //
	};

	if (DataInfoText.find(PeopleData) != DataInfoText.end())
	{
		MoreInfoText.append(DataInfoText[PeopleData]);
	}

	DBLib::SetText(ButtonManager->GetWndHandler(IDC_W_Info), InfoText);
	DBLib::SetText(ButtonManager->GetWndHandler(IDC_W_MoreInfo), MoreInfoText);
}

void DBWindowWriter::SelectChild(size_t Index)
{
	if (! bEditMode)
	{
		if (Index > ChildrenNum)
		{
			NextPeople();
			return;
		}
	}
	else
	{
		if (Index > MembersData.Children.size())
		{
			NextPeople();
			return;
		}
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
	case PD_BornCity:
	{
		return true;
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
		if (Value >= 0 && Value < 8)
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
	case PD_Gender:				SetEditboxStyle(EDIT_STYLE_BASE, 1);	break;
	case PD_BirthMonth:			SetEditboxStyle(ES_NUMBER, 2);			break;
	case PD_BirthDay:			SetEditboxStyle(ES_NUMBER, 2);			break;
	case PD_BirthYear:			SetEditboxStyle(ES_NUMBER, 4);			break;
	case PD_EducationDegree:	SetEditboxStyle(EDIT_STYLE_BASE, 2);	break;
	case PD_MaritalStatus:		SetEditboxStyle(EDIT_STYLE_BASE, 1);	break;
	case PD_ChildrenNum:		SetEditboxStyle(ES_NUMBER, 1);			break;
	case PD_MailZipCode:		SetEditboxStyle(ES_NUMBER, 6);			break;
	default:					SetEditboxStyle(EDIT_STYLE_BASE);		break;
		// clang-format on
	}	// Switch PeopleData
}

void DBWindowWriter::UpdateEditText()
{
	std::wstring Text;
	std::string	 SText;

	switch (PeopleData)
	{
	case PD_MaritalStatus:
	{
		if (MembersData.MaritalStatus)
		{
			SText = std::to_string(MembersData.MaritalStatus);
		}
		break;
	}
	case PD_ChildrenNum:
	{
		if (MembersData.Children.size())
		{
			SText = std::to_string(MembersData.Children.size());
		}
		break;
	}
	case PD_MailCountry: SText = MembersData.MailCountry; break;
	case PD_MailRegion: SText = MembersData.MailRegion; break;
	case PD_MailCity: SText = MembersData.MailCity; break;
	case PD_MailStreet: SText = MembersData.MailStreet; break;
	case PD_MailHomeNumber: SText = MembersData.MailHomeNumber; break;
	case PD_MailZipCode:
	{
		if (MembersData.MailZipCode)
		{
			SText = std::to_string(MembersData.MailZipCode);
			break;
		}
	}
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

	SendMessage(EditBox, EM_SETSEL, (WPARAM)SText.size(), (LPARAM)SText.size());
}

void DBWindowWriter::NextPeople()
{
	PeopleData = EPeopleData::PD_Name;
	PeopleType = static_cast<EPeopleType>(PeopleType + 1);
}

void DBWindowWriter::NextLine()
{
	PeopleData = static_cast<EPeopleData>(PeopleData + 1);
	UpdateEditStyle();
	if (bEditMode)
	{
		UpdateEditText();
	}
}

bool DBWindowWriter::OpenImage()
{
	// Display the Open dialog box.

	// bool ImageSaved = false;
	// while (! ImageSaved)

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
		// ImageSaved = SaveImage(ofn.lpstrFile);
		SaveImage(ofn.lpstrFile);
		return true;
	}
	return false;
}

bool DBWindowWriter::SaveImage(const std::wstring& InImagePath)
{
	auto DataManager = cmd::get::GetDataManager();
	if (! DataManager) return false;

	FImagePath NewImage;
	NewImage.Initial = InImagePath;

	const int ImageId = GetNextImageId() + ImagesToCopy.size();
	NewImage.Final	  = DBPaths::GetImagePath(DataManager->GetSelectedFolderId(), ImageId);
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
	if (ImagesToCopy.size() == 0) return;

	int LocLastImgId = 0;
	for (auto& Elem : ImagesToCopy)
	{
		CopyFile(Elem.Initial.c_str(), Elem.Final.c_str(), false);
		LocLastImgId = Elem.ImageId;
	}
	auto DataManager = cmd::get::GetDataManager();
	if (! DataManager) return;

	DataManager->WriteImageId(LocLastImgId);
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
	case PD_BornCity:
	{
		DataToChange->BirthCity = Info;
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
		MembersData.Parents.resize((Status == EMeritialStatus::MS_Married) ? 2 : 1);
		break;
	}
	case PD_ChildrenNum:
	{
		const size_t ArrSize = DBConvert::StringToInt(Info);
		MembersData.Children.resize(ArrSize);
		MembersData.ChildrenNum = ArrSize;
		ChildrenNum				= ArrSize; // local
		break;
	}
	case PD_MailCountry:
	{
		MembersData.MailCountry = Info;
		break;
	}
	case PD_MailRegion:
	{
		if (Info == "0" || Info == " ")
		{
			MembersData.MailRegion = "";
			break;
		}
		MembersData.MailRegion = Info;
		break;
	}
	case PD_MailCity:
	{
		if (Info == "0" || Info == " ")
		{
			MembersData.MailCity = "";
			break;
		}
		MembersData.MailCity = Info;
		break;
	}
	case PD_MailStreet:
	{
		if (Info == "0" || Info == " ")
		{
			MembersData.MailStreet = "";
			break;
		}
		MembersData.MailStreet = Info;
		break;
	}
	case PD_MailHomeNumber:
	{
		if (Info == "0" || Info == " ")
		{
			MembersData.MailHomeNumber = "0";
			break;
		}
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

		DataManager->SetMember(MemberId, MembersData);
		cmd::wnd::SetViewerData(MemberId, MembersData);

		CopySavedImages();
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

//==============================================================================//
//==================================================//
void DBWindowWriter::Revert()
{
	if (bFinish) return;

	const int IntPeopleData = static_cast<int>(PeopleData);
	if (IntPeopleData > 1)
	{
		PeopleData = static_cast<EPeopleData>(IntPeopleData - 1);
		while (PeopleData == EPeopleData::PD_NotChildInfo ||   //
			   PeopleData == EPeopleData::PD_OnlyParentInfo || //
			   PeopleData == EPeopleData::PD_ImageFile)
		{
			PeopleData = static_cast<EPeopleData>((int)PeopleData - 1);
		}
	}
	else
	{
		if (bEditMode) return;

		const int IntPeopleType = static_cast<int>(PeopleType);
		if (IntPeopleType <= 1) return; // if its parent 1

		PeopleType = static_cast<EPeopleType>(IntPeopleType - 1);

		switch (PeopleType)
		{
		case EPeopleType::PT_Parent:
		{
			PeopleData = (EPeopleData)((int)EPeopleData::PD_Max - 1);
			break;
		}
		case EPeopleType::PT_Spouse:
		{
			if (MembersData.Parents.size() == 2)
			{
				PeopleData = (EPeopleData)((int)EPeopleData::PD_OnlyParentInfo - 1);
			}
			else // no spose
			{
				PeopleType = EPeopleType::PT_Parent;
				PeopleData = (EPeopleData)((int)EPeopleData::PD_Max - 1);
			}
			break;
		}
		default: // Child info
		{
			--EnteredChildrenNum;
			PeopleData = (EPeopleData)((int)EPeopleData::PD_NotChildInfo - 1);
			while ((int)PeopleData >= EPeopleData::PD_NotChildInfo || //
				   PeopleData == EPeopleData::PD_OnlyParentInfo ||	  //
				   PeopleData == EPeopleData::PD_ImageFile)
			{
				PeopleData = static_cast<EPeopleData>((int)PeopleData - 1);
			}
			break;
		}
		} // end switch
	}

	// you must change people data ptr
	SelectWriteData(PeopleType);
	UpdateEditText();
}
