#pragma once

#include "../hooks.hpp"

class TraceRayHook : public BaseHook
{
public:
	void Init();
	void Hook();
	void Destroy();
};

extern TraceRayHook traceray_hook;
