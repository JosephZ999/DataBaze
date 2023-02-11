#include "DBDataManager.h"
#include <iostream>
#include <fstream>

void DBDataManager::LoadFiles()
{

	const std::wstring FileName = GenerateFileLocation();
	{
		// Try to write array as json file
		// Generate json file name

		// Struct
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
		return;

		std::string FamilyData;
		DBConvert::WStringToString(Family.ToWString(), FamilyData);

		Json::Reader Reader;
		Json::Value	 RootValue;

		Reader.parse(FamilyData, RootValue);

		// Save Json
		Json::StreamWriterBuilder			Builder;
		std::unique_ptr<Json::StreamWriter> Writer(Builder.newStreamWriter());

		std::ofstream File(FileName);
		Writer->write(RootValue, &File);

		File.close();
	}

	// Json Read
	{
		// Try to read Json
		std::ifstream File(FileName);

		Json::Reader FileReader;
		Json::Value	 FileData;

		// parsing
		if (FileReader.parse(File, FileData))
		{
			std::wstring Line;
			DBConvert::StringToWString(FileData["Main"][0]["Family 1"][0]["List 1"]["Item 1"].asString(), Line);
			MessageBox(NULL, Line.c_str(), L"Dialog Box", MB_OK);
		}

		// Adding new data
		{
			//

			// Json::Value Main(Json::arrayValue);

			Json::Value Item(Json::arrayValue);
			Json::Value Item2;
			Json::Value Item3;

			Item3["Elem 1"] = Json::Value(0);
			Item3["Elem 2"] = Json::Value(1);

			Item.append(Item3);
			Item.append(Json::Value(2));
			Item.append(Json::Value(3));

			Item2["Text"] = Item;
			FileData["Main"].append(Item2);

			// Save Json
			Json::StreamWriterBuilder			Builder;
			std::unique_ptr<Json::StreamWriter> Writer(Builder.newStreamWriter());

			std::ofstream Save(FileName);
			Writer->write(FileData["Main"], &Save);

			Save.close();
		}
	}

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

		std::wstring Message = std::to_wstring(FileData["Main"].size());
		MessageBox(NULL, Message.c_str(), L"Dialog Box", MB_OK);

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

		std::wstring Message = std::to_wstring(FileData["Main"].size());
		MessageBox(NULL, Message.c_str(), L"Dialog Box", MB_OK);

		Json::StreamWriterBuilder			Builder;
		std::unique_ptr<Json::StreamWriter> Writer(Builder.newStreamWriter());
		Writer->write(FileData, &FileStream);
		FileStream.close();
	}
}

void DBDataManager::CheckFiles()
{
	const std::wstring ProjectPath = DBPaths::GetProjectPath();

	for (size_t i = 1; i <= 20; ++i)
	{
		std::wstring FilePath = ProjectPath;

		char str[256];
		sprintf_s(str, sizeof(str), "\\Folder_%i\\", i);

		FilePath.append(std::wstring(&str[0], &str[strlen(str)])).append(L"data.json");

		std::ifstream file(FilePath);

		// json reader
		Json::Reader reader;

		// this will contain complete JSON data
		Json::Value completeJsonData;

		// reader reads the data and stores it in completeJsonData
		if (reader.parse(file, completeJsonData))
		{
			// get value from index
			const auto Data = completeJsonData["name"].asCString();

			// convert value to wstring
			const std::wstring a(&Data[0], &Data[strlen(Data)]);

			MessageBox(NULL, a.c_str(), L"Dialog Box", MB_OK);
		}
		// MessageBox(NULL, FilePath.c_str(), L"Dialog Box", MB_OK);
	}
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

std::wstring DBDataManager::GenerateFileLocation()
{
	/*char str[256];
	sprintf_s(str, sizeof(str), "\\Folder_%i\\", FolderIndex);

	std::wstring FilePath = ProjectPath;
	FilePath.append(std::wstring(&str[0], &str[strlen(str)])).append(L"data.json");*/

	return DBPaths::GetProjectPath().append(L"\\ExampleArray.json");
}
