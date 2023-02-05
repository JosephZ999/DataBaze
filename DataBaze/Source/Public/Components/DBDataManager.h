#pragma once
#include "DBInterface.h"
#include "DBFunctionLibrary.h"

DECLARE_DELEGATE(OnUpdateSignature);

class DBDataManager : public DBInterface
{
public:
	OnUpdateSignature OnUpdate;

public:
	void LoadFiles();
};
