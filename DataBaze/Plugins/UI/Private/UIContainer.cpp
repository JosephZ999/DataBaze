#include "UIContainer.h"
#include "Components/UISlot.h"

void UIContainer::AddChild(const SlotPtr InChild)
{
	Children.push_back(InChild);
}

int UIContainer::GetChildrenNum()
{
	return Children.size();
}
