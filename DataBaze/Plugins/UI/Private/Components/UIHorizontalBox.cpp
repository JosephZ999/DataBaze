#include "Components/UIHorizontalBox.h"

void UIHorizontalBox::Draw(Size2D InPos, Size2D InSize)
{
	Position = InPos;
	Size	 = InSize;

	InPos += Offset;

	if (GetChildren().size() == 0) return;

	const int SpaceToFill = GetEmptySpace();
	const int FillerNum	  = GetFillersNum();
	const int ElemWidth	  = SpaceToFill / FillerNum;

	int CurrentPosX = InPos.X;
	for (size_t i = 0; i < GetChildren().size(); ++i)
	{
		if (GetChildren()[i]->IsFill())
		{
			const auto nPos	 = Size2D(CurrentPosX, InPos.Y);
			const auto nSize = Size2D(ElemWidth, InSize.Y);
			GetChildren()[i]->Draw(nPos, nSize);
			CurrentPosX += ElemWidth;
		}
		else
		{
			const auto nPos	 = Size2D(CurrentPosX, InPos.Y);
			const auto nSize = Size2D(GetChildren()[i]->GetSize().X, InSize.Y);
			GetChildren()[i]->Draw(nPos, nSize);
			CurrentPosX += GetChildren()[i]->GetSize().X;
		}
	}
}

inline int UIHorizontalBox::GetEmptySpace()
{
	int EmptySpace = GetSize().X;
	for (auto& Elem : GetChildren())
	{
		if (! Elem->IsFill())
		{
			EmptySpace = EmptySpace - Elem->GetSize().X;
		}
	}
	return (EmptySpace < 0) ? 0 : EmptySpace;
}

inline int UIHorizontalBox::GetFillersNum()
{
	int Fillers = 0;
	for (auto& Elem : GetChildren())
	{
		if (Elem->IsFill())
		{
			++Fillers;
		}
	}
	if (Fillers <= 0)
	{
		return 1;
	}
	return Fillers;
}
