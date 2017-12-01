#pragma once

class CMatSystemSurface
{
<<<<<<< HEAD
public:
	void SetDrawColor(Color color)
	{
		return getvfunc<void(__thiscall*)(void*, Color)>(this, 14)(this, color);
	}
	void DrawFilledRect(int x, int y, int w, int h)
	{
		return getvfunc<void(__thiscall*)(void*, int, int, int, int)>(this, 16)(this, x, y, x + w, y + h);
	}
=======

>>>>>>> 1ca4338319b59e3df8f6fdd429fca15760e6e890
};
