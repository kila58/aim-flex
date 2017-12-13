#pragma once

#include "../features.hpp"

class Unload : public BaseFeature
{
public:
	Unload() : BaseFeature(HookType::WINDOWPROC) {}

	void Init();
	void Invoke();
	void Destroy();
};

extern Unload unload;