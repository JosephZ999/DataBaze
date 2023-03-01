#include "Components/UISlot.h"

void UISlot::SetWindow(HWND InOwningWnd)
{
	OwningWnd = InOwningWnd;
}

void UISlot::Draw(Size2D InPos, Size2D InSize)
{
	if (! OwningWnd) return;

	Size2D FinalPos;
	Size2D FinalSize;

	switch (VAlignment)
	{
	case VA_Top:
	{
		FinalPos.Y	= InPos.Y;
		FinalSize.Y = Size.Y;
		break;
	}
	case VA_Center:
	{
		FinalPos.Y	= InPos.Y + (InSize.Y / 2 - Size.Y / 2);
		FinalSize.Y = Size.Y;
		break;
	}
	case VA_Bottom:
	{
		FinalPos.Y	= InPos.Y + InSize.Y - Size.Y;
		FinalSize.Y = Size.Y;
		break;
	}
	case VA_Fill:
	{
		FinalPos.Y	= InPos.Y;
		FinalSize.Y = InSize.Y;
		break;
	}
	}

	switch (HAlignment)
	{
	case HA_Left:
	{
		FinalPos.X	= InPos.X;
		FinalSize.X = Size.X;
		break;
	}
	case HA_Center:
	{
		FinalPos.X	= InPos.X + (InSize.X / 2 - Size.X / 2);
		FinalSize.X = Size.X;
		break;
	}
	case HA_Right:
	{
		FinalPos.X	= InPos.X + InSize.X - Size.X;
		FinalSize.X = Size.X;
		break;
	}
	case HA_Fill:
	{
		FinalPos.X	= InPos.X;
		FinalSize.X = InSize.X;
		break;
	}
	}

	if (OwningWnd)
	{
		SetWindowPos(OwningWnd, 0,	  //
			FinalPos.X, FinalPos.Y,	  //
			FinalSize.X, FinalSize.Y, //
			0);						  // Options
	}
}

void UISlot::ReDraw()
{
	Draw(Position, Size);
}
