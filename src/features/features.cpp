#include "features.hpp"

#include <algorithm>

std::deque<BaseFeature*> BaseFeature::features;
std::unordered_map<HookType, ArgsArrayType> BaseFeature::arguments;

void Features::Init()
{
	auto& features = BaseFeature::GetFeatures();

	for (auto& feature : features)
		feature->Init();

	std::sort(features.begin(), features.end(), [](BaseFeature* a, BaseFeature* b)
	{
		return a->GetPriority() > b->GetPriority();
	});
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
