#pragma once

#include <deque>

#include "../definitions.hpp"

class BaseFeature
{
private:
	HookType type = INVALID;
	static std::deque<BaseFeature*> features;
protected:
	BaseFeature(HookType type) : type(type)
	{
		AddFeature(this);
	}
public:
	static std::deque<BaseFeature*> GetFeatures()
	{
		return features;
	};
	static void AddFeature(BaseFeature* feature)
	{
		features.emplace_back(feature);
	}
	virtual HookType GetHookType()
	{
		return type;
	}
	virtual void Init() = 0;
	virtual void Invoke() = 0;
	virtual void Destroy() = 0;
};

class Features
{
public:
	void Init();
	void Invoke(HookType type);
};

extern Features features;
