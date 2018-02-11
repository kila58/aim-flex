#pragma once

#include <cmath>
#include <Windows.h>

#include <intrin.h>

#include "definitions.hpp"

#undef min
#undef max

class Vector;

bool WorldToScreen(const Vector& point, Vector& screen);

template <typename T>
inline T clamp(const T& val, const T& min, const T& max)
{
	return std::max(min, std::min(val, max));
}
inline float normalize(const float& f)
{
	return std::remainderf(f, 360.f);
}
using RandomFloatType = float(__cdecl*)(float, float);
inline float RandomFloat(float min, float max)
{
	static auto RandomFloatFn = (RandomFloatType)(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));

	return RandomFloatFn(min, max);
}
using RandomSeedType = void(__cdecl*)(int);
inline void RandomSeed(int seed)
{
	static auto RandomSeedFn = (RandomSeedType)(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed"));

	return RandomSeedFn(seed);
}
inline ptr* GetEBP()
{
	return (ptr*)(((char*)_AddressOfReturnAddress()) - sizeof(void*));
}
