#pragma once

class Color
{
public:
	inline Color() {}
	inline Color(int r, int g, int b) : r(r), g(g), b(b), a(255) {}
	inline Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}

	unsigned char r, g, b, a;
};
