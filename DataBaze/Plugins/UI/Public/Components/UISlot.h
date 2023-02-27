#pragma once
#include "Size2D.h"
#include "framework.h"

class UISlot
{
public:
	UISlot() {}
	UISlot(HWND InOwningWnd)
		: OwningWnd(InOwningWnd)
	{
	}

private:
	HWND OwningWnd = nullptr;

protected:
	Size2D Position;
	Size2D Size;

public:
	void SetWindow(HWND InOwningWnd);

	virtual void Draw(Size2D InPos, Size2D InSize);
};
