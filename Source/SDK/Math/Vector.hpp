#ifndef SDK_MATH_VECTOR
#define SDK_MATH_VECTOR

#include <cmath>
#include <algorithm>

class Vector {
public:
	float x, y, z;
	
	inline Vector() {
		x = y = z = 0;
	}
	inline Vector(const Vector& other) {
		x = other.x;
		y = other.y;
		z = other.z;
	}
	inline explicit Vector(float scalar) {
		x = y = z = scalar;
	}
	inline Vector(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// Assign
	inline Vector& operator=(const Vector& other) = default;

	// Array-like access
	inline float& operator[](int i) {
		return ((float*)this)[i];
	}

	inline float operator[](int i) const {
		return ((float*)this)[i];
	}

	// Arithmetic using mutable vectors

	// Arithmetic with other Vectors
	inline Vector& operator+=(const Vector& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	inline Vector& operator-=(const Vector& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	inline Vector& operator*=(const Vector& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	inline Vector& operator/=(const Vector& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	// Arithmetic with floating-point number
	inline Vector& operator+=(float other) {
		x += other;
		y += other;
		z += other;
		return *this;
	}

	inline Vector& operator-=(float other) {
		x -= other;
		y -= other;
		z -= other;
		return *this;
	}

	inline Vector& operator*=(float other) {
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	inline Vector& operator/=(float other) {
		x /= other;
		y /= other;
		z /= other;
		return *this;
	}

	// Comparison
	inline bool operator==(const Vector& other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	inline bool operator!=(const Vector& other) const {
		return x != other.x || y != other.y || z != other.z;
	}

	// Invert
	inline Vector operator-() const {
		return {-x, -y, -z};
	}

	// Arithmetic using non-mutable vectors

	// Arithmetic with other Vectors
	inline Vector operator+(const Vector& other) const {
		Vector vec;
		vec.x = x + other.x;
		vec.y = y + other.y;
		vec.z = z + other.z;
		return vec;
	}

	inline Vector operator-(const Vector& other) const {
		Vector vec;
		vec.x = x - other.x;
		vec.y = y - other.y;
		vec.z = z - other.z;
		return vec;
	}

	inline Vector operator*(const Vector& other) const {
		Vector vec;
		vec.x = x * other.x;
		vec.y = y * other.y;
		vec.z = z * other.z;
		return vec;
	}

	inline Vector operator/(const Vector& other) const {
		Vector vec;
		vec.x = x / other.x;
		vec.y = y / other.y;
		vec.z = z / other.z;
		return vec;
	}

	// Arithmetic with floating-point number
	inline Vector operator+(float other) const {
		Vector vec;
		vec.x = x + other;
		vec.y = y + other;
		vec.z = z + other;
		return vec;
	}

	inline Vector operator-(float other) const {
		Vector vec;
		vec.x = x - other;
		vec.y = y - other;
		vec.z = z - other;
		return vec;
	}

	inline Vector operator*(float other) const {
		Vector vec;
		vec.x = x * other;
		vec.y = y * other;
		vec.z = z * other;
		return vec;
	}

	inline Vector operator/(float other) const {
		Vector vec;
		vec.x = x / other;
		vec.y = y / other;
		vec.z = z / other;
		return vec;
	}

	inline float LengthSquared() const {
		return x * x + y * y + z * z;
	}

	inline float Length() const {
		return sqrt(LengthSquared());
	}

	inline float Dot(const Vector& other) const {
		return this->x * other.x + this->y * other.y + this->z * other.z;
	}

	inline Vector Normalize() const {
		float len = Length();
		return {
			this->x / len,
			this->y / len,
			this->z / len
		};
	}

	inline Vector Wrap() {
		while (this->x > 90.0f)
			this->x -= 180.f;

		while (this->x < -90.0f)
			this->x += 180.f;

		this->x = std::clamp(this->x, -89.0f, 89.0f);

		while (this->y > 180.f)
			this->y -= 360.f;

		while (this->y < -180.f)
			this->y += 360.f;
		this->z = 0.0f;
		return *this;
	}
};

class __attribute__((aligned(16))) AlignedVector : public Vector {
public:
	inline AlignedVector() = default;
	inline AlignedVector(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = 0;
	}

	AlignedVector& operator=(const Vector &vOther) {
		this->x = vOther.x;
		this->y = vOther.y;
		this->z = vOther.z;
		this->w = 0;
		return *this;
	}
	float w;
};

#endif