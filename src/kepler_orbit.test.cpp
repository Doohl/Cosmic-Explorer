#include "kepler_orbit.h"

#include <iostream>
#include <fstream>

#include "doctest.h"
#include "json.hpp"
#include "utilities.h"

using json = nlohmann::json;

/**
 * 	Test to see if the planet Earth (using orbital parameters mostly taken from Wikipedia) can be simulated using KeplerOrbit logic
 */
TEST_CASE("Earth can be properly simulated using KeplerOrbit logic") {

	std::ifstream i("data/sol.json");
	json sol;
	i >> sol;

	double solarMass = sol["mass"]; // solar mass in kg
	Vec2 solarPosition = {0, 0};

	json earth = sol["children"][2];
	double earthMass = earth["mass"]; // earth mass in kg
	double standardGravTotal = (solarMass * Util::GRAV) + (earthMass * Util::GRAV);
	KeplerOrbit earthOrbit(earth["orbit"]["semimajorAxis"], earth["orbit"]["eccentricity"], earth["orbit"]["epochTime"], earth["orbit"]["epochAnomaly"], earth["orbit"]["lAscending"], earth["orbit"]["aPeriapsis"], standardGravTotal, true);

	SUBCASE("test Earth's computed static orbital elements") {
		CHECK(earthOrbit.getPeriapsis() == doctest::Approx(147098449.4729).epsilon(0.0001));
		CHECK(earthOrbit.getApoapsis() == doctest::Approx(152097596.5271).epsilon(0.0001));
		CHECK(earthOrbit.getSemiminorAxis() == doctest::Approx(149577139.3552).epsilon(0.0001));
		CHECK(earthOrbit.getLPeriapsis() == doctest::Approx(1.7967674211761813).epsilon(0.0001));
		CHECK(earthOrbit.getMeanAngularMotion() == doctest::Approx(-1.9910e-7).epsilon(0.0001));
		CHECK(earthOrbit.getPeriod() == doctest::Approx(1).epsilon(0.0001));
	}
	SUBCASE("test Earth's orbital center") {
		Vec2 center = earthOrbit.getCenter(solarPosition);
		CHECK(center.x == doctest::Approx(560036.6322).epsilon(0.0001));
		CHECK(center.y == doctest::Approx(2436026.8447).epsilon(0.0001));
	}
	SUBCASE("test conditions at t = 0") {
		universeTime t = 0.0;
		SUBCASE("test Earth's anomaly computations") {
			CHECK(earthOrbit.computeMeanAnomaly(t) == doctest::Approx(-6.2590).epsilon(0.0001));
			CHECK(earthOrbit.computeEccentricAnomaly(earthOrbit.computeMeanAnomaly(t)) == doctest::Approx(-6.2586).epsilon(0.0001));
			CHECK(earthOrbit.computeTrueAnomaly(t) == doctest::Approx(-6.2582).epsilon(0.0001));
		}
		SUBCASE("test Earth's position") {
			Vec2 position = earthOrbit.computeCoordinates(solarPosition, t);
			CHECK(position.x == doctest::Approx(-29369616.5890).epsilon(0.0001));
			CHECK(position.y == doctest::Approx(-144137437.9411).epsilon(0.0001));
		}
	}
	// As we go further forward in time we loosen our presumptions of where objects will be - the nature of IEEE 754
	SUBCASE("test conditions at t = 3.154e+9 (+100 years)") {
		universeTime t = 3.154e+9;
		SUBCASE("test Earth's anomaly computations") {
			CHECK(earthOrbit.computeMeanAnomaly(t) == doctest::Approx(-634.2271).epsilon(0.01));
			CHECK(earthOrbit.computeEccentricAnomaly(earthOrbit.computeMeanAnomaly(t)) == doctest::Approx(-634.2208).epsilon(0.01));
			CHECK(earthOrbit.computeTrueAnomaly(t) == doctest::Approx(-5.8961).epsilon(0.01));
		}
		SUBCASE("test Earth's position") {
			Vec2 position = earthOrbit.computeCoordinates(solarPosition, t);
			CHECK(position.x == doctest::Approx(23632183.6830).epsilon(0.01));
			CHECK(position.y == doctest::Approx(-145179325.3196).epsilon(0.01));
		}
	}
}

/**
 * 	Test to see if the Moon can be simulated using KeplerOrbit logic
 */
// TEST_CASE("The Moon can be properly simulated using KeplerOrbit logic") {
// 	double solarMass = 1.9885e30; // solar mass in kg
// 	Vec2 solarPosition = {0, 0};

// 	double earthMass = 5.97237e24; // earth mass in kg
// 	double standardGravTotal = (solarMass * Util::GRAV) + (earthMass * Util::GRAV);
// 	KeplerOrbit earthOrbit(149598023.0, 0.0167086, 0.0, 6.259047404, -0.196535244, 1.99330266505, standardGravTotal, true);

// 	// double lunarMass = 
// }