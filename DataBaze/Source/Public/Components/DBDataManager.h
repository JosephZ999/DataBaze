#pragma once
#include "DBInterface.h"
#include "DBFunctionLibrary.h"
#include <vector>

#include "json/reader.h"
#include "json/writer.h"

#define MAX_FOLDERS_NUM 20
#define MAX_MEMBERS_NUM 500

// #include "DBDataTypes.h"

DECLARE_DELEGATE(OnUpdateSignature);

class DBDataManager : public DBInterface
{
	typedef std::vector<int> Folders;

public:
	OnUpdateSignature OnUpdate;

private:
	int		SelectedFolderId = 1;
	int		SelectedMemberId = 1;
	Folders ValidFolders[MAX_FOLDERS_NUM];

public:
	void SelectMember(int InMemberId);
	bool ChangeFolder(bool bNext);

	bool SearchValidFolders();
	int	 GetValidFoldersNum();
	void LoadFiles();
	void AddMember(const DBFamilyData& MemberData);
	void LoadMember(DBFamilyData& OutMemberData);
	void GetMembersList(std::vector<std::wstring>& OutList);

private:
	bool SetFolder(int FolderId);

	bool		 CheckFile(const std::wstring& InFilePath);
	void		 FillFamilyInfo(const DBFamilyData& MemberData, Json::Value& OutValue);
	std::wstring GenerateFileLocation(bool CreateFolder = true);
	std::wstring GenerateFileLocationById(int InId, bool CreateFolder = false);
	std::wstring GetMemberStatus(Json::Value& InData, int InId);
};
