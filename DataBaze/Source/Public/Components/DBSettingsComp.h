#pragma once

#include "DBInterface.h"
#include "Simpleini/SimpleIni.h"
#include "DBSettingsTypes.h"
#include <vector>
#include <string>

class DBSettingsComp : public DBInterface
{
public:
	DBSettingsComp();
	static const char*					IniFileName;
	static const std::vector<DBIniItem> DefaultItems;

private:
	CSimpleIniA IniFile;

public:
	int	  GetIntValue(const DBIniItem& Key);
	bool  GetBoolValue(const DBIniItem& Key);
	float GetFloatValue(const DBIniItem& Key);
	std::string GetStringValue(const DBIniItem& Key);
};
