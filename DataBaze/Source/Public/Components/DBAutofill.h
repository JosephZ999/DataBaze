#pragma once
#include "DBInterface.h"
#include "SimpleThread/Public/STClient.h"

class DBAutofill : public DBInterface, public STClient
{
private:
	virtual void Tick(float DeltaTime) override;
};
