#pragma once

class STClient
{
public:
	STClient();
	virtual ~STClient();

private:
	float WaitTime = 0.f;

public:
	// Interface
	virtual void Tick(float DeltaTime) = 0;
	void Wait(float Seconds);

private:
	void OnUpdate(float DeltaTime);
	void BindToThread();

private:
	friend class STManager;
};
