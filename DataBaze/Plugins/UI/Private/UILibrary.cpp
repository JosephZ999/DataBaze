#include "UILibrary.h"
#include "Components/UIVerticalBox.h"
#include "Components/UIHorizontalBox.h"

UIVerticalBox* UILib::CreateVerticalBox(const Size2D& InPos, const Size2D& InSize)
{
	auto obj = new UIVerticalBox;
	obj->SetSize(InSize);
	obj->SetPos(InPos);
	return obj;
}

UIHorizontalBox* UILib::CreateHorizontalBox(const Size2D& InPos, const Size2D& InSize)
{
	auto obj = new UIHorizontalBox;
	obj->SetSize(InSize);
	obj->SetPos(InPos);
	return obj;
}
