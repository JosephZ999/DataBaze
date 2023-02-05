#include "DBDataManager.h"
#include "json/json.h"

#include <iostream>
#include <fstream>

using namespace std;

void DBDataManager::LoadFiles()
{
	const wstring ProjectPath = DBPaths::GetProjectPath();

	for (size_t i = 1; i <= 20; ++i)
	{
		wstring FilePath = ProjectPath;

		char str[256];
		sprintf_s(str, sizeof(str), "\\Folder_%i\\", i);

		FilePath.append(wstring(&str[0], &str[strlen(str)])).append(L"data.json");

		ifstream file(FilePath);

		// json reader
		Json::Reader reader;

		// this will contain complete JSON data
		Json::Value completeJsonData;

		// reader reads the data and stores it in completeJsonData
		if (reader.parse(file, completeJsonData))
		{
			auto	Data = completeJsonData["name"].asCString();
			wstring a(&Data[0], &Data[strlen(Data)]);
			MessageBox(NULL, a.c_str(), L"Dialog Box", MB_OK);
		}
		// MessageBox(NULL, FilePath.c_str(), L"Dialog Box", MB_OK);
	}

	// complete JSON data
	// cout << "Complete JSON data : " << endl << completeJsGrade << endl;

	// get the value associated with name key
	// cout << "Name : " << completeJsonData["name"] << endl;

	// get the value associated with grade key
	// cout << "Grade : " << completeJsonData["grade"] << endl;

	OnUpdate.Broadcast();
}
