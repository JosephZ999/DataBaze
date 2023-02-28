#pragma once
#include "Size2D.h"
#include "framework.h"

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
	bool bFill	   = false;

protected:
	Size2D Position;
	Size2D Size;

public:
	void SetWindow(HWND InOwningWnd);

	virtual void Draw(Size2D InPos, Size2D InSize);
};
