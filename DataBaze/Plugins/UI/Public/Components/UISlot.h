#pragma once
#include "Size2D.h"
#include "framework.h"
#include "UIDataTypes.h"

class UISlot
{
public:
	UISlot() {}
	UISlot(HWND InOwningWnd)
		: OwningWnd(InOwningWnd)
		, bFill(true)
	{
	}
	UISlot(HWND InOwningWnd, Size2D InSize)
		: OwningWnd(InOwningWnd)
		, Size(InSize)
	{
	}

private:
	HWND OwningWnd = nullptr;

	bool bFill = false;

	EVerticalAlignment	 VAlignment = VA_Center;
	EHorizontalAlignment HAlignment = HA_Center;

protected:
	Size2D Position;
	Size2D Size;

public:
	void SetWindow(HWND InOwningWnd);

	virtual void Draw(Size2D InPos, Size2D InSize);

	void ReDraw();

	bool IsFill() const { return bFill; }

	Size2D GetPos() const { return Position; }
	Size2D GetSize() const { return Size; }
	void   SetPos(const Size2D& NewPos) { Position = NewPos; }
	void   SetSize(const Size2D& NewSize) { Size = NewSize; }
};
