#pragma once

#undef min
#undef max

class Vector;

bool WorldToScreen(const Vector& point, Vector& screen);

template <typename T>
inline T clamp(const T& val, const T& min, const T& max)
{
	return std::max(min, std::min(val, max));
}
