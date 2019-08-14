#pragma once

namespace Util {
	// Approximate PI constant
	constexpr double PI = 3.14159265358979323846;
	// Approximate gravitational constant in [m^3 kg^-1 s^-2]
	constexpr double GRAV = 6.67408e-11;
	// Conversion factor from AU to km
	constexpr double AU = 149598023;
	// The number of seconds in an earth day
	constexpr double EARTH_YEAR = 3.154e+7;
	// Conversion factor from degrees to radians
	constexpr double DEG = 0.0174533;
	// Number of milliseconds between the Unix epoch and the J2000 epoch
	constexpr double J2000_UNIX = 946728000000;

	// Max allowed ellipse semimajor axis, in pixels
	constexpr double MAX_ELLIPSE_SEMIMAJOR = 2000;
	// Max allowed distance from a system's center (~150 AU)
	constexpr double SYSTEM_EDGE = 2.244e+10;
}