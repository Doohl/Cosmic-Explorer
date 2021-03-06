#include "kepler_orbit.h"

#include <iostream>
#include <fstream>

#include "doctest.h"
#include "dummy_data.h"
#include "utilities.h"

void testEarthInit(KeplerOrbit& earthOrbit) {
	CHECK(earthOrbit.getPeriapsis() == doctest::Approx(147098449.4729).epsilon(0.0001));
	CHECK(earthOrbit.getApoapsis() == doctest::Approx(152097596.5271).epsilon(0.0001));
	CHECK(earthOrbit.getSemiminorAxis() == doctest::Approx(149577139.3552).epsilon(0.0001));
	CHECK(earthOrbit.getLPeriapsis() == doctest::Approx(1.7967674211761813).epsilon(0.0001));
	CHECK(earthOrbit.getMeanAngularMotion() == doctest::Approx(-1.9910e-7).epsilon(1e-10));
	CHECK(earthOrbit.getPeriod() == doctest::Approx(1).epsilon(1e-7));
}

void testMoonInit(KeplerOrbit& moonOrbit) {
	CHECK(moonOrbit.getPeriapsis() == doctest::Approx(363295.4949).epsilon(0.0001));
	CHECK(moonOrbit.getApoapsis() == doctest::Approx(405502.5051).epsilon(0.0001));
	CHECK(moonOrbit.getSemiminorAxis() == doctest::Approx(383819.2716).epsilon(0.0001));
	CHECK(moonOrbit.getLPeriapsis() == doctest::Approx(7.735822843614466).epsilon(0.0001));
	CHECK(moonOrbit.getMeanAngularMotion() == doctest::Approx(-0.0000026653).epsilon(1e-10));
	CHECK(moonOrbit.getPeriod() == doctest::Approx(0.0001302).epsilon(1e-7));
}

