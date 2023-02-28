#pragma once

#include "DBInterface.h"
#include "DBDataTypes.h"
#include "DBFunctionLibrary.h"
#include "Components/UIVerticalBox.h"

class DBButtonManager : public DBInterface
{
public:
private:
	std::unique_ptr<UIVerticalBox> VerticalBox;
	// functions
public:
	void Initialize(HWND OwnerWnd, std::vector<FButton>& Buttons);

private:
};
