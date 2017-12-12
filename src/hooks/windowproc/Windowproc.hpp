#pragma once

#include "../hooks.hpp"

class WindowProcHook : public BaseHook
{
public:
	void Init();
	void Destroy();
};
