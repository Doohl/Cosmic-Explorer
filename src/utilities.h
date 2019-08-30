#pragma once

namespace Util {
	// Approximate PI constant
	constexpr double PI = 3.14159265358979323846;
	constexpr float PIf = 3.14159265358979f;

	// Approximate gravitational constant in [m^3 kg^-1 s^-2]
	constexpr double Grav = 6.67408e-11;
	// Conversion factor from AU to km
	constexpr double Au2Km = 149598023;
	// The number of seconds in an earth year
	constexpr double EarthYear = 3.154e+7;
	// Conversion factor from degrees to radians
	constexpr double Deg2Rad = 0.0174533;
	// Conversion factor from arc minutes to radians
	constexpr double ArcMin2Rad = 0.000290888;
	// Conversion factor from arc seconds to radians
	constexpr double ArcSec2Rad = 4.84814e-6;

	// The unix time of the J2000 epoch
	constexpr double J2000Unix = 946684800;

	// Max allowed ellipse semimajor axis, in pixels
	constexpr double MaxEllipseSemimajor = 7000;
	// Max allowed distance from a system's center (~150 AU)
	constexpr double SystemEdge = 2.244e+10;
}