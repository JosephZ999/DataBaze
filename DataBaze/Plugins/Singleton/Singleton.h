#pragma once
#include <typeinfo>
#include <vector>
#include <type_traits>

class SingletonManager;

class Singleton abstract
{
	size_t Id = 0;

protected:
	size_t GetId() { return Id; }
	virtual ~Singleton() {}

public:
	friend SingletonManager;
};

// Factory
class SingletonManager
{
private:
	static std::vector<Singleton*> Objects;

public:
	template <typename T> //
	static T* Initialize()
	{
		// get if exist
		const auto ClassId = typeid(T).hash_code();
		for (auto Obj : Objects)
		{
			if (Obj->Id == ClassId)
			{
				return static_cast<T*>(Obj);
			}
		}

		// create if not exist
		if (std::is_base_of_v<Singleton, T>)
		{
			auto TRef		= new T;
			auto SingletonT = static_cast<Singleton*>(TRef);
			SingletonT->Id	= ClassId;
			Objects.push_back(SingletonT);
			return TRef;
		}
		return nullptr;
	}

	template <typename T> //
	static T* Get()
	{
		// get if exist
		const auto ClassId = typeid(T).hash_code();
		for (auto Obj : Objects)
		{
			if (Obj->Id == ClassId)
			{
				return static_cast<T*>(Obj);
			}
		}
		return nullptr;
	}

	static bool DestroyAll()
	{
		for (auto Obj : Objects)
		{
			delete Obj;
		}
		return false;
	}
};
