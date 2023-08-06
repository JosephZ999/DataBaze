#include "DBSettingsComp.h"
#include <assert.h>

const char* DBSettingsComp::IniFileName = "Settings.ini";

const DBIniItem DBIniItem::AutoFill_SinglePressingDelay = DBIniItem("Autofill", "SinglePressingDelay", "0.04");
const DBIniItem DBIniItem::AutoFill_TypingTextDelay		= DBIniItem("Autofill", "TypingTextDelay", "0");
const DBIniItem DBIniItem::AutoFill_HotkeyDelay			= DBIniItem("Autofill", "HotkeyDelay", "0.04");
const DBIniItem DBIniItem::AutoFill_ClipboardDelay		= DBIniItem("Autofill", "ClipboardDelay", "0.04");
const DBIniItem DBIniItem::AutoFill_ImageOpen			= DBIniItem("Autofill", "ImageOpen", "1");
const DBIniItem DBIniItem::AutoFill_ImageClose			= DBIniItem("Autofill", "ImageClose", "0.5");
const DBIniItem DBIniItem::AutoFill_Mail				= DBIniItem("Autofill", "Mail", "NAME@GMAIL.COM");

// clang-format off
const std::vector<DBIniItem> DBSettingsComp::DefaultItems = 
{
	DBIniItem::AutoFill_SinglePressingDelay,
	DBIniItem::AutoFill_TypingTextDelay,
	DBIniItem::AutoFill_HotkeyDelay,
	DBIniItem::AutoFill_ClipboardDelay,
	DBIniItem::AutoFill_ImageOpen,
	DBIniItem::AutoFill_ImageClose,
	DBIniItem::AutoFill_Mail
};
// clang-format on

namespace // ini file functions
{
	void CreateDefaultIni(CSimpleIniA* ini)
	{
		ini->Reset();
		ini->SetUnicode();

		for (auto& Elem : DBSettingsComp::DefaultItems)
		{
			ini->SetValue(Elem.Section.c_str(), Elem.Key.c_str(), Elem.Value.c_str());
		}
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

std::string DBSettingsComp::GetStringValue(const DBIniItem& Key)
{
	return IniFile.GetValue(Key.Section.c_str(), Key.Key.c_str());
}
