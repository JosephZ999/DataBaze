#include "DBButtonManager.h"
#include "Components/UISpacer.h"
#include "UIDataTypes.h"

void DBButtonManager::Initialize(HWND OwnerWnd, std::vector<FButton>& Buttons)
{
	VerticalBox = std::unique_ptr<UIVerticalBox>(new UIVerticalBox);

	if (Buttons.size() == 0) return;
	for (size_t i = 0; i < Buttons.size(); ++i)
	{
		auto Wnd = DBLib::CreateButton(OwnerWnd, Buttons[i]);

		auto Slot = UISlot(Wnd);
		// Slot.SetSize(Size2D(20, 20));
		VerticalBox->AddChild(Slot);

		if (i != Buttons.size() - 1)
		{
			VerticalBox->AddChild(UISpacer(Size2D(0, 25)));
		}
	}
	VerticalBox->Draw(Size2D(25, 25), Size2D(150, 280));
}
