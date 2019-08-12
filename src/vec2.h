#pragma once

// A simple 2D vector for storing directions and points in space
struct Vec2 {
	Vec2()
		: x(0.0), y(0.0)
	{}
	Vec2(double _x, double _y)
		: x(_x), y(_y)
	{}

	// Set a new vector position
	void set(const double& _x, const double& _y);

	double x, y;
};

// Return a rotated point along an x and y origin
Vec2 rotatePoint(Vec2 point, const Vec2& origin, const double& angle);