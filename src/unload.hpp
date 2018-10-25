#pragma once

class Unload
{
public:
	void Poll(DLLInfo* info, bool force = false);
};

extern Unload unload;
