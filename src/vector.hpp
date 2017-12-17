#pragma once

typedef float VMatrix[4][4];

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

	float Length() const;
	float Length2D() const;
	float Length2DSqr() const;
	float Distance(const Vector&) const;
	float Distance2D(const Vector&) const;
	float Distance2DSqr(const Vector&) const;
	float Dot(const Vector&) const;
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

inline float Vector::Length() const
{
	return std::sqrt((x * x) + (y * y) + (z * z));
}

inline float Vector::Length2D() const
{
	return std::sqrt((x * x) + (y * y));
}

inline float Vector::Length2DSqr() const
{
	return ((x * x) + (y * y));
}

inline float Vector::Distance(const Vector& vec) const
{
	return (*this - vec).Length();
}

inline float Vector::Distance2D(const Vector& vec) const
{
	return (*this - vec).Length2D();
}

inline float Vector::Distance2DSqr(const Vector& vec) const
{
	return (*this - vec).Length2DSqr();
}

inline float Vector::Dot(const Vector& vec) const
{
	return x * vec.x + y * vec.y + z * vec.z;
}

inline void VectorTransform(const Vector& vec, const VMatrix& matrix, Vector& out)
{
	out.x = vec.Dot(Vector(matrix[0][0], matrix[0][1], matrix[0][2])) + matrix[0][3];
	out.y = vec.Dot(Vector(matrix[1][0], matrix[1][1], matrix[1][2])) + matrix[1][3];
	out.z = vec.Dot(Vector(matrix[2][0], matrix[2][1], matrix[2][2])) + matrix[2][3];
}

static const float pi = 3.14159265358979323846f;

inline float Rad2Deg(float rad)
{
	return rad * (180.f / pi);
}

inline float Deg2Rad(float deg)
{
	return deg * (pi / 180.f);
}

inline void VectorAngles(const Vector &vec, Angle &angles)
{
	if (vec.x == 0.0f && vec.y == 0.0f)
	{
		if (vec.z > 0.0f)
		{
			angles.p = -90.0f;
		}
		else
		{
			angles.p = 90.0f;
		}
	}
	else
	{
		angles.p = Rad2Deg(atan2(-vec.z, vec.Length2D()));
		angles.y = Rad2Deg(atan2(vec.y, vec.x));
	}
}