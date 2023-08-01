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
			// ����� ������� ������ ������ ���������, ���� ������� �� �����������.
			// ��������, ��������� ���������� ��� ������� �������� �� ���������.
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
