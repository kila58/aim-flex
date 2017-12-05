#pragma once

class CMatSystemSurface
{
public:
	void SetDrawColor(Color color)
	{
		return getvfunc<void(__thiscall*)(void*, Color)>(this, 14)(this, color);
	}
	void DrawFilledRect(int x, int y, int w, int h)
	{
		return getvfunc<void(__thiscall*)(void*, int, int, int, int)>(this, 16)(this, x, y, x + w, y + h);
	}
	void DrawOutlinedRect(int x, int y, int w, int h)
	{
		return getvfunc<void(__thiscall*)(void*, int, int, int, int)>(this, 18)(this, x, y, x + w, y + h);
	}
};
