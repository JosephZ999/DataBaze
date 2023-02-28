#pragma once
#include "UIContainer.h"
#include "Components/UISlot.h"

class UIVerticalBox : public UIContainer, public UISlot
{
public:
	virtual void Draw(Size2D InPos, Size2D InSize) override;
};
