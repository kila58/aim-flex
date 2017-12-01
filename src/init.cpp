#include "init.hpp"

#include "aim-flex.hpp"

#include "interfaces.hpp"

#include "hooks\hooks.hpp"

void Init()
{
	if (interfaces.Init())
	{
		hooks.Init();
	}
}
