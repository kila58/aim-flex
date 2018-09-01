#pragma once

#include "../features.hpp"

#include "../settings/settings.hpp"

#include "elements/basetab.hpp"
#include "elements/tablist.hpp"
#include "elements/toggletab.hpp"
#include "elements/dropdown.hpp"
#include "elements/dropdown_option.hpp"
#include "elements/slider.hpp"

class Menu : public BaseFeature
{
public:
	Menu() : BaseFeature(PAINT) {}

	void Init();
	void Invoke();
	void Destroy();

	int font, font_small;
	TabLists lists;
	bool open = false;
};

extern Menu menu;
