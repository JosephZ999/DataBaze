#pragma once
#include "SimpleThread/STDataTypes.h"
#include <memory>

class STClient;

class STManager
{
private:
	STManager();

public:
	~STManager();
	static std::weak_ptr<STManager> Instance;

private:
	std::vector<STClient*> Clients;

public:
	static std::shared_ptr<STManager> GetInstance();

	// For Clients
	void BindClient(STClient* Client); // Bind to Thread's update
	void UnBindClient(STClient* Client);

private:
	void StartThread();
	void ThreadUpdate(float Delta);

private:
	friend class STUpdater;
};

