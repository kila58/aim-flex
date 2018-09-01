#pragma once

#include "../hooks.hpp"

class DrawModelExecuteHook : public BaseHook
{
public:
	void Init();
	void Destroy();
};

extern DrawModelExecuteHook drawmodelexecute_hook;
