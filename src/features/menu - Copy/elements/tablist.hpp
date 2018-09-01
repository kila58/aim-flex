#pragma once

#include "../../../aim-flex.hpp"

#include "../../input/input.hpp"

// big nox note: ur making list into std::deque because it's static now so every time u use list now u have to find the appropriate list in the deque for the sublevel

//using IndexedTab = std::pair<size_t, BaseTab>;

struct IndexedTab
{
	IndexedTab(size_t index, BaseTab tab, int sublevel) : index(index), tab(tab), sublevel(sublevel) {}

	size_t index;
	BaseTab tab;
	int sublevel;
};

class TabList
{
private:
	static std::once_flag flag;
	// todo: also we could just put indice, counter and sublevel inside the IndexedTab... (late night coding???)
	static std::deque<std::deque<IndexedTab>> list;
	int x = 250, y = 250;
	int sublevel = 0;
	static std::unordered_map</*sublevel*/int, std::pair</*index*/int, /*list_len*/int>> current_indices;
	static int sublevel_counter;
	static int current_sublevel;
	static std::string current_main_tab_title;
public:
	TabList()
	{
		sublevel_counter++;

		sublevel = sublevel_counter;
	}

	std::deque<IndexedTab>& FindList(int sublevel)
	{
		for (auto& idxtab : list)
		{
			for (auto& what : idxtab)
			{
				if (what.sublevel == sublevel)
					return what;
			}
		}
	}

	std::optional<IndexedTab> FindTab(const std::string& name)
	{
		auto list = 

		for (auto& idxtab : list)
		{
			if (idxtab.tab.GetTitle() == name)
				return idxtab;
		}

		return std::nullopt;
	}

	std::optional<IndexedTab> FindTabWithIndex(int index)
	{
		for (auto& idxtab : list)
		{
			if (idxtab.index == index)
				return idxtab;
		}

		return std::nullopt;
	}

	std::optional<IndexedTab> FindTabWithDifference(const std::string& name, int difference)
	{
		bool next = false;

		for (auto& idxtab : list)
		{
			// don't do anything dumb in list please...
			//int index = &tab - &list[0];

			if (idxtab.tab.GetTitle() == name)
			{
				//if (list.c(index))
					//return list.at(index);

				return idxtab;
			}
		}

		return std::nullopt;
	}

	// definition of TabList::AddTabList && ClearTabListSublevels is in menu.cpp
	TabList& AddTabList(TabList& new_list);
	void ClearTabListSublevels();
	void Construct(json& settings)
	{
		for (auto[it, index] = std::tuple{ settings.begin(), 1u }; it != settings.end(); ++it, index++)
		{
			// base level, use the tab names
			if (sublevel == 1)
				list.emplace_back(index, it.key(), current_sublevel);
			// sub level, use the feature pretty names
			else
				list.emplace_back(index, it.value()[1].get<std::string>(), current_sublevel);
		}
	}
	void DrawShadow()
	{
		int final_tab_height = basetab::tab_h + basetab::tab_divider_y;
		int tabs_height = final_tab_height * list.size()
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
	void DrawOutline()
	{
		int final_tab_height = basetab::tab_h + basetab::tab_divider_y;
		int tabs_height = final_tab_height * list.size()
			- basetab::tab_divider_y; // ignore space for last divider

		matsystemsurface->SetDrawColor(Color(0, 0, 0));
		matsystemsurface->DrawOutlinedRect(x, y, basetab::tab_w, tabs_height);
	}
	void DrawTabDividers(BaseTab& tab)
	{
		auto [x, y] = tab.GetPosition();

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
	// todo: do this on init automatically
	void CreateSublevelIndex(int sublevel, int length)
	{
		if (!current_indices.count(sublevel))
			current_indices[sublevel] = { 1, length };
	}
	// todo todo todo !!!!!!!!: clear current_indices
	void UpdateSublevels(int new_index)
	{
		if (current_sublevel == 1)
		{
			debug << "title " << current_main_tab_title << "\n!";

			if (auto next = FindTabWithIndex(new_index))
			{
				ClearTabListSublevels();

				AddTabList(TabList())
					.Construct(::settings.GetSettings()[next.value().tab.GetTitle()]);

				debug << "hay " << current_main_tab_title << "\n!";
			}

			//AddTabList(TabList())
			//	.Construct(::settings.GetSettings()["rage"]);
		}
	}
	void Think()
	{
		std::call_once(flag, [&]() 
		{
			input.OnKey(VK_DOWN, [this]()
			{
				//CreateSublevelIndex();

				auto& _cur = current_indices[current_sublevel];
				
				auto& current_index = _cur.first;
				auto last_index = current_index;
				auto& length = _cur.second;

				debug << "length: " << length << "\n";

				if (current_index == length)
					current_index = 1;
				else
					current_index++;

				debug << "difference: " << current_index - last_index << "\n";

				debug << "vk_next, current_sublevel: " << current_sublevel << "\n";
				debug << "vk_next: " << current_index << "\n";

				UpdateSublevels(current_index);
			});

			input.OnKey(VK_UP, [this]()
			{
				//CreateSublevelIndex();
				
				auto& _cur = current_indices[current_sublevel];

				auto& current_index = _cur.first;
				auto& length = _cur.second;

				if (current_index == 1)
					current_index = length;
				else
					current_index--;

				debug << "vk_prior, current_sublevel: " << current_sublevel << "\n";
				debug << "vk_prior: " << current_index << "\n";

				UpdateSublevels(current_index);
			});

			input.OnKey(VK_RIGHT, [this]()
			{
				current_sublevel++;
			});

			input.OnKey(VK_LEFT, [this]()
			{
				current_sublevel--;
			});
		});
	}
	void Draw()
	{
		x =+ sublevel * (basetab::tab_w - 1);

		auto curlist = list

		Think();
		CreateSublevelIndex(sublevel, list.size());

		//debug << sublevel << "\n";

		auto& current_index = current_indices[sublevel].first;

		int final_tab_height = basetab::tab_h + basetab::tab_divider_y;

		DrawShadow();

		for (auto& idxtab : list)
		{
			size_t index = idxtab.index;
			BaseTab& tab = idxtab.tab;

			//if (sublevel == 2)
				//debug << "cur index (" << current_index << ") : " << index << "\n";

			bool highlighted = index == current_index;
			tab.SetHighlighted(highlighted);

			if (sublevel == 1 && highlighted)
			{
				current_main_tab_title = idxtab.tab.GetTitle();

				//debug << current_main_tab_title << "\n";
			}

			tab.SetX(x);
			tab.SetY(index * (final_tab_height) 
				- final_tab_height + y); // ignore space for first tab

			tab.Draw();

			if (index != 1)
				DrawTabDividers(tab);
		}

		DrawOutline();
	}
};

int TabList::sublevel_counter = 0;
int TabList::current_sublevel = 1;
std::unordered_map</*sublevel*/int, std::pair</*index*/int, /*list_len*/int>> TabList::current_indices;
std::string TabList::current_main_tab_title = "";
std::deque<IndexedTab> TabList::list;

std::once_flag TabList::flag;

/* 
template<typename... Args>
TabList Construct(Args&&... args)
{
	auto TabList = [&] { return TabList(std::move(args)...); };
}
*/
