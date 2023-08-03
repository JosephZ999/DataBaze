#pragma once
#include "DBInterface.h"
#include "SimpleThread/Public/STClient.h"
#include "DBDataTypes.h"
#include <vector>
#include <string>
#include <Windows.h>

class DBAction;

class DBAutofill : public DBInterface, public STClient
{
	typedef std::vector<WORD> buttons;

private:
	bool Enabled = false;
	std::vector<DBAction*> ActionList;
	int					   CurrentActionIndex = 0;
	FMemberId MemberId;

public:
	~DBAutofill();
	void Init(const DBFamilyData& InUserData, FMemberId InId);
	void StartFilling();
	void Clear();

private:
	virtual void Tick(float DeltaTime) override;

	void InitMemberActions(const DBFamilyData& Data);
	void InitSubMemberActions(const DBPeopleData& Data, bool FirstPeople = false);
};

class DBAction abstract
{
public:
	virtual ~DBAction() {}
	float DelaySeconds = 0.f; // delay at the end
public:
	virtual void DoAction() = 0;
};

/*
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
	DBAction_PressButtons(std::vector<WORD> InButtonList, int InTimes = 1)
		: ButtonList(InButtonList)
		, Times(InTimes)
		, Type(Normal)
	{}

	// Typing text
	DBAction_PressButtons(std::string InText)
		: Type(Text)
	{
		StringToButtonList(InText);
	}

	// Hotkeys
	DBAction_PressButtons(WORD FirstKey, WORD SecondKey)
		: Type(Hotkey)
	{
		ButtonList.push_back(FirstKey);
		ButtonList.push_back(SecondKey);
	}

private:
	std::vector<WORD> ButtonList;
	int				  Times = 0;
	EActionType		  Type	= Normal;

public:
	virtual void DoAction() override;

private:
	void StringToButtonList(std::string& InText);
};

/*
/* Copy to Clipboard
*/
class DBAction_Clipboard : public DBAction
{
public:
	DBAction_Clipboard(std::string InText)
		: TextToClip(InText)
	{}

	DBAction_Clipboard(std::wstring InText)
		: WTextToClip(InText)
	{}

private:
	std::string	 TextToClip;
	std::wstring WTextToClip;

public:
	virtual void DoAction() override;
};
