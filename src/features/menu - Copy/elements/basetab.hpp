#pragma once

namespace basetab
{
	const int tab_w = (16 * 16) /*256*/;
	const int tab_h = 36;
	const int tab_title_left_pad = 7;
	const int tab_divider_y = 2;
};

class BaseTab
{
private:
	std::string name = "Tab";
	int x = 0, y = 0;
	bool highlighted = false;
public:
	BaseTab(std::string& name) : name(name) {}

	inline std::pair<int, int> GetPosition() const
	{
		return std::make_pair(x, y);
	}
	inline void SetPosition(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	inline void SetX(int x)
	{
		this->x = x;
	}
	inline void SetY(int y)
	{
		this->y = y;
	}
	inline void SetHighlighted(bool current)
	{
		highlighted = current;
	}
	inline bool GetHighlighted()
	{
		return highlighted;
	}
	inline std::string& GetTitle()
	{
		return name;
	}
	// definition of BaseTab::Draw is in menu.cpp
	void Draw();
	void DrawArrow();
};
