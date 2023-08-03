#include "DBAutofill.h"

void DBAutofill::Init(const DBFamilyData& InUserData)
{
}

void DBAutofill::StartFilling()
{
}

void DBAutofill::Tick(float DeltaTime)
{
	if (ActionList.size() == 0 || CurrentActionIndex >= ActionList.size()) return;

	auto Action = ActionList[CurrentActionIndex];
	Action->DoAction();
	Wait(Action->DelaySeconds);
	++CurrentActionIndex;
}

void DBAction_PressButtons::DoAction() {}

void DBAction_Clipboard::DoAction() {}
