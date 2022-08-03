#ifndef SDK_VECTOR
#define SDK_VECTOR

#include <math.h>

class Vector {
public:
	float x, y, z;
	
	inline Vector() {
		x = y = z = 0;
	}
	inline Vector(float scalar) {
		x = y = z = scalar;
	}
	inline Vector(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline Vector operator+(Vector& other) {
		return Vector(
			x + other.x,
			y + other.y,
			z + other.z
		);
	}

	inline Vector operator-(Vector& other) {
		return Vector(
			x - other.x,
			y - other.y,
			z - other.z
		);
	}

	inline Vector operator*(Vector& other) {
		return Vector(
			x * other.x,
			y * other.y,
			z * other.z
		);
	}

	inline Vector operator/(Vector& other) {
		return Vector(
			x / other.x,
			y / other.y,
			z / other.z
		);
	}

	inline Vector& operator+=(Vector& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	inline Vector& operator-=(Vector& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	inline Vector& operator*=(Vector& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	inline Vector& operator/=(Vector& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	inline Vector operator+(float other) {
		return Vector(
			x + other,
			y + other,
			z + other
		);
	}

	inline Vector operator-(float other) {
		return Vector(
			x - other,
			y - other,
			z - other
		);
	}

	inline Vector operator*(float other) {
		return Vector(
			x * other,
			y * other,
			z * other
		);
	}

	inline Vector operator/(float other) {
		return Vector(
			x / other,
			y / other,
			z / other
		);
	}

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

	inline float& operator[](int i) {
		return ((float*)this)[i];
	}

	inline float operator[](int i) const {
		return ((float*)this)[i];
	}

	inline float Length() {
		return sqrt(x * x + y * y + z * z);
	}

	inline float Dot(Vector other) {
		return this->x * other.x + this->y * other.y;
	}

	inline Vector Normalize() {
		float len = Length();
		return Vector(
			this->x / len,
			this->y / len,
			this->z / len
		);
	}

	inline Vector Wrap() {
		while (angle.x > 89.0f)
			angle.x -= 180.f;

		while (angle.x < -89.0f)
			angle.x += 180.f;

		while (angle.y > 180.f)
			angle.y -= 360.f;

		while (angle.y < -180.f)
			angle.y += 360.f;
		z = 0.0f;
		return *this;
	}
};

#endif
