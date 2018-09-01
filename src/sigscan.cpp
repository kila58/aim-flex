#include <Windows.h>

#include "sigscan.hpp"

#include <psapi.h>
#include <string>
#include <algorithm>

std::string HexToBytes(std::string hex)
{
	std::string bytes;

	hex.erase(std::remove_if(hex.begin(), hex.end(), isspace), hex.end());

	for (uint i = 0; i < hex.length(); i += 2)
	{
		if ((uchar)hex[i] == '?')
		{
			bytes += '?';
			i -= 1;

			continue;
		}

		uchar byte = (uchar)std::strtol(hex.substr(i, 2).c_str(), nullptr, 16);
		bytes += byte;
	}

	return bytes;
}

ptr SigScan(const char* pattern, const char* module)
{
	HMODULE mod = GetModuleHandleA(module);
	MODULEINFO info;
	GetModuleInformation(GetCurrentProcess(), mod, &info, sizeof(info));

	uchar* base = (uchar*)mod;

	std::string signature = HexToBytes(pattern);

	uchar first = (uchar)signature.at(0);
	uchar* end = (base + info.SizeOfImage) - signature.length();

	for (; base < end; ++base)
	{
		if (*base != first)
			continue;

		uchar* bytes = base;
		uchar* sig = (uchar*)signature.c_str();

		for (; *sig; ++sig, ++bytes)
		{
			if (*sig == '?')
				continue;

			if (*bytes != *sig)
				goto end;
		}

		return (ptr)base;

	end:;
	}

	return NULL;
}
