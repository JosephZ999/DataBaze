#pragma once
#include "DBInterface.h"
#include "Plugins/SimpleThread/Public/STClient.h"

class DBAutofill : public DBInterface, public STClient
{

};
