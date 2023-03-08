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
	{}
	UISlot(HWND InOwningWnd, Size2D InSize)
		: OwningWnd(InOwningWnd)
		, Size(InSize)
		, bFill(true)
	{}

private:
	HWND OwningWnd = nullptr;

	bool bFill = false;

	EVerticalAlignment	 VAlignment = VA_Fill;
	EHorizontalAlignment HAlignment = HA_Fill;

protected:
	Size2D Position;
	Size2D Size;
	Size2D Offset;

public:
	void SetWindow(HWND InOwningWnd);

	virtual void Draw(Size2D InPos, Size2D InSize);

	void ReDraw();

	bool IsFill() const { return bFill; }
	void SetFill(bool Fill) { bFill = Fill; }

	Size2D GetPos() const { return Position; }
	Size2D GetSize() const { return Size; }
	void   SetPos(const Size2D& NewPos) { Position = NewPos; }
	void   SetSize(const Size2D& NewSize) { Size = NewSize; }
	void   SetOffset(const Size2D& NewOffset);

	void SetVAlign(EVerticalAlignment InAlign) { VAlignment = InAlign; }
	void SetHAlign(EHorizontalAlignment InAlign) { HAlignment = InAlign; }
};
