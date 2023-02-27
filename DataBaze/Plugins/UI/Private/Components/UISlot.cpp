#include "Components/UISlot.h"

void UISlot::SetWindow(HWND InOwningWnd)
{
	OwningWnd = InOwningWnd;
}

void UISlot::Draw(Size2D InPos, Size2D InSize)
{
	if (OwningWnd)
	{
		SetWindowPos(OwningWnd, 0,	//
			Position.X, Position.Y, //
			Size.X, Size.Y,			//
			SWP_NOSIZE);			// Options
	}
}
