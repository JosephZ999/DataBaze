#pragma once

#include "DBInterface.h"
#include "Simpleini/SimpleIni.h"

class DBSettingsComp : public DBInterface
{
public:
	DBSettingsComp();
	static const char* IniFileName;

private:
	CSimpleIniA IniFile;

public:
	int	  GetIntValue(const char* Section, const char* Key);
	bool  GetBoolValue(const char* Section, const char* Key);
	float GetFloatValue(const char* Section, const char* Key);
};
