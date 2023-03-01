#include "DBDataTypes.h"
#include "DBJsonUtils.h"

void FWindowContainer::Add(const DBWindow& InButton)
{
	if (LastIndex < 10)
	{
		Windows[LastIndex] = InButton;
		++LastIndex;
		return;
	}
}

bool FWindowContainer::FindByIndex(EDBWinCompId Id, DBWindow& OutWindow) const
{
	for (auto& Elem : Windows)
	{
		if (Elem.Id == Id)
		{
			OutWindow = Elem;
			return true;
		}
	}
	return false;
}

bool FWindowContainer::Contains(EDBWinCompId Id) const
{
	for (auto& Elem : Windows)
	{
		if (Elem.Id == Id)
		{
			return true;
		}
	}
	return false;
}

bool DBFamilyData::IsHasChildren() const
{
	return Children.size() > 0;
}

bool DBFamilyData::IsHasASpouse() const
{
	return Parents.size() > 1;
}

EMeritialStatus DBFamilyData::GetStatus() const
{
	return EMeritialStatus(static_cast<EMeritialStatus>(MaritalStatus));
}

void DBFamilyData::SwitchParents()
{
	auto Cache = Parents[0];
	Parents[0] = Parents[1];
	Parents[1] = Cache;
}

EEducationDegree DBPeopleData::GetEducation() const
{
	return static_cast<EEducationDegree>(EducationDegree);
}

std::string DBPeopleData::GetBirthDataAsString() const
{
	using namespace std;
	return string(to_string(BirthDay)).append(".").append(to_string(BirthMonth)).append(".").append(to_string(BirthYear));
}

std::string DBPeopleData::GetGenderAsString() const
{
	return (Gender == 1) ? "Mele" : "Femele";
}

std::string DBPeopleData::GetEducationAsString() const
{
	std::string OutValue;
	switch (static_cast<EEducationDegree>(EducationDegree))
	{ // clang-format off
	case ED_HighSchoolNoDegree:			OutValue = "High school no degree";		break;
	case ED_HighSchoolDegree:			OutValue = "High school";				break;
	case ED_vocationalSchool:			OutValue = "Vocational school";			break;
	case ED_SomeUniversityCourses:		OutValue = "Some univer courses";		break;
	case ED_UniversityDegree:			OutValue = "University degree";			break;
	case ED_SomeGraduatelevelCourses:	OutValue = "Graduate level courses";	break;
	case ED_MastersDegree:				OutValue = "Masters degree";			break;
	case ED_SomeDoctoralCourses:		OutValue = "Doctoral courses";			break;
	case ED_Doctorate:					OutValue = "Doctorate";					break;
	} // clang-format on
	return OutValue;
}

std::string DBPeopleData::GetAsString(EPeopleData DataType)
{
	using namespace std;
	switch (DataType)
	{
	case PD_Name: return Name;
	case PD_FamilyName: return FamilyName;
	case PD_Gender: return to_string(Gender);
	case PD_BirthMonth: return to_string(BirthMonth);
	case PD_BirthDay: return to_string(BirthDay);
	case PD_BirthYear: return to_string(BirthYear);
	case PD_BornCountry: return BirthCountry;
	case PD_EducationDegree: return to_string(EducationDegree);
	case PD_WhereLive: return WhereLive;
	}

	return std::string();
}
