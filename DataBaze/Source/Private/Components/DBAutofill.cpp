#include "DBAutofill.h"
#include "DBKeyCodes.h"
#include "DBFunctionLibrary.h"
#include <map>
#include "DBSettingsComp.h"
#include "DBCommandCenter.h"

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

bool ConfirmationFound = false;

DBAutofill::~DBAutofill()
{
	Enabled = false;
	Clear();
}

void DBAutofill::Init(const DBFamilyData& InUserData, FMemberId InId, HWND InOwnerWindow)
{
	UserData	= InUserData;
	MemberId	= InId;
	OwnerWindow = InOwnerWindow;
}

void DBAutofill::InitMemberActions(const DBFamilyData& Data)
{
	const std::vector<int> HK_Paste = {VK_CONTROL, VK_V};

	InitSubMemberActions(Data.Parents[0], true);

	// Mail
	ActionList.push_back(new DBAction_PressButtons(VK_TAB, 3));
	ActionList.push_back(new DBAction_Clipboard(Data.MailStreet, OwnerWindow));
	ActionList.push_back(new DBAction_PressButtons(HK_Paste));
	ActionList.push_back(new DBAction_PressButtons(VK_TAB, 3));

	if (Data.MailHomeNumber.size() > 0 && DBConvert::StringToInt(Data.MailHomeNumber) != 0)
	{
		ActionList.push_back(new DBAction_Clipboard(Data.MailHomeNumber, OwnerWindow));
		ActionList.push_back(new DBAction_PressButtons(HK_Paste));
	}
	ActionList.push_back(new DBAction_PressButtons(VK_TAB));

	ActionList.push_back(new DBAction_Clipboard(Data.MailCity, OwnerWindow));
	ActionList.push_back(new DBAction_PressButtons(HK_Paste));
	ActionList.push_back(new DBAction_PressButtons(VK_TAB));

	ActionList.push_back(new DBAction_Clipboard(Data.MailRegion, OwnerWindow));
	ActionList.push_back(new DBAction_PressButtons(HK_Paste));
	ActionList.push_back(new DBAction_PressButtons(VK_TAB));

	if (Data.IsHasZipCode())
	{
		ActionList.push_back(new DBAction_Clipboard(std::to_string(Data.MailZipCode), OwnerWindow));
		ActionList.push_back(new DBAction_PressButtons(HK_Paste));
	}
	else
	{
		ActionList.push_back(new DBAction_PressButtons(VK_TAB));
		ActionList.push_back(new DBAction_PressButtons(VK_SPACE));
		ActionList.push_back(new DBAction_PressButtons(VK_TAB));
	}
	ActionList.push_back(new DBAction_PressButtons(Data.MailCountry));
	ActionList.push_back(new DBAction_PressButtons(VK_TAB));

	ActionList.push_back(new DBAction_PressButtons(Data.Parents[0].WhereLive));
	ActionList.push_back(new DBAction_PressButtons(VK_TAB, 2));

	// EMail
	ActionList.push_back(new DBAction_WriteMail());
	ActionList.push_back(new DBAction_PressButtons(VK_TAB));
	ActionList.push_back(new DBAction_WriteMail());
	ActionList.push_back(new DBAction_PressButtons(VK_TAB));

	// Education level
	{
		const int EducationLevel = Data.Parents[0].EducationDegree - 1;
		const int SkipCount		 = 10 - EducationLevel;
		ActionList.push_back(new DBAction_PressButtons(VK_TAB, EducationLevel));
		ActionList.push_back(new DBAction_PressButtons(VK_SPACE));
		ActionList.push_back(new DBAction_PressButtons(VK_TAB, SkipCount));
	}

	// Select meritial status
	{
		const int StatusLevel = Data.MaritalStatus - 1;
		const int SkipCount	  = 6 - StatusLevel;
		ActionList.push_back(new DBAction_PressButtons(VK_TAB, StatusLevel));
		ActionList.push_back(new DBAction_PressButtons(VK_SPACE));
		ActionList.push_back(new DBAction_PressButtons(VK_TAB, SkipCount));
	}
	ActionList.push_back(new DBAction_Clipboard(std::to_string(Data.ChildrenNum), OwnerWindow));
	ActionList.push_back(new DBAction_PressButtons(HK_Paste));
	ActionList.push_back(new DBAction_PressButtons(VK_TAB));
}

