#pragma once

class CEngineClient
{
public:
	void GetScreenSize(int* w, int* h)
	{
		return getvfunc<void(__thiscall*)(void*, int*, int*)>(this, 5)(this, w, h);
	}
	VMatrix& WorldToScreenMatrix()
	{
		return getvfunc<VMatrix&(__thiscall*)(void*)>(this, 37)(this);
	}
};
