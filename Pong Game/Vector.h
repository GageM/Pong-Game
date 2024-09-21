#pragma once

#include <math.h>

struct Vector2
{
	inline Vector2() : x(0.0f), y(0.0f) {}
	inline Vector2(const float& x_, const float& y_) : x(x_), y(y_) {}

	// Helper Defaults
	inline static Vector2 up() { return Vector2(0.0f, 1.0f); }
	inline static Vector2 right() { return Vector2(1.0f, 0.0f); }
	inline static Vector2 one() { return Vector2(1.0f, 1.0f); }

	// Vector Functions
	inline static float Magnitude(Vector2& v) { return (float)sqrt((v.x * v.x) + (v.y * v.y)); }
	inline static Vector2 Normalize(Vector2 v) { return Vector2(v.x / Magnitude(v), v.y / Magnitude(v)); }
	inline static float Dot(Vector2 a, Vector2 b) { Vector2 m = a * b; return(m.x + m.y); }


	// Operator Overloads
	inline Vector2 operator+(const Vector2& other) { return Vector2(x + other.x, y + other.y); }
	inline Vector2 operator+=(const  Vector2& other) { *this = *this + other; return *this; }

	inline Vector2 operator-(const Vector2& other) { return Vector2(x - other.x, y - other.y); }
	inline Vector2 operator-=(const  Vector2& other) { *this = *this - other; return *this; }

	inline Vector2 operator*(const Vector2& other) { return Vector2(x * other.x, y * other.y); }
	inline Vector2 operator*=(const  Vector2& other) { *this = *this * other; return *this; }

	inline Vector2 operator*(const float& other) { return Vector2(x * other, y * other); }
	inline Vector2 operator*=(const  float& other) { *this = *this * other; return *this; }

	inline Vector2 operator/(const Vector2& other) { return Vector2(x / other.x, y / other.y); }
	inline Vector2 operator/=(const  Vector2& other) { *this = *this / other; return *this; }

	inline Vector2 operator/(const float& other) { return Vector2(x / other, y / other); }
	inline Vector2 operator/=(const  float& other) { *this = *this / other; return *this; }


	float x, y;
};

struct Vector3
{
	inline Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	inline Vector3(const float& x_, const float& y_, const float& z_) : x(x_), y(y_), z(z_) {}

	// Helper Defaults
	inline static Vector3 up() { return Vector3(0.0f, 1.0f, 0.0f); }
	inline static Vector3 right() { return Vector3(1.0f, 0.0f, 0.0f); }
	inline static Vector3 forward() { return Vector3(0.0f, 0.0f, 1.0f); }
	inline static Vector3 one() { return Vector3(1.0f, 1.0f, 1.0f); }

	// Vector Functions
	inline static float Magnitude(Vector3 v) { return (float)sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); }
	inline static Vector3 Normalize(Vector3 v) { return Vector3(v.x / Magnitude(v), v.y / Magnitude(v), v.z / Magnitude(v)); }
	inline static float Dot(Vector3 a, Vector3 b) { Vector3 m = a * b; return(m.x + m.y + m.z); }
	inline static Vector3 Cross(Vector3 a, Vector3 b) { return Vector3( (a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (b.x * a.y)); }

	// Operator Overloads
	inline Vector3 operator+(const Vector3& other) { return Vector3(x + other.x, y + other.y, z + other.z); }

	inline Vector3 operator-(const Vector3& other) { return Vector3(x - other.x, y - other.y, z - other.z); }

	inline Vector3 operator*(const Vector3& other) { return Vector3(x * other.x, y * other.y, z * other.z); }
	inline Vector3 operator*(const float& other) { return Vector3(x * other, y * other, z * other); }

	inline Vector3 operator/(const Vector3& other) { return Vector3(x / other.x, y / other.y, z / other.z); }
	inline Vector3 operator/(const float& other) { return Vector3(x / other, y / other, z / other); }


	float x, y, z;
};
