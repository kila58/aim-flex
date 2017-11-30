#pragma once

#include <string>

class Interfaces
{
public:
	bool Init();
	template<class T> T	GetInterface(const char* module, const std::string& name);
};

extern Interfaces interfaces;
