#pragma once

#include <string>

class Interfaces
{
public:
	bool Init();
	template<typename T> T GetInterface(const char* module, const std::string_view& name);
};

extern Interfaces interfaces;
