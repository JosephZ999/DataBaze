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

private:
	int SelectedFolderId = 1;
	int SelectedMemberId = 1;

protected:
	void SelectFolder(int InId) { SelectedFolderId = InId; }
	void SelectMember(int InId) { SelectedMemberId = InId; }

	void SearchValidFolders();
	int	 GetValidFoldersNum();
	void LoadFiles();
	void AddMember(const DBFamilyData& MemberData);
	void LoadMember(DBFamilyData& OutMemberData);

private:
	bool		 CheckFile(const std::wstring& InFilePath);
	void		 FillFamilyInfo(const DBFamilyData& MemberData, Json::Value& OutValue);
	std::wstring GenerateFileLocation();
	std::wstring GenerateFileLocationById(int InId);

public:
	friend class DBSystem;
};
