#pragma once

#include "../../../aim-flex.hpp"

#include "../../input/input.hpp"

#include "../elements/basetab.hpp"
#include "../elements/toggletab.hpp"
#include "../elements/dropdown.hpp"
#include "../elements/dropdown_option.hpp"
#include "../elements/slider.hpp"
#include "../elements/keypicker.hpp"

struct IndexedTab
{
	IndexedTab(uint index, BaseTab* tab) : index(index), tab(tab) {}

	uint index;
	std::unique_ptr<BaseTab> tab;
};

struct IndexedTabList
{
	IndexedTabList(uint sublevel, json& ref_to_settings) : sublevel(sublevel), ref_to_settings(ref_to_settings) {}

	uint sublevel;
	std::deque<IndexedTab> list;
	uint index_counter = 0;
	uint current_index = 1;
	json& ref_to_settings;

	void AddTab(const std::string& title, bool has_sublist, json& ref_to_setting, tab_type type = type_basetab)
	{
		index_counter++;

		switch (type)
		{
		case type_basetab:
			list.emplace_back(index_counter, new BaseTab(title, has_sublist));
			break;
		case type_toggletab:
			list.emplace_back(index_counter, new ToggleTab(title, has_sublist, ref_to_setting));
			break;
		case type_dropdown:
			list.emplace_back(index_counter, new DropdownTab(title, has_sublist, ref_to_setting));
			break;
		case type_dropdown_option:
			list.emplace_back(index_counter, new DropdownOptionTab(title, has_sublist, ref_to_setting));
			break;
		case type_slider:
			list.emplace_back(index_counter, new SliderTab(title, has_sublist, ref_to_setting));
			break;
		case type_keypicker:
			list.emplace_back(index_counter, new KeyPickerTab(title, has_sublist, ref_to_setting));
			break;
		}
	}
};

json& json_placeholder = json();

