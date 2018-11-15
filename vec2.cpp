#include "vec2.hpp"

Vec2 operator+(Vec2 v1, Vec2 v2) {
	Vec2 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	return result;
}

Vec2 operator-(Vec2 v1, Vec2 v2) {
	Vec2 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	return result;
}

Vec2 operator*(float scalar, Vec2 v) {
	Vec2 result;
	result.x = v.x * scalar;
	result.y = v.y * scalar;
	return result;
}

Vec2 operator/(Vec2 v, float scalar) {
	Vec2 result;
	result.x = v.x / scalar;
	result.y = v.y / scalar;
	return result;
}

float dotProduct(Vec2 v1, Vec2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}