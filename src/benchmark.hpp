#pragma once

#include <chrono>
#include <unordered_map>
#include <utility>

using Time = std::chrono::time_point<std::chrono::steady_clock>;
using Result = std::pair<Time, Time>;
using MsFloat = std::chrono::duration<float, std::chrono::milliseconds::period>;

class Benchmark
{
private:
	std::unordered_map<const char*, Result> times;
public:
	void Start(const char* name);
	void End(const char* name);
	float GetResult(const char* name);
};

extern Benchmark benchmark;
