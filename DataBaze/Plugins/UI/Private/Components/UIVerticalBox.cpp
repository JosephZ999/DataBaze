#include "Components/UIVerticalBox.h"

void UIVerticalBox::Draw(Size2D InPos, Size2D InSize)
{
	Position = InPos;
	Size	 = InSize;

	for (auto& Elem : GetChildren())
	{
		const Size2D StartPoint = {0, 0};
		const Size2D EndPoint	= {0, 0};

		Elem.Draw(StartPoint, EndPoint);
	}
}
