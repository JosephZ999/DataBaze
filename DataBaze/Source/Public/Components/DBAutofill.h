#pragma once
#include "DBInterface.h"
#include "DBThreadTypes.h"

class DBAutofill : public DBInterface, public DBThreadUser
{
	DBAutofill();
};
