#include "DBDataManager.h"
#include <iostream>
#include <fstream>

void DBDataManager::LoadFiles()
{
	const std::wstring FileName = GenerateFileLocation();

	DBFamilyData Family;

	DBPeopleData People;
	People.Name		  = L"Doston";
	People.FamilyName = L"Hamdamov";
	Family.Parents.push_back(People);
	Family.Parents.push_back(People);

	Family.Children.push_back(People);
	Family.Children.push_back(People);
	Family.Children.push_back(People);

	AddMember(Family);

	OnUpdate.Broadcast();
}

void DBDataManager::AddMember(const DBFamilyData& MemberData)
{
	const auto FileName = GenerateFileLocation();

	Json::Reader FileReader;
	Json::Value	 FileData;

	// Read Json file
	if (CheckFile(FileName))
	{
		// Read file data
		std::ifstream File(FileName);
		FileReader.parse(File, FileData);
		File.close();

		// Add New Member
		Json::Value Family;
		FillFamilyInfo(MemberData, Family);
		FileData["Main"].append(Family);

		// Save to file
		std::ofstream FileStream(FileName);

		Json::StreamWriterBuilder			Builder;
		std::unique_ptr<Json::StreamWriter> Writer(Builder.newStreamWriter());
		Writer->write(FileData, &FileStream);
		FileStream.close();
	}
	else
	{
		std::ofstream FileStream(FileName);

		// Add New Member
		Json::Value Family;
		FillFamilyInfo(MemberData, Family);
		FileData["Main"].append(Family);

		Json::StreamWriterBuilder			Builder;
		std::unique_ptr<Json::StreamWriter> Writer(Builder.newStreamWriter());
		Writer->write(FileData, &FileStream);
		FileStream.close();
	}
}

void DBDataManager::LoadMember(DBFamilyData& OutMemberData)
{
	//
}

bool DBDataManager::SearchValidFolders()
{
	bool SomeFileWasFound = false;
	for (size_t i = 1; i <= MAX_FOLDERS_NUM; ++i)
	{
		const std::wstring FilePath = GenerateFileLocationById(i, true);
		std::ifstream	   File(FilePath);
		if (File.good())
		{
			SomeFileWasFound = true;
			ValidFolders->push_back(i);
		}
		File.close();
	}

#ifdef _DEBUG
	std::wstring Message;
	Message.append(L"Valid Folders Num : ").append(std::to_wstring(ValidFolders->size()));
	MessageBox(NULL, Message.c_str(), L"Func: SearchValidFolders", MB_OK);

	for (auto& Elem : *ValidFolders)
	{
		std::wstring Message;
		Message.append(L"Valid Folder : ").append(std::to_wstring(Elem));
		MessageBox(NULL, Message.c_str(), L"Func: SearchValidFolders", MB_OK);
	}
#endif
	return SomeFileWasFound;
}

int DBDataManager::GetValidFoldersNum()
{
	return 0;
}

bool DBDataManager::CheckFile(const std::wstring& InFilePath)
{
	return std::ifstream(InFilePath).good();
}

void DBDataManager::FillFamilyInfo(const DBFamilyData& MemberData, Json::Value& OutValue)
{
	const bool HasChildren = MemberData.IsHasChildren();
	const bool HasSpouse   = MemberData.IsHasASpouse();

	Json::Value FamilyMember;
	Json::Value MemberItem;

	std::string StringData;
	DBConvert::WStringToString(MemberData.Parents[0].Name, StringData);
	MemberItem["Name"] = Json::Value(StringData);

	DBConvert::WStringToString(MemberData.Parents[0].FamilyName, StringData);
	MemberItem["FamilyName"] = Json::Value(StringData);

	FamilyMember["Member 1"] = MemberItem;
	FamilyMember["Member 2"] = MemberItem;
	OutValue				 = FamilyMember;
}

std::wstring DBDataManager::GenerateFileLocation(bool CreateFolder)
{
	return GenerateFileLocationById(SelectedFolderId, CreateFolder);
}

std::wstring DBDataManager::GenerateFileLocationById(int InId, bool CreateFolder)
{
	auto Folder = DBPaths::GetProjectPath().append(L"\\Data");
	_wmkdir(Folder.c_str());

	Folder.append(L"\\Folder_").append(std::to_wstring(InId));
	if (CreateFolder)
	{
		_wmkdir(Folder.c_str());
	}
	return Folder.append(L"\\data.json");
}
