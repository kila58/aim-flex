#pragma once

#include <string>

class Interfaces
{
public:
	bool Init();
	template<typename T> T GetInterface(const char* module, const std::string& name);
};

extern Interfaces interfaces;