class TabLists
{
private:
	// mapper doesn't resolve std::once_flag, so we just use a bool
	//std::once_flag flag;
	bool once = false;
	std::map<uint, IndexedTabList> lists;
	uint sublevel_counter = 0;
	int x = 0, y = 540;
	uint current_sublevel = 1;
public:
	void AddList(json& settings, json& super_class = json_placeholder)
	{
		sublevel_counter++;

		lists.emplace(sublevel_counter, IndexedTabList(sublevel_counter, settings));

		for (auto& tab : settings.items())
		{
			if (current_sublevel == 1)
				lists.at(sublevel_counter).AddTab(tab.key(), true, tab.value());
			else
			{
				std::string name = tab.value()[1].get<std::string>();

				// [json.exception.parse_error.112] parse error at 240: error reading CBOR; last byte: 0x1C 
				if (name == "a")
					continue;

				tab_type type = type_basetab;
				bool use_superclass = false;

				if (tab.value()[2].is_boolean())
				{
					type = type_toggletab;
				}
				else if (tab.value()[2].is_array())
				{
					type = type_dropdown;
				}
				else if (tab.value()[2].is_null())
				{
					type = type_dropdown_option;
					use_superclass = true;
				}
				else if (tab.value()[3].is_number())
				{
					type = type_slider;
				}
				else if (tab.value()[2].is_number())
				{
					type = type_keypicker;
				}

				lists.at(sublevel_counter).AddTab(name, tab.value()[2].is_structured(), use_superclass ? super_class : tab.value(), type);
			}
		}
	}
	void DrawShadow(uint list_size, uint sublevel)
	{
		int final_tab_height = basetab::tab_h + basetab::tab_divider_y;
		int tabs_height = final_tab_height * list_size
			- basetab::tab_divider_y; // ignore space for last divider

		if (sublevel == 1)
		{
			matsystemsurface->SetDrawColor(Color(0, 0, 0, 128));
			matsystemsurface->DrawOutlinedRect(x - 1, y - 1, basetab::tab_w + 2, tabs_height + 2);
		}
		// we are merging the dividers, so we need to push forward the shadow a bit
		else
		{
			matsystemsurface->SetDrawColor(Color(0, 0, 0, 128));
			matsystemsurface->DrawOutlinedRect(x + 2, y - 1, basetab::tab_w - 1, tabs_height + 2);
		}
	}
	void DrawOutline(uint list_size)
	{
		int final_tab_height = basetab::tab_h + basetab::tab_divider_y;
		int tabs_height = final_tab_height * list_size
			- basetab::tab_divider_y; // ignore space for last divider

		matsystemsurface->SetDrawColor(Color(0, 0, 0));
		matsystemsurface->DrawOutlinedRect(x, y, basetab::tab_w, tabs_height);
	}
	void DrawTabDividers(BaseTab& tab)
	{
		auto[x, y] = tab.GetPosition();

		if (tab.GetHighlighted())
		{
			matsystemsurface->SetDrawColor(Color(35, 35, 35));
			matsystemsurface->DrawLine(x, y - 2, x + basetab::tab_w, y - 2);

			matsystemsurface->SetDrawColor(Color(0, 0, 0));
			matsystemsurface->DrawLine(x, y - 1, x + basetab::tab_w, y - 1);
		}
		else
		{
			matsystemsurface->SetDrawColor(Color(0, 0, 0));
			matsystemsurface->DrawLine(x, y - 2, x + basetab::tab_w, y - 2);

			matsystemsurface->SetDrawColor(Color(35, 35, 35));
			matsystemsurface->DrawLine(x, y - 1, x + basetab::tab_w, y - 1);
		}
	}
	void Think()
	{
		//std::call_once(flag, [&]()
		if (!once)
		{
			input.OnKey(VK_DOWN, [this]()
			{
				auto& current_tab_list = lists.at(current_sublevel);
				uint& current_index = current_tab_list.current_index;

				auto& current_tab = current_tab_list.list.at(current_index - 1);
				if (!current_tab.tab->IsMenuOpen())
					return;

				size_t length = current_tab_list.list.size();

				if (current_index == length)
					current_index = 1;
				else
					current_index++;
			});

			input.OnKey(VK_UP, [this]()
			{
				auto& current_tab_list = lists.at(current_sublevel);
				uint& current_index = current_tab_list.current_index;

				auto& current_tab = current_tab_list.list.at(current_index - 1);
				if (!current_tab.tab->IsMenuOpen())
					return;

				size_t length = current_tab_list.list.size();

				if (current_index == 1)
					current_index = length;
				else
					current_index--;
			});

			input.OnKey(VK_RIGHT, [this]()
			{
				auto& current_tab_list = lists.at(current_sublevel);
				uint& current_index = current_tab_list.current_index;

				auto& current_tab = current_tab_list.list.at(current_index - 1);
				if (!current_tab.tab->IsMenuOpen())
					return;

				current_tab.tab->OnPressedRight();

				if (current_tab.tab->HasSublist())
				{
					if (current_sublevel == 1)
					{
						current_sublevel++;

						auto& new_list = current_tab_list.ref_to_settings[current_tab.tab->GetTitle()];
						AddList(new_list);
					}
					else
					{
						for (auto& i : current_tab_list.ref_to_settings)
						{
							if (i[1].get<std::string>() == current_tab.tab->GetTitle())
							{
								current_sublevel++;

								AddList(i[2], i);

								break;
							}
						}
					}
				}
			});

			input.OnKey(VK_LEFT, [this]()
			{
				auto& current_tab_list = lists.at(current_sublevel);
				uint& current_index = current_tab_list.current_index;

				auto& current_tab = current_tab_list.list.at(current_index - 1);
				if (!current_tab.tab->IsMenuOpen())
					return;

				if (current_tab.tab->OnPressedLeft())
					return;

				if (current_sublevel > 1)
				{
					uint new_sublevel = current_sublevel - 1;

					// std::experimental::erase_if?
					for (auto it = lists.begin(); it != lists.end(); )
					{
						if (it->second.sublevel > new_sublevel)
						{
							it = lists.erase(it);

							continue;
						}

						++it;
					}

					current_sublevel = new_sublevel;
					sublevel_counter = new_sublevel;
				}
			});

			once = true;
		}
		//});
	}
	void Draw()
	{
		Think();

		constexpr int final_tab_height = basetab::tab_h + basetab::tab_divider_y;

		int sw, sh;
		engineclient->GetScreenSize(sw, sh);

		y = (sh / 3);

		for (auto it = lists.begin(); it != lists.end(); ++it)
		{
			auto& indexed_tab_list = *it;

			uint list_size = indexed_tab_list.second.list.size();
			uint sublevel = indexed_tab_list.second.sublevel;
			uint current_index = indexed_tab_list.second.current_index;

			x = sublevel * (basetab::tab_w - 1)
				- (basetab::tab_w - 1); // ignore space for first sublevel

			if (sublevel != 1)
				y += std::prev(it)->second.current_index * (final_tab_height)
				- final_tab_height; // ignore space for first sublevel

			DrawShadow(list_size, sublevel);

			for (auto& indexed_tab : indexed_tab_list.second.list)
			{
				auto tab = indexed_tab.tab.get();
				uint index = indexed_tab.index;

				tab->SetHighlighted(index == current_index);

				tab->SetX(x);
				tab->SetY(index * (final_tab_height)
					- final_tab_height + y); // ignore space for first tab

				tab->Draw();

				if (index != 1)
					DrawTabDividers(*tab);
			}

			DrawOutline(list_size);
		}
	}
};
