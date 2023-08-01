#pragma once

#include <functional>

template <typename ReturnType, typename... Args> //
class TFunction
{
public:
	using FunctionType = std::function<ReturnType(Args...)>;

	TFunction()
		: func(nullptr)
	{}

	explicit TFunction(FunctionType func)
		: func(func)
	{}

	void setFunction(FunctionType func) { this->func = func; }

	ReturnType operator()(Args... args)
	{
		if (func)
		{
			return func(args...);
		}
		else
		{
			// Можно выбрать другой способ обработки, если функция не установлена.
			// Например, выбросить исключение или вернуть значение по умолчанию.
			return ReturnType();
		}
	}

	TFunction& operator=(const FunctionType& Func)
	{
		setFunction(Func);
		return this;
	}

	operator bool() { return func; }

private:
	FunctionType func;
};
