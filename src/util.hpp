#pragma once

#include <cmath>
#include <Windows.h>

#include <intrin.h>

#include "definitions.hpp"

#undef min
#undef max

class Vector;

bool WorldToScreen(const Vector& point, Vector& screen);

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
template<class Rng>
class Reverse
{
	Rng& object;
public:
	Reverse(Rng& object) noexcept : object(object) {}

	auto begin() const noexcept
	{
		return std::make_reverse_iterator(std::end(object));
	}

	auto end() const noexcept
	{
		return std::make_reverse_iterator(std::begin(object));
	}
};
inline float lerp_axis(float a, float b, float f)
{
	return a + f * normalize(b - a);
}
