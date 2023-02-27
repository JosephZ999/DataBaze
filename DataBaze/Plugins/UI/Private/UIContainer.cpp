#include "UIContainer.h"
#include "Components/UISlot.h"

void UIContainer::AddChild(const UISlot& InChild)
{
	Children.push_back(InChild);
}
