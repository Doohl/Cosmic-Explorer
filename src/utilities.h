#pragma once

namespace Util {
	// Approximate PI constant
	constexpr double pi = 3.14159265358979323846;
	constexpr float pif = 3.14159265358979f;

	// Approximate gravitational constant in [m^3 kg^-1 s^-2]
	constexpr double grav = 6.67408e-11;
	// Conversion factor from AU to km
	constexpr double au = 149598023;
	// The number of seconds in an earth day
	constexpr double earthYear = 3.154e+7;
	// Conversion factor from degrees to radians
	constexpr double deg2rad = 0.0174533;
	// Number of seconds between the Unix epoch and the J2000 epoch
	constexpr double j2000Unix = 946728000;

	// Max allowed ellipse semimajor axis, in pixels
	constexpr double maxEllipseSemimajor = 2000;
	// Max allowed distance from a system's center (~150 AU)
	constexpr double systemEdge = 2.244e+10;
}