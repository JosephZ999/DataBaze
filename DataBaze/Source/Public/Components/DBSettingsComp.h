#pragma once

#include "DBInterface.h"
#include "Simpleini/SimpleIni.h"
#include <vector>
#include <string>

struct DBIniItem
{
	DBIniItem(std::string InSection, std::string InKey, std::string InValue)
		: Section(InSection)
		, Key(InKey)
		, Value(InValue)
	{}

	std::string Section;
	std::string Key;
	std::string Value;

	static const DBIniItem AutoFill_Delay;
	static const DBIniItem AutoFill_Other;
};

class DBSettingsComp : public DBInterface
{
public:
	DBSettingsComp();
	static const char*					IniFileName;
	static const std::vector<DBIniItem> DefaultItems;

private:
	CSimpleIniA IniFile;

public:
	int	  GetIntValue(DBIniItem Key);
	bool  GetBoolValue(DBIniItem Key);
	float GetFloatValue(DBIniItem Key);
};
