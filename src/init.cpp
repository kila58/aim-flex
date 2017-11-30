#include "init.hpp"

#include "aim-flex.hpp"

#include "interfaces.hpp"

#include "hooks\paint\paint.hpp"

void Init()
{
	if (interfaces.Init())
		MessageBoxA(NULL, "rare", "rare", NULL);

	PaintHook::Init();
}
