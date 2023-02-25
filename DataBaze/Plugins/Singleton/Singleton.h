#pragma once
#include <typeinfo>

class Singleton
{
protected:
	static Singleton* _self;
	Singleton() {}
	virtual ~Singleton() { DeleteInstance(); }

public:
	static Singleton* Instance()
	{
		if (! _self)
		{
			_self = new Singleton();
		}
		return _self;
	}

	template <typename T> //
	static T* Get()
	{
		return dynamic_cast<T*>(Instance());
	}

	static bool DeleteInstance()
	{
		if (_self)
		{
			delete _self;
			_self = nullptr;
			return true;
		}
		return false;
	}
};

Singleton* Singleton ::_self = nullptr;

class SingletonFactory
{
public:
	template <typename T> //
	static T* Get()
	{

	}
};
