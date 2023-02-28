#pragma once
#include "Components/UISlot.h"
#include "Size2D.h"

class UISpacer : public UISlot
{
public:
	UISpacer(Size2D InSize)
		: UISlot(nullptr, InSize)
	{
	}
};
