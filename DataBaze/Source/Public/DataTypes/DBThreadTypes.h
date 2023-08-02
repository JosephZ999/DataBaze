#pragma once

class DBThreadUser
{
private:
	bool Enabled = false;

public:
	bool IsThreadRunning() const { return !Enabled; }
	
private:
	friend class DBThreadManager;
};
