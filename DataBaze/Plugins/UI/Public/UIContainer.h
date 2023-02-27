#pragma once
#include <vector>

class UISlot;

class UIContainer
{
	std::vector<UISlot> Children;

public:
	void AddChild(const UISlot& InChild);
	inline std::vector<UISlot>& GetChildren() { return Children; }
};
