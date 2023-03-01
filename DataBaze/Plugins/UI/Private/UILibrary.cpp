#include "UILibrary.h"
#include "Components/UISpacer.h"
#include "Components/UIVerticalBox.h"
#include "Components/UIHorizontalBox.h"

UILib::SlotPtr UILib::CreateSlot(HWND OwningWnd, const Size2D& Size)
{
	return SlotPtr(new UISlot(OwningWnd, Size));
}

UILib::SlotPtr UILib::CreateSpacer(const Size2D& Size)
{
	return SlotPtr(new UISpacer(Size));
}

std::shared_ptr<UIVerticalBox> UILib::CreateVerticalBox(const Size2D& InPos, const Size2D& InSize)
{
	auto obj = new UIVerticalBox;
	obj->SetSize(InSize);
	obj->SetPos(InPos);
	return std::shared_ptr<UIVerticalBox>(obj);
}

std::shared_ptr<UIHorizontalBox> UILib::CreateHorizontalBox(const Size2D& InPos, const Size2D& InSize)
{
	auto obj = new UIHorizontalBox;
	obj->SetSize(InSize);
	obj->SetPos(InPos);
	return std::shared_ptr<UIHorizontalBox>(obj);
}
