#include "Components/UISlot.h"

void UISlot::SetWindow(HWND InOwningWnd)
{
	OwningWnd = InOwningWnd;
}

void UISlot::Draw(Size2D InPos, Size2D InSize)
{
	if (! OwningWnd) return;

	UINT Flags = SWP_NOSIZE | SWP_NOMOVE;

	if (Size != InSize)
	{
		Flags = Flags - SWP_NOSIZE;
		Size  = InSize;
	}
	if (Position != InPos)
	{
		Flags	 = Flags - SWP_NOMOVE;
		Position = InPos;
	}

	if (OwningWnd)
	{
		SetWindowPos(OwningWnd, 0,	//
			Position.X, Position.Y, //
			Size.X, Size.Y,			//
			Flags);					// Options
	}
}

void UISlot::ReDraw()
{
	Draw(Position, Size);
}
