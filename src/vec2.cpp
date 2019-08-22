#include "vec2.h"

#include <cmath>

Vec2 rotatePoint(Vec2 point, const Vec2& origin, double angle) {
	double sin = std::sin(angle);
	double cos = std::cos(angle);

	point.x -= origin.x;
	point.y -= origin.y;

	double relX = point.x * cos - point.y * sin;
	double relY = point.x * sin + point.y * cos;

	point.x = relX + origin.x;
	point.y = relY + origin.y;
	return point;
}

void Vec2::set(double _x, double _y) {
	x = _x;
	y = _y;
}

void Vec2::set(const Vec2& vec) {
	x = vec.x;
	y = vec.y;
}