#include "Components/UIVerticalBox.h"

void UIVerticalBox::Draw(Size2D InPos, Size2D InSize)
{
	Position = InPos;
	Size	 = InSize;

	InPos += Offset;

	if (GetChildren().size() == 0) return;

	const int SpaceToFill = GetEmptySpace();
	const int FillerNum	  = GetFillersNum();
	const int ElemHeight  = SpaceToFill / FillerNum;

	int CurrentPosY = InPos.Y;
	for (size_t i = 0; i < GetChildren().size(); ++i)
	{
		if (GetChildren()[i]->IsFill())
		{
			const auto nPos	 = Size2D(InPos.X, CurrentPosY);
			const auto nSize = Size2D(InSize.X, ElemHeight);
			GetChildren()[i]->Draw(nPos, nSize);
			CurrentPosY += ElemHeight;
		}
		else
		{
			const auto nPos	 = Size2D(InPos.X, CurrentPosY);
			const auto nSize = Size2D(InSize.X, GetChildren()[i]->GetSize().Y);
			GetChildren()[i]->Draw(nPos, nSize);
			CurrentPosY += GetChildren()[i]->GetSize().Y;
		}
	}
}

inline int UIVerticalBox::GetEmptySpace()
{
	int EmptySpace = GetSize().Y;
	for (auto& Elem : GetChildren())
	{
		if (! Elem->IsFill())
		{
			EmptySpace = EmptySpace - Elem->GetSize().Y;
		}
	}
	return (EmptySpace < 0) ? 0 : EmptySpace;
}

inline int UIVerticalBox::GetFillersNum()
{
	int Fillers = 0;
	for (auto& Elem : GetChildren())
	{
		if (Elem->IsFill())
		{
			++Fillers;
		}
	}
	if (Fillers == 0)
	{
		return 1;
	}
	return Fillers;
}