void DBAutofill::InitSubMemberActions(const DBPeopleData& Data, bool FirstPeople)
{
	const std::vector<int> HK_Paste = {VK_CONTROL, VK_V};

	ActionList.push_back(new DBAction_Clipboard(Data.FamilyName, OwnerWindow));
	ActionList.push_back(new DBAction_PressButtons(HK_Paste));
	ActionList.push_back(new DBAction_PressButtons(VK_TAB));

	ActionList.push_back(new DBAction_Clipboard(Data.Name, OwnerWindow));
	ActionList.push_back(new DBAction_PressButtons(HK_Paste));
	ActionList.push_back(new DBAction_PressButtons(VK_TAB, 3));
	ActionList.push_back(new DBAction_PressButtons(VK_SPACE));
	ActionList.push_back(new DBAction_PressButtons(VK_TAB));
	// Gender
	if (Data.IsMele())
	{
		ActionList.push_back(new DBAction_PressButtons(VK_SPACE));
		ActionList.push_back(new DBAction_PressButtons(VK_TAB, 2));
	}
	else
	{
		ActionList.push_back(new DBAction_PressButtons(VK_TAB));
		ActionList.push_back(new DBAction_PressButtons(VK_SPACE));
		ActionList.push_back(new DBAction_PressButtons(VK_TAB));
	}
	ActionList.push_back(new DBAction_Clipboard(std::to_string(Data.BirthMonth), OwnerWindow));
	ActionList.push_back(new DBAction_PressButtons(HK_Paste));
	ActionList.push_back(new DBAction_PressButtons(VK_TAB));

	ActionList.push_back(new DBAction_Clipboard(std::to_string(Data.BirthDay), OwnerWindow));
	ActionList.push_back(new DBAction_PressButtons(HK_Paste));
	ActionList.push_back(new DBAction_PressButtons(VK_TAB));

	ActionList.push_back(new DBAction_Clipboard(std::to_string(Data.BirthYear), OwnerWindow));
	ActionList.push_back(new DBAction_PressButtons(HK_Paste));
	ActionList.push_back(new DBAction_PressButtons(VK_TAB));

	// Fill City
	if (Data.IsBirthCityValid())
	{
		ActionList.push_back(new DBAction_Clipboard(Data.BirthCity, OwnerWindow));
		ActionList.push_back(new DBAction_PressButtons(HK_Paste));
		ActionList.push_back(new DBAction_PressButtons(VK_TAB, 2));
	}
	else
	{
		ActionList.push_back(new DBAction_PressButtons(VK_TAB));
		ActionList.push_back(new DBAction_PressButtons(VK_SPACE));
		ActionList.push_back(new DBAction_PressButtons(VK_TAB));
	}

	ActionList.push_back(new DBAction_PressButtons(Data.BirthCountry));
	ActionList.push_back(new DBAction_PressButtons(VK_TAB));

	if (FirstPeople)
	{
		ActionList.push_back(new DBAction_PressButtons(VK_TAB, 4));
	}
	ActionList.back()->DelaySeconds = DBAutofillSettings::LoadOption(ESettingType::ImageOpen);

	// Image
	const std::string SImage = Data.ImageFile;
	std::wstring	  WImage;
	DBConvert::StringToWString(SImage, WImage);
	std::wstring ImagePath = DBPaths::GetDataFolderPath(MemberId.FolderId).append(WImage);

	ActionList.push_back(new DBAction_Clipboard(ImagePath, OwnerWindow));
	ActionList.push_back(new DBAction_PressButtons(HK_Paste));
	ActionList.push_back(new DBAction_PressButtons(VK_RETURN));
	ActionList.back()->DelaySeconds = DBAutofillSettings::LoadOption(ESettingType::ImageClose);

	ActionList.push_back(new DBAction_PressButtons(VK_TAB));
}

void DBAutofill::InitSaveResult()
{
	const std::vector<int> SelectAll = {VK_CONTROL, VK_A};
	ActionList.push_back(new DBAction_PressButtons(SelectAll));

	const std::vector<int> Copy = {VK_CONTROL, VK_C};
	ActionList.push_back(new DBAction_PressButtons(Copy));

	ActionList.back()->DelaySeconds = 0.2f;

	std::wstring FileName = DBPaths::GenerateConfirmFileName(UserData, MemberId);
	ActionList.push_back(new DBAction_SaveToFile(FileName, MemberId));
}

void DBAutofill::InitActionStep(EAutofillStep Step)
{
	Clear();

	switch (Step)
	{
	case EAutofillStep::None:
	{
		// auto switch to next step
		int CurrentStepInt = static_cast<int>(CurrentStep);
		CurrentStep		   = static_cast<EAutofillStep>(CurrentStepInt + 1);
		if (CurrentStep == EAutofillStep::Max)
		{
			CurrentStep = EAutofillStep::Page1;
		}
		InitActionStep(CurrentStep);
		break;
	}
	case EAutofillStep::Page1:
	{
		InitMemberActions(UserData);
		CurrentStep = EAutofillStep::Page1;
		break;
	}
	case EAutofillStep::Page2:
	{
		if (UserData.IsHasASpouse() || UserData.ChildrenNum > 0)
		{
			if (UserData.IsHasASpouse())
			{
				InitSubMemberActions(UserData.Parents[1]);
			}

			for (int i = 0; i < UserData.ChildrenNum; ++i)
			{
				InitSubMemberActions(UserData.Children[i]);
			}
			CurrentStep = EAutofillStep::Page2;
		}
		else
		{
			InitActionStep(EAutofillStep::SaveResult);
		}
		break;
	}
	case EAutofillStep::SaveResult:
	{
		InitSaveResult();
		CurrentStep = EAutofillStep::SaveResult;
		break;
	}
	case EAutofillStep::Max:
	{
		CurrentStep = EAutofillStep::None;
		break;
	}
	} // switch
}

