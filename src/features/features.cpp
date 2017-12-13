#include "features.hpp"

std::deque<BaseFeature*> BaseFeature::features;
std::unordered_map<HookType, ArgsArrayType> BaseFeature::arguments;

void Features::Init()
{
	for (auto& feature : BaseFeature::GetFeatures())
		feature->Init();
}

void Features::Invoke(HookType type)
{
	for (auto& feature : BaseFeature::GetFeatures())
	{
		if (feature->GetHookType() == type)
			feature->Invoke();
	}
}

void Features::Destroy()
{
	for (auto& feature : BaseFeature::GetFeatures())
	{
		feature->Destroy();
	}
}

Features features;
