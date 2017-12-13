#pragma once

class Unload
{
public:
	void Poll(HMODULE module);
};

extern Unload unload;
