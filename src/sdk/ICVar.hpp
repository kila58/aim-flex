#pragma once

class ICVar
{
public:
	template<typename... Args>
	void ConsoleColorPrintf(const std::string& str, const Args&... args)
	{
		const Color white(255, 255, 255);

		getvfunc<void(__cdecl*)(void*, const Color&, const char*, ...)>(this, 25)(this, white, str.c_str(), args...);
	}
};
