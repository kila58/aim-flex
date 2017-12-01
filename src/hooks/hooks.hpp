#pragma once

#include "../hook.hpp"

#include <deque>

class BaseHook
{
private:
	static std::deque<BaseHook*> hooks;
protected:
	BaseHook()
	{
		AddHook(this);
	}
	Hook* hook;
public:
	static std::deque<BaseHook*> GetHooks()
	{
		return hooks;
	};
	static void AddHook(BaseHook* hook)
	{
		hooks.emplace_back(hook);
	}
	virtual void Init() = 0;
	virtual void Destory() = 0;
};

class Hooks
{
public:
	void Init();
};

extern Hooks hooks;
