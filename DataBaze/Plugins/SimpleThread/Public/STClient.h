#pragma once

class STClient
{
public:
	STClient();
	virtual ~STClient();

private:
	float WaitTime = 0.f;

private:
	void OnUpdate(float DeltaTime);

public:
	virtual void Tick(float DeltaTime);
	virtual void BindToThread();

	void Wait(float Seconds);

private:
	friend class STManager;
};
