#pragma once

// A simple 2D vector for storing directions and points in space
struct Vec2 {
	double x = 0.0;
	double y = 0.0;

	Vec2(double _x, double _y)
		: x(_x), y(_y)
	{}
	Vec2()
		: x(0.0), y(0.0)
	{}

	// Set a new vector position
	void set(double _x, double _y);
	void set(const Vec2& vec);
};

// A simple 4D vector, generally for storing color data
struct Vec4 {
	double r = 0.0f;
	double g = 0.0f;
	double b = 0.0f;
	double a = 1.0f;

	Vec4(double _r, double _g, double _b, double _a)
		: r(_r), g(_g), b(_b), a(_a)
	{}
	Vec4(double _r, double _g, double _b)
		: r(_r), g(_g), b(_b), a(1.0)
	{}
	Vec4()
		: r(0.0), g(0.0), b(0.0), a(1.0)
	{}
};

// Return a rotated point along an x and y origin
Vec2 rotatePoint(Vec2 point, const Vec2& origin, double angle);


#include <ostream>

std::ostream& operator<<(std::ostream& out, const Vec2& vec);