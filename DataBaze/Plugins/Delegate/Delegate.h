#pragma once

class IDelegate
{
public:
	virtual ~IDelegate(){}
	virtual void Call() {}
};

template <class T> //
class Delegate : public IDelegate
{
	typedef void (T::*Callback)();

private:
	T* Object;

public:
	explicit Delegate(T* InObject, void (T::*InFunc)())
		: Object(InObject)
		, CallbackFunction(InFunc)

	{
	}

	virtual ~Delegate() override
	{
		// MessageBox(NULL, L"Destroy delegate", L"Dialog Box", MB_OK);
	}

	virtual void Call() override { (Object->*CallbackFunction)(); }

private:
	Callback CallbackFunction;
};

class DBDelegateContainer
{
private:
	IDelegate* DelegateObj;

public:
	~DBDelegateContainer()
	{
		if (DelegateObj)
		{
			delete DelegateObj;
			DelegateObj = nullptr;
		}
	}

	template <typename T> //
	void Bind(T* Object, void (T::*InFunc)())
	{
		if (DelegateObj)
		{
			delete DelegateObj;
			DelegateObj = nullptr;
		}
		DelegateObj = new Delegate<T>(Object, InFunc);
	}

	void Broadcast()
	{
		if (! DelegateObj) return;

		DelegateObj->Call();
	}
};

// clang-format off
#define DECLARE_DELEGATE(DelegateName) class DelegateName : public DBDelegateContainer {}
// clang-format on
