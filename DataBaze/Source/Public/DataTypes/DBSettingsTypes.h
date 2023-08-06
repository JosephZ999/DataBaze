#pragma once

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

	static const DBIniItem AutoFill_SinglePressingDelay;
	static const DBIniItem AutoFill_TypingTextDelay;
	static const DBIniItem AutoFill_HotkeyDelay;
	static const DBIniItem AutoFill_ClipboardDelay;
	static const DBIniItem AutoFill_ImageOpen;
	static const DBIniItem AutoFill_ImageClose;
	static const DBIniItem AutoFill_Mail;
};
