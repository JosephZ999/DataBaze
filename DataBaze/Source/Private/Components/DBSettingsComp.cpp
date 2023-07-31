#include "DBSettingsComp.h"
#include <assert.h>

const char*					 DBSettingsComp::IniFileName = "Settings.ini";
const std::vector<DBIniItem> DBSettingsComp::DefaultItems =
	{
		DBIniItem::AutoFill_Delay,
		DBIniItem::AutoFill_Other
};

namespace // ini file functions
{
	void CreateDefaultIni(CSimpleIniA * ini)
	{
		ini->Reset();
		ini->SetUnicode();

		for (auto& Elem : DBSettingsComp::DefaultItems)
		{
			ini->SetValue(Elem.Section.c_str(), Elem.Key.c_str(), Elem.Value.c_str());
		}
		ini->SaveFile(DBSettingsComp::IniFileName);
	}

	bool LoadIniFile(CSimpleIniA * ini)
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

int DBSettingsComp::GetIntValue(const DBIniItem& Key)
{
	auto LongValue = IniFile.GetLongValue(Key.Section.c_str(), Key.Key.c_str());
	return static_cast<int>(LongValue);
}

bool DBSettingsComp::GetBoolValue(const DBIniItem& Key)
{
	return IniFile.GetBoolValue(Key.Section.c_str(), Key.Key.c_str());
}

float DBSettingsComp::GetFloatValue(const DBIniItem& Key)
{
	auto DoubleValue = IniFile.GetDoubleValue(Key.Section.c_str(), Key.Key.c_str());
	return static_cast<float>(DoubleValue);
}
