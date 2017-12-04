#pragma once

#include "definitions.hpp"

template <typename T>
inline T getvfunc(void* table, size_t index)
{
	return T((*(ptr**)table)[index]);
}

inline ptr getvfunc(void* table, size_t index)
{
	return ((*(ptr**)table)[index]);
}