TEST_SUITE("KeplerOrbit") {

	/**
	 * 	Test to see if the planet Earth (using orbital parameters mostly taken from Wikipedia) can be simulated using KeplerOrbit logic
	 */
	TEST_CASE("Earth can be properly simulated using KeplerOrbit logic") {
		double solarMass = DummyData::Sol["mass"]; // solar mass in kg
		Vec2 solarPosition = {0, 0};

		json earth = DummyData::Sol["children"][2];
		double earthMass = earth["mass"]; // earth mass in kg
		double standardGravTotal = (solarMass * Util::Grav) + (earthMass * Util::Grav);
		universeTime epoch = KeplerOrbit::GetEpochTime(earth["orbit"]);
		
		KeplerOrbit earthOrbit(earth["orbit"]["semimajorAxis"], earth["orbit"]["eccentricity"], epoch, earth["orbit"]["meanAnomaly"], earth["orbit"]["lAscending"], earth["orbit"]["aPeriapsis"], standardGravTotal, true);

		SUBCASE("test Earth's computed static orbital elements") {
			testEarthInit(earthOrbit);
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
	TEST_CASE("The Moon can be properly simulated using KeplerOrbit logic") {
		double solarMass = DummyData::Sol["mass"]; // solar mass in kg
		Vec2 solarPosition = {0, 0};

		json earth = DummyData::Sol["children"][2];
		double earthMass = earth["mass"]; // earth mass in kg
		double standardGravTotal = (solarMass * Util::Grav) + (earthMass * Util::Grav);
		KeplerOrbit earthOrbit(earth["orbit"]["semimajorAxis"], earth["orbit"]["eccentricity"], KeplerOrbit::GetEpochTime(earth["orbit"]), earth["orbit"]["meanAnomaly"], earth["orbit"]["lAscending"], earth["orbit"]["aPeriapsis"], standardGravTotal, true);

		json moon = earth["children"][0];
		double lunarMass = moon["mass"];
		double standardGravTotalMoon = (earthMass * Util::Grav) + (lunarMass * Util::Grav);
		KeplerOrbit moonOrbit(moon["orbit"]["semimajorAxis"], moon["orbit"]["eccentricity"], KeplerOrbit::GetEpochTime(moon["orbit"]), moon["orbit"]["meanAnomaly"], moon["orbit"]["lAscending"], moon["orbit"]["aPeriapsis"], standardGravTotalMoon, true);


		SUBCASE("test Moon's computed static orbital elements") {
			testMoonInit(moonOrbit);
		}
		SUBCASE("test conditions at t = 0") {
			universeTime t = 0.0;
			SUBCASE("test Moon's anomaly computations") {
				CHECK(moonOrbit.computeMeanAnomaly(t) == doctest::Approx(-2.2149).epsilon(0.0001));
				CHECK(moonOrbit.computeEccentricAnomaly(moonOrbit.computeMeanAnomaly(t)) == doctest::Approx(-2.25743).epsilon(0.0001));
				CHECK(moonOrbit.computeTrueAnomaly(t) == doctest::Approx(-2.2991).epsilon(0.0001));
			}
			SUBCASE("test Moon's position (relative to the Sun)") {
				Vec2 earthPosition = earthOrbit.computeCoordinates(solarPosition, t);
				Vec2 position = moonOrbit.computeCoordinates(earthPosition, t);
				CHECK(position.x == doctest::Approx(-29695629.5249).epsilon(0.0001));
				CHECK(position.y == doctest::Approx(-143909480.6461).epsilon(0.0001));
			}
		}
	}

	TEST_CASE("Initializing KeplerOrbit with JSON") {
		double solarMass = DummyData::Sol["mass"];

		json earth = DummyData::Sol["children"][2];
		double earthMass = earth["mass"]; // earth mass in kg
		double standardGravTotal = (solarMass * Util::Grav) + (earthMass * Util::Grav);

		KeplerOrbit earthOrbit(earth["orbit"], standardGravTotal);

		SUBCASE("test Earth's computed static orbital elements") {
			testEarthInit(earthOrbit);
		}
	}

	TEST_CASE("KeplerOrbit::GetAnomaly") {
		json orbit = {
			{"meanAnomalyDeg", "55.2"}
		};
		CHECK(KeplerOrbit::GetAnomaly(orbit) == doctest::Approx(0.9634217).epsilon(0.00001));

		orbit["meanAnomalyDeg"] = "55.2 12.0";
		CHECK(KeplerOrbit::GetAnomaly(orbit) == doctest::Approx(0.9634217 + 0.00349066).epsilon(0.00001));

		orbit["meanAnomalyDeg"] = "55.2 12.0 24.11";
		CHECK(KeplerOrbit::GetAnomaly(orbit) == doctest::Approx(0.9634217 + 0.00349066 + 0.00010234417).epsilon(0.00001));

		orbit["meanAnomalyDeg"] = "55.2 12.0 24.11 11";
		CHECK(KeplerOrbit::GetAnomaly(orbit) == doctest::Approx(0.9634217 + 0.00349066 + 0.00010234417).epsilon(0.00001));

		orbit["meanAnomalyDeg"] = "";
		CHECK_THROWS(KeplerOrbit::GetAnomaly(orbit));
	}

	TEST_CASE("Malformed Kepler Orbits") {
		SUBCASE("Semimajor axis must be > 0") {
			CHECK_THROWS_WITH(KeplerOrbit(-100, 0.5, 0.0, 0.0, 0.0, 0.0, 1000.0, false), "Invalid semimajor axis");
			CHECK_THROWS_WITH(KeplerOrbit(0, 0.5, 0.0, 0.0, 0.0, 0.0, 1000.0, false), "Invalid semimajor axis");
			CHECK_NOTHROW(KeplerOrbit(100, 0.5, 0.0, 0.0, 0.0, 0.0, 1000.0, false));
		}
		
		SUBCASE("Eccentricity must be within [0, 1)") {
			CHECK_THROWS_WITH(KeplerOrbit(100.0, 50, 0.0, 0.0, 0.0, 0.0, 1000.0, false), "Eccentricity out of bounds");
			CHECK_THROWS_WITH(KeplerOrbit(100.0, 1, 0.0, 0.0, 0.0, 0.0, 1000.0, false), "Eccentricity out of bounds");
			CHECK_THROWS_WITH(KeplerOrbit(100.0, -1, 0.0, 0.0, 0.0, 0.0, 1000.0, false), "Eccentricity out of bounds");
			CHECK_NOTHROW(KeplerOrbit(100.0, 0, 0.0, 0.0, 0.0, 0.0, 1000.0, false));
			CHECK_NOTHROW(KeplerOrbit(100.0, 0.5, 0.0, 0.0, 0.0, 0.0, 1000.0, false));
			CHECK_NOTHROW(KeplerOrbit(100.0, 0.9999, 0.0, 0.0, 0.0, 0.0, 1000.0, false));
		}

		SUBCASE("Standard grav total must be > 0") {
			CHECK_THROWS_WITH(KeplerOrbit(100.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0, false), "Invalid standard grav total");
			CHECK_THROWS_WITH(KeplerOrbit(100.0, 0.5, 0.0, 0.0, 0.0, 0.0, -100.0, false), "Invalid standard grav total");
			CHECK_NOTHROW(KeplerOrbit(100.0, 0.5, 0.0, 0.0, 0.0, 0.0, 100.0, false));
		}
	}
}