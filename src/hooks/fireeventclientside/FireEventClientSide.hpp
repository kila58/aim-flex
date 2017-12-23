#pragma once

#include "../hooks.hpp"

class FireEventClientSideHook : public BaseHook
{
public:
	void Init();
	void Destroy();
};
