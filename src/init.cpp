#include "init.hpp"

#include "aim-flex.hpp"

#include "interfaces.hpp"

#include "hooks\hooks.hpp"

void Init()
{
	if (interfaces.Init())
	{
		hooks.Init();
<<<<<<< HEAD
=======

		MessageBox(NULL, "big epic", "big epic", NULL);
>>>>>>> 1ca4338319b59e3df8f6fdd429fca15760e6e890
	}
}
