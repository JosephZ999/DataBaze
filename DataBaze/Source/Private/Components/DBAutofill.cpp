#include "DBAutofill.h"
#include "DBKeyCodes.h"
#include "DBFunctionLibrary.h"
#include <map>

static const std::string Mail = std::string("DEVILNOISY999@GMAIL.COM");

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
	InitSubMemberActions(Data.Parents[0]);

	// Mail
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB), 3));
	ActionList.push_back(new DBAction_Clipboard(Data.MailStreet));
	ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB), 3));

	if (Data.MailHomeNumber.size() > 0 && DBConvert::StringToInt(Data.MailHomeNumber) != 0)
	{
		ActionList.push_back(new DBAction_Clipboard(Data.MailHomeNumber));
		ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	}
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));

	ActionList.push_back(new DBAction_Clipboard(Data.MailCity));
	ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));

	ActionList.push_back(new DBAction_Clipboard(Data.MailRegion));
	ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));

	if (Data.IsHasZipCode())
	{
		ActionList.push_back(new DBAction_Clipboard(std::to_string(Data.MailZipCode)));
		ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	}
	else
	{
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_SPACE)));
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));
	}
	ActionList.push_back(new DBAction_PressButtons(Data.MailCountry));
	ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));

	ActionList.push_back(new DBAction_PressButtons(Data.Parents[0].WhereLive));
	ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB), 2));

	// EMail
	ActionList.push_back(new DBAction_PressButtons(Mail));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));
	ActionList.push_back(new DBAction_PressButtons(Mail));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));

	// Education level
	{
		const int EducationLevel = Data.Parents[0].EducationDegree - 1;
		const int SkipCount		 = 10 - EducationLevel;
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB), EducationLevel));
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_SPACE)));
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB), SkipCount));
	}

	// Select meritial status
	{
		const int StatusLevel = Data.MaritalStatus - 1;
		const int SkipCount	  = 6 - StatusLevel;
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB), StatusLevel));
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_SPACE)));
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB), SkipCount));
	}
	ActionList.push_back(new DBAction_Clipboard(std::to_string(Data.ChildrenNum)));
	ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));
}

void DBAutofill::InitSubMemberActions(const DBPeopleData& Data, bool FirstPeople)
{
	ActionList.push_back(new DBAction_Clipboard(Data.FamilyName));
	ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));

	ActionList.push_back(new DBAction_Clipboard(Data.Name));
	ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB), 3));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_SPACE)));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));
	// Gender
	if (Data.IsMele())
	{
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_SPACE)));
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB), 2));
	}
	else
	{
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_SPACE)));
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));
	}
	ActionList.push_back(new DBAction_Clipboard(std::to_string(Data.BirthMonth)));
	ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));

	ActionList.push_back(new DBAction_Clipboard(std::to_string(Data.BirthDay)));
	ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));

	ActionList.push_back(new DBAction_Clipboard(std::to_string(Data.BirthYear)));
	ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));

	// Fill City
	if (Data.IsBirthCityValid())
	{
		ActionList.push_back(new DBAction_Clipboard(Data.BirthCity));
		ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB), 2));
	}
	else
	{
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_SPACE)));
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));
	}

	ActionList.push_back(new DBAction_PressButtons(Data.BirthCountry));
	ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));

	if (FirstPeople)
	{
		ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB), 4));
		ActionList.back()->DelaySeconds = 1.f;
	}

	// Image
	const std::string SImage = Data.ImageFile;
	std::wstring	  WImage;
	DBConvert::StringToWString(SImage, WImage);
	std::wstring FilePath = DBPaths::GetDataFolderPath(MemberId.FolderId).append(WImage);

	ActionList.push_back(new DBAction_Clipboard(FilePath));
	ActionList.push_back(new DBAction_PressButtons(VK_CONTROL, VK_V));
	ActionList.push_back(new DBAction_PressButtons(buttons(VK_RETURN)));
	ActionList.back()->DelaySeconds = 0.5f;

	ActionList.push_back(new DBAction_PressButtons(buttons(VK_TAB)));
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
	case EAutofillStep::SaveResult: break;
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
	InitActionStep(Step);

	CurrentActionIndex = 0;
	Enabled			   = true;
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
		// Finish
		Enabled = false;
	}
	else
	{
		auto Action = ActionList[CurrentActionIndex];
		Action->DoAction();
		Wait(Action->DelaySeconds);
		++CurrentActionIndex;
	}
}

void DBAction_PressButtons::DoAction()
{
	switch (Type)
	{
	case DBAction_PressButtons::Normal:
	{
		break;
	}
	case DBAction_PressButtons::Text:
	{
		break;
	}
	case DBAction_PressButtons::Hotkey:
	{
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

void DBAction_Clipboard::DoAction() {}
