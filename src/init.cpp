#include "init.hpp"

#include "aim-flex.hpp"

#include "interfaces.hpp"

void Init()
{
	if (interfaces.Init())
		MessageBoxA(NULL, "rare", "rare", NULL);
}
