#pragma once
#include <vector>
#include <memory>

class UISlot;

class UIContainer
{
	typedef std::shared_ptr<UISlot> SlotPtr;
	typedef std::vector<SlotPtr>	SlotPtrArr;

	SlotPtrArr Children;

protected:
	SlotPtrArr GetChildren() { return Children; }

public:
	void AddChild(const SlotPtr InChild);
	int	 GetChildrenNum();
};
