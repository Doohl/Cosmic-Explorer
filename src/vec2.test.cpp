#include "vec2.h"

#include "doctest.h"
#include "utilities.h"

TEST_CASE("Vec2 class functions") {
	Vec2 testVec(5.0, -5512.0);
	
	SUBCASE("Vec2 constructors") {
		CHECK(testVec.x == 5);
		CHECK(testVec.y == -5512.0);

		Vec2 testVec2 = {10, 10};
		CHECK(testVec2.x == 10);
		CHECK(testVec2.y == 10);
	}
	SUBCASE("Vec2 double-precision accuracy test") {
		testVec.set(5.667821e-30, 5e22);
		CHECK(testVec.x == doctest::Approx(5.667821e-30).epsilon(0.0001));
		CHECK(testVec.y == doctest::Approx(5e22).epsilon(0.0001));
	}
}

TEST_CASE("Rotating a Vec2") {
	Vec2 point = {100, 100};
	Vec2 origin = {-20, -20};
	Vec2 rotated = rotatePoint(point, origin, Util::PI / 4);
	CHECK(rotated.x == doctest::Approx(-20).epsilon(0.0001));
	CHECK(rotated.y == doctest::Approx(149.705627).epsilon(0.0001));
}