void DBAutofill::StartFilling(EAutofillStep Step)
{
	if (Enabled) return;

	Wait(1.f);

	// Init Actions
	CurrentActionIndex = 0;
	InitActionStep(Step);

	Enabled = true;
}

void DBAutofill::Clear()
{
	for (auto& Elem : ActionList)
	{
		delete Elem;
	}
	ActionList.clear();
}

void DBAutofill::Tick(float DeltaTime)
{
	if ((int)ActionList.size() == 0 || ! Enabled) return;

	if (CurrentActionIndex >= (int)ActionList.size())
	{
		Enabled = false;
		Clear();

		if (CurrentStep == EAutofillStep::SaveResult)
		{
			if (ConfirmationFound)
			{
				OnFinish.Broadcast();
			}
			else
			{
				CurrentStep = EAutofillStep::Page2;
				OnFinishWithError.Broadcast();
			}
		}
		return;
	}
	else
	{
		if (InProgress) return;

		InProgress	= true;
		auto Action = ActionList[CurrentActionIndex];
		Wait(Action->DelaySeconds);
		Action->DoAction();
		++CurrentActionIndex;
		InProgress = false;
	}
}

void DBAction_PressButtons::DoAction()
{
	switch (Type)
	{
	case DBAction_PressButtons::Normal:
	{
		for (int i = 0; i < Times; ++i)
		{
			DBInput::PressKey(SingleButton);
		}
		break;
	}
	case DBAction_PressButtons::Text:
	{
		for (auto& Elem : ButtonList)
		{
			DBInput::PressKey(Elem);
		}
		break;
	}
	case DBAction_PressButtons::Hotkey:
	{
		DBInput::PressKeys(ButtonList[0], ButtonList[1]);
		break;
	}
	} // switch end
}

void DBAction_PressButtons::StringToButtonList(std::string& InText)
{
	for (std::string::iterator it = InText.begin(); it != InText.end(); ++it)
	{
		if (VKeys.find(*it) != VKeys.end())
		{
			ButtonList.push_back(VKeys[*it]);
		}
	}
}

void DBAction_WriteMail::DoAction()
{
	std::string Mail = cmd::ini::GetStringValue(DBIniItem::AutoFill_Mail);

	char delimiter = '@';

	// »щем позицию символа "@" в строке
	size_t pos = Mail.find(delimiter);

	if (pos != std::string::npos)
	{
		// »спользуем метод substr дл€ получени€ двух частей строки
		std::string part1 = Mail.substr(0, pos); // от начала строки до "@" (не включа€ "@")
		std::string part2 = Mail.substr(pos + 1);

		WriteString(part1);
		DBInput::PressKeys(VK_SHIFT, VK_2);
		WriteString(part2);
	}
}

void DBAction_WriteMail::WriteString(std::string& Text)
{
	for (std::string::iterator it = Text.begin(); it != Text.end(); ++it)
	{
		if (VKeys.find(*it) != VKeys.end())
		{
			DBInput::PressKey(VKeys[*it]);
		}
	}
}

void DBAction_Clipboard::DoAction()
{
	if (! TextToClip.empty())
	{
		DBInput::CopyToClipboard(WindowHandle, TextToClip);
	}
	if (! WTextToClip.empty())
	{
		DBInput::CopyToClipboard(WindowHandle, WTextToClip);
	}
}

void DBAction_SaveToFile::DoAction()
{
	if (OpenClipboard(NULL))
	{
		HANDLE		 clip = GetClipboardData(CF_UNICODETEXT);
		std::wstring Code = std::wstring((wchar_t*)clip);
		CloseClipboard();

		const std::wstring search = L"Confirmation";
		const size_t	   pos	  = Code.find(search);
		if (pos != std::wstring::npos)
		{
			cmd::data::SaveMemberCode(MemberId, FilePath, Code);
			ConfirmationFound = true;
		}
		else
		{
			ConfirmationFound = false;
		}
	}
}

float DBAutofillSettings::LoadOption(ESettingType InType)
{
	switch (InType)
	{
	case ESettingType::NormalPressing:
	{
		return cmd::ini::GetFloatValue(DBIniItem::AutoFill_SinglePressingDelay);
	}
	case ESettingType::TypingText:
	{
		return cmd::ini::GetFloatValue(DBIniItem::AutoFill_TypingTextDelay);
	}
	case ESettingType::Hotkeys:
	{
		return cmd::ini::GetFloatValue(DBIniItem::AutoFill_HotkeyDelay);
	}
	case ESettingType::Clipboard:
	{
		return cmd::ini::GetFloatValue(DBIniItem::AutoFill_ClipboardDelay);
	}
	case ESettingType::ImageOpen:
	{
		return cmd::ini::GetFloatValue(DBIniItem::AutoFill_ImageOpen);
	}
	case ESettingType::ImageClose:
	{
		return cmd::ini::GetFloatValue(DBIniItem::AutoFill_ImageClose);
	}
	}
	return 0.0f;
}
