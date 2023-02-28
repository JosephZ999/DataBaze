#pragma once
#include "Components/UISlot.h"
#include "UIContainer.h"

class UIHorizontalBox : public UIContainer, public UISlot
{
public:
	virtual void Draw(Size2D InPos, Size2D InSize) override;

private:
	inline int GetEmptySpace();
	inline int GetFillersNum();
};
