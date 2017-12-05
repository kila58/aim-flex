#pragma once

class Vector
{
public:
	Vector() {}
	Vector(float x, float y, float z) : x(x), y(y), z(z) {}

	float x, y, z;

	inline float operator[](int index)
	{
		return ((float*)this)[index];
	}

	inline const float operator[](int index) const
	{
		return ((float*)this)[index];
	}
};

typedef float VMatrix[4][4];
