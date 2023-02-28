#include "Components/UIVerticalBox.h"

void UIVerticalBox::Draw(Size2D InPos, Size2D InSize)
{
	Position = InPos;
	Size	 = InSize;

	if (GetChildren().size() == 0) return;

	const int SpaceToFill = GetEmptySpace();
	const int FillerNum	  = GetFillersNum();
	const int ElemHeight  = SpaceToFill / FillerNum;


	int CurrentPosY = Position.Y;
	for (size_t i = 0; i < GetChildren().size(); ++i)
	{
		if (GetChildren()[i].IsFill())
		{
			GetChildren()[i].SetPos(Size2D(Position.X, CurrentPosY));
			GetChildren()[i].SetSize(Size2D(Size.X, ElemHeight));
			CurrentPosY += ElemHeight;
		}
		else
		{
			GetChildren()[i].SetPos(Size2D(Position.X, CurrentPosY));
			GetChildren()[i].SetSize(Size2D(Size.X, GetChildren()[i].GetSize().Y));
			CurrentPosY += GetChildren()[i].GetSize().Y;
		}
	}

	for (auto& Elem : GetChildren())
	{
		Elem.ReDraw();
	}
}

inline int UIVerticalBox::GetEmptySpace()
{
	int EmptySpace = Size.Y;
	for (auto& Elem : GetChildren())
	{
		if (! Elem.IsFill())
		{
			EmptySpace = EmptySpace - Elem.GetSize().Y;
		}
	}
	return (EmptySpace < 0) ? 0 : EmptySpace;
}

inline int UIVerticalBox::GetFillersNum()
{
	int Fillers = 0;
	for (auto& Elem : GetChildren())
	{
		if (Elem.IsFill())
		{
			++Fillers;
		}
	}
	return Fillers;
}
