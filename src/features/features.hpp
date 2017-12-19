#pragma once

#include <deque>
#include <unordered_map>
#include <any>

#include "../definitions.hpp"

using ArgsArrayType = std::deque<std::any>;

class BaseFeature
{
private:
	HookType type = INVALID;
	uint priority = 0;
	static std::deque<BaseFeature*> features;
	// todo: multiple dispatch instead of this solution for arguments
	static std::unordered_map<HookType, ArgsArrayType> arguments;
protected:
	BaseFeature(HookType type, uint priority) : type(type), priority(priority)
	{
		AddFeature(this);
	}
	BaseFeature(HookType type) : type(type)
	{
		AddFeature(this);
	}
	BaseFeature(uint priority) : priority(priority)
	{
		AddFeature(this);
	}
	BaseFeature()
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
	static const ArgsArrayType& GetArguments(HookType type)
	{
		return arguments[type];
	}
	const ArgsArrayType& GetArguments()
	{
		return arguments[type];
	}
	template<typename T>
	static T GetArg(const ArgsArrayType& array, uint index)
	{
		return std::any_cast<T>(array.at(index));
	}
	template<typename... Args>
	static void SetArguments(HookType type, const Args&... args)
	{
		arguments.clear();

		(arguments[type].push_back((args)), ...);
	}
	virtual HookType GetHookType()
	{
		return type;
	}
	virtual uint GetPriority()
	{
		return priority;
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
	void Destroy();
};

extern Features features;
