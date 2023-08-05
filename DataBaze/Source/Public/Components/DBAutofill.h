#pragma once
#include "DBInterface.h"
#include "SimpleThread/Public/STClient.h"
#include "DBDataTypes.h"
#include <vector>
#include <string>
#include <Windows.h>
#include "DBDelegate.h"

DECLARE_DELEGATE(OnFinishFillSignature);

class DBAction;

enum class EAutofillStep
{
	None,
	Page1,
	Page2,
	SaveResult,
	Max,
};

/*-----------------------------------------------------------------//
 *
 */
class DBAutofill : public DBInterface, public STClient
{
public:
	OnFinishFillSignature OnFinish;

private:
	bool				   Enabled = false;
	std::vector<DBAction*> ActionList;
	int					   CurrentActionIndex = 0;
	DBFamilyData		   UserData;
	FMemberId			   MemberId;
	EAutofillStep		   CurrentStep = EAutofillStep::None;
	HWND				   OwnerWindow;

public:
	~DBAutofill();
	void Init(const DBFamilyData& InUserData, FMemberId InId, HWND InOwnerWindow);
	void StartFilling(EAutofillStep Step = EAutofillStep::None);
	void Clear();

private:
	virtual void Tick(float DeltaTime) override;

	void InitMemberActions(const DBFamilyData& Data);
	void InitSubMemberActions(const DBPeopleData& Data, bool FirstPeople = false);
	void InitActionStep(EAutofillStep Step);
	void InitSaveResult();
};

/*-----------------------------------------------------------------//
/*
*/
class DBAction abstract
{
public:
	virtual ~DBAction() {}
	float DelaySeconds = 0.f; // delay at the end
public:
	virtual void DoAction() = 0;
};

/*-----------------------------------------------------------------//
/* Press hotkey or type a text
*/
class DBAction_PressButtons : public DBAction
{
	enum EActionType
	{
		Normal,
		Text,
		Hotkey,
	};

public:
	// Simple pressing keys
	DBAction_PressButtons(int InButton, int InTimes = 1)
		: Times(InTimes)
		, Type(Normal)
	{
		ButtonList.push_back(InButton);
		DelaySeconds = 0.05f;
	}

	// Typing text
	DBAction_PressButtons(std::string InText)
		: Type(Text)
	{
		StringToButtonList(InText);
	}

	// Hotkeys
	DBAction_PressButtons(const std::vector<int>& InButtons)
		: Type(Hotkey)
		, ButtonList(InButtons)
	{
		DelaySeconds = 0.05f;
	}

private:
	std::vector<int> ButtonList;
	int				 Times = 0;
	EActionType		 Type  = Normal;

public:
	virtual void DoAction() override;

private:
	void StringToButtonList(std::string& InText);
};

/*-----------------------------------------------------------------//
/* Email writer
*/
class DBAction_WriteMail : public DBAction
{
public:
	virtual void DoAction() override;

private:
	void WriteString(std::string& Text);
};

/*-----------------------------------------------------------------//
/* Copy to Clipboard
*/
class DBAction_Clipboard : public DBAction
{
public:
	DBAction_Clipboard(std::string InText, HWND OuterWindow)
		: TextToClip(InText)
		, WindowHandle(OuterWindow)
	{
		DelaySeconds = 0.05f;
	}

	DBAction_Clipboard(std::wstring InText, HWND OuterWindow)
		: WTextToClip(InText)
		, WindowHandle(OuterWindow)
	{
		DelaySeconds = 0.05f;
	}

private:
	HWND		 WindowHandle;
	std::string	 TextToClip;
	std::wstring WTextToClip;

public:
	virtual void DoAction() override;
};

class DBAction_SaveToFile : public DBAction
{
public:
	DBAction_SaveToFile(std::wstring InFilePath, FMemberId InMemberId)
		: FilePath(InFilePath)
		, MemberId(InMemberId)
	{}

private:
	std::wstring FilePath;
	FMemberId	 MemberId;

public:
	virtual void DoAction() override;
};
