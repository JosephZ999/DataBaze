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

	static const DBIniItem AutoFill_Delay;
	static const DBIniItem AutoFill_Other;
};
