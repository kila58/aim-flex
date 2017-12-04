#include "init.hpp"

#include "interfaces.hpp"

#include "hooks/hooks.hpp"

#include "features/features.hpp"

void Init()
{
	if (interfaces.Init())
	{
		hooks.Init();
		features.Init();
	}
}
