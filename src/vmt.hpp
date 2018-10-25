#pragma once

#include "definitions.hpp"

inline ptr getvfunc(void* table, size_t index)
{
	if (!table)
	{
		MessageBoxA(NULL, "kll sellf", "kill self", NULL);
	}

	return ((*(ptr**)table)[index]);
}

template <typename T>
inline T getvfunc(void* table, size_t index)
{
	return T(getvfunc(table, index));
}
