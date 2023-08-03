#pragma once
#include "DBInterface.h"
#include "SimpleThread/Public/STClient.h"
#include "DBDataTypes.h"
#include <vector>

class DBAction;

class DBAutofill : public DBInterface, public STClient
{
private:
	std::vector<DBAction*> ActionList;
	int CurrentActionIndex = 0;

public:
	void Init(const DBFamilyData& InUserData);
	void StartFilling();

private:
	virtual void Tick(float DeltaTime) override;
};

class DBAction
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
public:
	virtual void DoAction() override;
};

/*
/* Copy to or Paste Clipboard
*/
class DBAction_Clipboard : public DBAction
{
public:
	virtual void DoAction() override;
};