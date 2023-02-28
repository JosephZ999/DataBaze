#include "DBButtonManager.h"

void DBButtonManager::Initialize(HWND OwnerWnd, std::vector<FButton>& Buttons)
{
	VerticalBox = std::unique_ptr<UIVerticalBox>(new UIVerticalBox);

	if (Buttons.size() == 0) return;
	for (size_t i = 0; i < Buttons.size(); ++i)
	{
		auto Wnd = DBLib::CreateButton(OwnerWnd, Buttons[i]);
		VerticalBox->AddChild(UISlot(Wnd));
		// if (i + 1 != Buttons.size())
		//{
		//	VerticalBox->AddChild(UISlot(nullptr, ));
		//}
	}
	VerticalBox->Draw(Size2D(25, 25), Size2D(150, 280));
}
