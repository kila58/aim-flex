#pragma once

class Vector
{
public:
	Vector() {}
	Vector(float x, float y, float z) : x(x), y(y), z(z) {}

	float x, y, z;

	inline float operator[](int index)
	{
		switch (index)
		{
		case 1:
			return x;
		case 2:
			return y;
		case 3:
			return z;
		default:
			return 0.f;
		}
	}

	inline const float operator[](int index) const
	{
		switch (index)
		{
		case 1:
			return x;
		case 2:
			return y;
		case 3:
			return z;
		default:
			return 0.f;
		}
	}
};

class VMatrix
{
public:
	float matrix[4][4];

	inline float* operator[](int index)
	{
		return matrix[index];
	}
	inline const float* operator[](int index) const
	{
		return matrix[index];
	}
};
