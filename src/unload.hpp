#pragma once

class Unload
{
public:
	void Poll(HMODULE module, bool force = false);
};

extern Unload unload;
