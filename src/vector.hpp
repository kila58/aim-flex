#pragma once

class Vector
{
public:
	Vector() {}
	Vector(float x, float y, float z) : x(x), y(y), z(z) {}

	float x, y, z;

	float operator[](int);
	float const operator[](int) const;

	Vector operator-(const Vector&) const;
	Vector operator-(const float) const;
	Vector operator*(const float) const;
	Vector operator+(const Vector&) const;
};

inline float Vector::operator[](int index)
{
	return ((float*)this)[index];
}

inline float const Vector::operator[](int index) const
{
	return ((float*)this)[index];
}

inline Vector Vector::operator-(const Vector& vec) const
{
	return Vector(x - vec.x, y - vec.y, z - vec.z);
}

inline Vector Vector::operator-(const float f) const
{
	return Vector(x - f, y - f, z - f);
}

inline Vector Vector::operator*(const float f) const
{
	return Vector(x * f, y * f, z * f);
}

inline Vector Vector::operator+(const Vector& vec) const
{
	return Vector(x + vec.x, y + vec.y, z + vec.z);
}

typedef float VMatrix[4][4];
