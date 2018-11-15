#ifndef VEC2_HPP
#define VEC2_HPP

struct Vec2 {
	float x;
	float y;
	Vec2(float x = 0, float y = 0) : x(x), y(y) {}
};

Vec2 operator+(Vec2 v1, Vec2 v2);
Vec2 operator-(Vec2 v1, Vec2 v2);
Vec2 operator*(float scalar, Vec2 v);
Vec2 operator/(Vec2 v, float scalar);
float dotProduct(Vec2 v1, Vec2 v2);

#endif