#pragma once

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