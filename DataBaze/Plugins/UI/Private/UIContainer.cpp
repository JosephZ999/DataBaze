#include "UIContainer.h"
#include "Components/UISlot.h"

void UIContainer::AddChild(const UISlot& InChild)
{
	Children.push_back(InChild);
}

int UIContainer::GetChildrenNum()
{
	return Children.size();
}
