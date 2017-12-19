#pragma once

#include "../features.hpp"

class ESP : public BaseFeature
{
public:
	ESP() : BaseFeature(PAINT) {}

	void Init();
	void Invoke();
	void Destroy();
};

extern ESP esp;
