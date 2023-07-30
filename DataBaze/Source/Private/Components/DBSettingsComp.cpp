#include "DBSettingsComp.h"
#include <assert.h>

const char* DBSettingsComp::IniFileName = "Settings.ini";

namespace // ini file functions
{
	void CreateDefaultIni(CSimpleIniA* ini)
	{
		ini->Reset();
		ini->SetUnicode();
		ini->SetValue("Autofill", "FillDelay", "10.0");
		ini->SaveFile(DBSettingsComp::IniFileName);
	}

	bool LoadIniFile(CSimpleIniA* ini)
	{
		if (ini)
		{
			if (ini->LoadFile(DBSettingsComp::IniFileName) != 0)
			{
				CreateDefaultIni(ini);
			}
			return true;
		}
		assert(false && "LoadIniFile - invalid function param");
		return false;
	}

} // namespace

DBSettingsComp::DBSettingsComp()
{
	typedef std::list<CSimpleIniA::Entry> TNamesDepend;

	IniFile.SetUnicode();
	if (LoadIniFile(&IniFile)) return;
}

int DBSettingsComp::GetIntValue(const char* Section, const char* Key)
{
	auto LongValue = IniFile.GetLongValue(Section, Key);
	return static_cast<int>(LongValue);
}

bool DBSettingsComp::GetBoolValue(const char* Section, const char* Key)
{
	return IniFile.GetBoolValue(Section, Key);
}

float DBSettingsComp::GetFloatValue(const char* Section, const char* Key)
{
	auto DoubleValue = IniFile.GetDoubleValue(Section, Key);
	return static_cast<float>(DoubleValue);
}
