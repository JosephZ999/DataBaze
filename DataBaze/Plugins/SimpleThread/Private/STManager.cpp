#include "SimpleThread/Public/STManager.h"
#include "SimpleThread/Public/STClient.h"
#include <Windows.h>
#include <chrono>
// #include <string>

class STUpdater* UpdaterInstance = nullptr;

std::weak_ptr<STManager> STManager::Instance;

//-----------------------------------------------------------------//
class STUpdater final
{
private:
	STUpdater() {}

public:
	void Update(std::weak_ptr<STManager> Manager, float Delta)
	{
		if (! Manager.expired())
		{
			Manager.lock()->ThreadUpdate(Delta);
		}
	}

	friend class STManager;
};

// Thread Body //--------------------------------------------------//
DWORD WINAPI FirstThread(LPVOID Param)
{
	std::weak_ptr<STManager> Manager	  = STManager::GetInstance();
	float					 DeltaSeconds = 0.f;
	while (! Manager.expired())
	{
		auto start_time = std::chrono::high_resolution_clock::now(); // get the current time

		Sleep(10);
		if (UpdaterInstance)
		{
			UpdaterInstance->Update(Manager, DeltaSeconds);
		}
		else
		{
			// assertion
			return 0;
		}
		auto end_time = std::chrono::high_resolution_clock::now(); // get the current time again
		auto delta_time =
			std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time); // calculate the difference in seconds
		DeltaSeconds = static_cast<float>(delta_time.count()) * 0.000001f;

		/* char msg[100];
		 sprintf_s(msg, "Time is - %f \n", DeltaSeconds);
		 OutputDebugStringA(msg);*/
	}
	return 0;
}
//=================================================================//

STManager::STManager()
{
	UpdaterInstance = new STUpdater;
	if (UpdaterInstance)
	{
		StartThread();
	}
}

STManager::~STManager()
{
	if (UpdaterInstance)
	{
		delete UpdaterInstance;
		UpdaterInstance = nullptr;
	}
}

std::shared_ptr<STManager> STManager::GetInstance()
{
	if (Instance.expired())
	{
		auto Ins = std::shared_ptr<STManager>(new STManager);
		Instance = Ins;
		return Ins;
	}
	return Instance.lock();
}

void STManager::BindClient(STClient* Client)
{
	if (! Client) return;

	bool ClientAlradyBound = false;
	for (size_t i = 0; i < Clients.size(); ++i)
	{
		if (Client == Clients[i])
		{
			ClientAlradyBound = true;
		}
	}

	if (! ClientAlradyBound)
	{
		Clients.push_back(Client);
	}
}

void STManager::UnBindClient(STClient* Client)
{
	for (size_t i = 0; i < Clients.size(); ++i)
	{
		if (Clients[i] == Client)
		{
			Clients.erase(Clients.begin() + i);
		}
	}
}

void STManager::StartThread()
{
	HANDLE NewThread = CreateThread(NULL, 0, FirstThread, NULL, 0, NULL);
}

void STManager::ThreadUpdate(float Delta)
{
	for (auto& Elem : Clients)
	{
		Elem->OnUpdate(Delta);
	}
}
