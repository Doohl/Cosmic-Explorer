#include "vec2.h"

#include "doctest.h"
#include <iostream>

TEST_CASE("Vec2 constructor works") {
	std::cout << "test";
	Vec2 testVec(5.0, -5512.0);
	CHECK(testVec.x == 5);
	CHECK(testVec.y == -5512.0);
}
