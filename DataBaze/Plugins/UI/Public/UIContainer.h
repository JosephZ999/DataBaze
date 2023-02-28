#pragma once
#include <vector>

class UISlot;

class UIContainer
{
	std::vector<UISlot> Children;

protected:
	inline std::vector<UISlot>& GetChildren() { return Children; }

public:
	void AddChild(const UISlot& InChild);
	int GetChildrenNum();
};
