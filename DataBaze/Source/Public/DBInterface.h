#pragma once

class DBInterface
{
private:
	DBInterface* Owner = nullptr;

public:
	DBInterface*		 GetOwner();
	void				 SetOwner(DBInterface* NewOwner);
	virtual DBInterface* GetSystem();
};
