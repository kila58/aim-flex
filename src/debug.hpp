#pragma once

#include <sstream>

class Debug
{
private:
	std::ostringstream stream;
public:
	template<typename T>
	Debug& operator<<(T val)
	{
		stream << val;

		cvar->ConsoleColorPrintf(stream.str().c_str());

		stream = std::ostringstream();

		return *this;
	}
};

extern Debug debug;
