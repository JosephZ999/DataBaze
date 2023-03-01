#pragma once
#include "Size2D.h"
#include <memory>

#include "Components/UISlot.h"

class UIVerticalBox;
class UIHorizontalBox;
namespace UILib
{
typedef std::shared_ptr<UISlot> SlotPtr;

template <typename T> //
std::shared_ptr<T> CreateASlot(const Size2D& InPos, const Size2D& InSize)
{
	std::shared_ptr<T> TObj(new T);
	if (UISlot* SlotPtr = dynamic_cast<UISlot*>(TObj))
	{
		SlotPtr->SetPos(InPos);
		SlotPtr->SetSize(InSize);
		return TObj;
	}
}
SlotPtr CreateSlot(HWND OwningWnd, const Size2D& Size);
SlotPtr CreateSpacer(const Size2D& Size);

std::shared_ptr<UIVerticalBox>	 CreateVerticalBox(const Size2D& InPos, const Size2D& InSize);
std::shared_ptr<UIHorizontalBox> CreateHorizontalBox(const Size2D& InPos, const Size2D& InSize);

} // namespace UILib
