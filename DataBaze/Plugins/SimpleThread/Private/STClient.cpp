#include "Plugins/SimpleThread/Public/STClient.h"
#include "Plugins/SimpleThread/Public/STManager.h"
#include <Windows.h>

STClient::STClient()
{
	BindToThread();
}

void STClient::BindToThread()
{
	auto ThreadManager = STManager::GetInstance();
	if (ThreadManager)
	{
		ThreadManager->BindClient(this);
	}
}

STClient::~STClient()
{
	auto ThreadManager = STManager::GetInstance();
	if (ThreadManager)
	{
		ThreadManager->UnBindClient(this);
	}
}

void STClient::OnUpdate(float DeltaTime)
{
	if (WaitTime > 0.f)
	{
		WaitTime -= DeltaTime;
	}

	if (WaitTime <= 0.f)
	{
		Tick(DeltaTime);
	}
}

void STClient::Wait(float Seconds)
{
	WaitTime += Seconds;
}

void STClient::Tick(float DeltaTime)
{
	// char msg[100];
	// sprintf_s(msg, "Wait 2 seconds, delta %f \n", DeltaTime);
	// OutputDebugStringA(msg);
	// Wait(2.f);
}
