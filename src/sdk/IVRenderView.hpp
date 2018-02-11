#pragma once

class IVRenderView
{
public:
	void SetBlend(float blend)
	{
		getvfunc<void(__thiscall*)(void*, float)>(this, 4)(this, blend);
	}
	void SetColor(float r, float g, float b)
	{
		float color[3];
		color[0] = r / 255.f;
		color[1] = g / 255.f;
		color[2] = b / 255.f;

		getvfunc<bool(__thiscall*)(void*, float*)>(this, 6)(this, color);
	}
};
