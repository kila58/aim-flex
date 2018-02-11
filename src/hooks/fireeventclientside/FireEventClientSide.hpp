#pragma once

#include "../hooks.hpp"

class EventListener;

class FireEventClientSideHook : public BaseHook
{
private:
	EventListener* bullet_impact;
public:
	void Init();
	void Destroy();
};
