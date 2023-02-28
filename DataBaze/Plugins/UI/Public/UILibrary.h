#pragma once
#include "Size2D.h"

class UIVerticalBox;
class UIHorizontalBox;

namespace UILib
{
UIVerticalBox*	 CreateVerticalBox(const Size2D& InPos, const Size2D& InSize);
UIHorizontalBox* CreateHorizontalBox(const Size2D& InPos, const Size2D& InSize);
} // namespace UILib
