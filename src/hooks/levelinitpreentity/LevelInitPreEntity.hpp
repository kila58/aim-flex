#pragma once

#include "../hooks.hpp"

class LevelInitPreEntityHook : public BaseHook
{
public:
	void Init();
	void Destroy();
};
