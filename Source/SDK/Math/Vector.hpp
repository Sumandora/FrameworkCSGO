#ifndef SDK_MATH_VECTOR
#define SDK_MATH_VECTOR

#include <algorithm>
#include <cmath>

#include "../../ConVarStorage.hpp"

struct Vector {
	float x, y, z;

	inline Vector()
	{
		x = y = z = 0;
	}
	inline Vector(const Vector& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}
	inline explicit Vector(float scalar)
	{
		x = y = z = scalar;
	}
	inline Vector(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// Assign
	inline Vector& operator=(const Vector& other) = default;

	// Array-like access
	inline float* Base()
	{
		return (float*)this;
	}

	inline float& operator[](int i)
	{
		return ((float*)this)[i];
	}

	inline float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	// Arithmetic using mutable vectors

	// Arithmetic with other Vectors
	inline Vector& operator+=(const Vector& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	inline Vector& operator-=(const Vector& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	inline Vector& operator*=(const Vector& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	inline Vector& operator/=(const Vector& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	// Arithmetic with floating-point number
	inline Vector& operator+=(float other)
	{
		x += other;
		y += other;
		z += other;
		return *this;
	}

	inline Vector& operator-=(float other)
	{
		x -= other;
		y -= other;
		z -= other;
		return *this;
	}

	inline Vector& operator*=(float other)
	{
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	inline Vector& operator/=(float other)
	{
		x /= other;
		y /= other;
		z /= other;
		return *this;
	}

	// Comparison
	inline bool operator==(const Vector& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	inline bool operator!=(const Vector& other) const
	{
		return x != other.x || y != other.y || z != other.z;
	}

	// Invert
	inline Vector operator-() const
	{
		return { -x, -y, -z };
	}

	// Arithmetic using non-mutable vectors

	// Arithmetic with other Vectors
	inline Vector operator+(const Vector& other) const
	{
		Vector vec;
		vec.x = x + other.x;
		vec.y = y + other.y;
		vec.z = z + other.z;
		return vec;
	}

	inline Vector operator-(const Vector& other) const
	{
		Vector vec;
		vec.x = x - other.x;
		vec.y = y - other.y;
		vec.z = z - other.z;
		return vec;
	}

	inline Vector operator*(const Vector& other) const
	{
		Vector vec;
		vec.x = x * other.x;
		vec.y = y * other.y;
		vec.z = z * other.z;
		return vec;
	}

	inline Vector operator/(const Vector& other) const
	{
		Vector vec;
		vec.x = x / other.x;
		vec.y = y / other.y;
		vec.z = z / other.z;
		return vec;
	}

	// Arithmetic with floating-point number
	inline Vector operator+(float other) const
	{
		Vector vec;
		vec.x = x + other;
		vec.y = y + other;
		vec.z = z + other;
		return vec;
	}

	inline Vector operator-(float other) const
	{
		Vector vec;
		vec.x = x - other;
		vec.y = y - other;
		vec.z = z - other;
		return vec;
	}

	inline Vector operator*(float other) const
	{
		Vector vec;
		vec.x = x * other;
		vec.y = y * other;
		vec.z = z * other;
		return vec;
	}

	inline Vector operator/(float other) const
	{
		Vector vec;
		vec.x = x / other;
		vec.y = y / other;
		vec.z = z / other;
		return vec;
	}

	[[nodiscard]] inline float HorizontalLengthSquared() const
	{
		return x * x + y * y;
	}

	[[nodiscard]] inline float HorizontalLength() const
	{
		return fsqrt(HorizontalLengthSquared());
	}

	[[nodiscard]] inline float LengthSquared() const
	{
		return x * x + y * y + z * z;
	}

	[[nodiscard]] inline float Length() const
	{
		return fsqrt(LengthSquared());
	}

	[[nodiscard]] inline float Dot(const Vector& other) const
	{
		return this->x * other.x + this->y * other.y + this->z * other.z;
	}

	[[nodiscard]] inline Vector Normalized() const
	{
		const float len = Length();
		return {
			this->x / len,
			this->y / len,
			this->z / len
		};
	}

	inline Vector Wrap()
	{
		// Use 360 in the remainder call and then clamp it to [-89..89] to prevent it from flipping the view
		this->x = std::remainderf(this->x, 360.0f);
		this->x = std::clamp(this->x, -ConVarStorage::cl_pitchup()->GetFloat(), ConVarStorage::cl_pitchdown()->GetFloat());

		this->y = std::remainderf(this->y, 360.0f);

		// Technically this could be [-50..50] (used to be [-90..90]) but there is no real point in having one of these angles besides HvHs.
		// There is no way to achieve such an angle under normal circumstances thus I rather zero it instead of going with something else.
		this->z = 0.0f;

		return *this;
	}
};

class __attribute__((aligned(16))) AlignedVector : public Vector {
public:
	inline AlignedVector() = default;
	inline AlignedVector(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = 0;
	}

	AlignedVector& operator=(const Vector& vOther)
	{
		this->x = vOther.x;
		this->y = vOther.y;
		this->z = vOther.z;
		this->w = 0;
		return *this;
	}
	float w;
};

#endif
