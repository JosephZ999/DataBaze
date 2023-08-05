#include "SimpleThread/Public/STClient.h"
#include "SimpleThread/Public/STManager.h"
#include <Windows.h>
#include "DBMacroUtils.h"

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
		WaitTime = 0.f;
		Tick(DeltaTime);
	}
}

void STClient::Wait(float Seconds)
{
	WaitTime = Seconds;
}