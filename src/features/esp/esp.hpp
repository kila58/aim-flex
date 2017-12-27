#pragma once

#include "../features.hpp"

class C_BaseEntity;

class ESP : public BaseFeature
{
public:
	ESP() : BaseFeature(PAINT) {}

	void Init();
	bool CreateBox(C_BaseEntity* p, int& x, int& y, int& w, int& h);
	void Invoke();
	void Destroy();
};

extern ESP esp;
