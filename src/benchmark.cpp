#include "benchmark.hpp"

// if < c++14 use QueryPerformanceCounter
// this could be constexpr (or a fat macro) if we don't want it too dynamic
void Benchmark::Start(const char* name)
{
	times[name].first = std::chrono::high_resolution_clock::now();
}

void Benchmark::End(const char* name)
{
	times[name].second = std::chrono::high_resolution_clock::now();
}

float Benchmark::GetResult(const char* name)
{
	if (times.find(name) != times.end())
	{
		auto result = times[name];

		return MsFloat(result.second - result.first).count();
	}

	return 0.f;
}

Benchmark benchmark;
