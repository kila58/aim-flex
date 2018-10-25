#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <string>

struct DLLInfo
{
	HMODULE module;
	std::string jwt_token;
};

void Init(DLLInfo* info);
