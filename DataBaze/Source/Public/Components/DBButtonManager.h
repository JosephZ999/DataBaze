#pragma once

#include "DBInterface.h"
#include "DBDataTypes.h"
#include "DBFunctionLibrary.h"

class DBButtonManager : public DBInterface
{
public:
private:
	// functions
public:
	void Initialize(HWND OwnerWnd, std::vector<FButton>& Buttons);

private:
};
