#include "Components/UIVerticalBox.h"

void UIVerticalBox::Draw(Size2D InPos, Size2D InSize)
{
	Position = InPos;
	Size	 = InSize;

	if (GetChildren().size() == 0) return;

	const int ElemHeight = Size.Y / GetChildrenNum();

	for (size_t i = 0; i < GetChildren().size(); ++i)
	{
		const int PosY = (i == 0) ? Position.Y : Position.Y + i * ElemHeight;
		GetChildren()[i].Draw(Size2D(Position.X, PosY), Size2D(Size.X, ElemHeight));
	}
}