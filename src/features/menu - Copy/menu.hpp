#pragma once

#include "../features.hpp"

#include "../settings/settings.hpp"

#include "elements/basetab.hpp"
#include "elements/tablist.hpp"

class Menu : public BaseFeature
{
public:
	Menu() : BaseFeature(PAINT) {}

	void Init();
	void Invoke();
	void Destroy();

	int font, font_small;
	std::deque<TabList> lists;
	bool mark_for_clear = false;
};

extern Menu menu;
