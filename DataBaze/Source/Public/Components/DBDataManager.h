#pragma once
#include "DBInterface.h"
#include "DBFunctionLibrary.h"

#include "json/reader.h"
#include "json/writer.h"
// #include "DBDataTypes.h"

DECLARE_DELEGATE(OnUpdateSignature);

class DBDataManager : public DBInterface
{
public:
	OnUpdateSignature OnUpdate;


public:
	void LoadFiles();
	void AddMember(const DBFamilyData& MemberData);

private:
	void CheckFiles();
	bool CheckFile(const std::wstring& InFilePath);
	void FillFamilyInfo(const DBFamilyData& MemberData, Json::Value& OutValue);
	std::wstring GenerateFileLocation();
};
