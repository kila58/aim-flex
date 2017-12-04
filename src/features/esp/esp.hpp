#pragma once

#include "../features.hpp"

class ESP : public BaseFeature
{
public:
	ESP() : BaseFeature(HookType::PAINT) {}

	void Init();
	void Invoke();
	void Destroy();
};

ESP esp;
