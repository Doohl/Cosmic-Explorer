#pragma once

#include <cmath>

#include "typedefs.h"
#include "vec2.h"

// Perform a Newton Approximation for Eccentric Anomaly
double eccentricAnomalyApproximation(const int& maxComputations, double eccentricAnomaly, 
	const double& meanAnomaly, const double& eccentricity, const double& error);

class KeplerOrbit {
public:
	// Create a 2D Kepler Orbit using 6 initial orbital elements
	KeplerOrbit(double _semimajorAxis, double _eccentricity, universeTime _epochTime,
		double _epochAnomaly, double _lAscending, double _aPeriapsis, double _standardGravTotal,
		bool _clockwise)
		: semimajorAxis(_semimajorAxis),
		eccentricity(_eccentricity),
		epochTime(_epochTime),
		epochAnomaly(_epochAnomaly),
		lAscending(_lAscending),
		aPeriapsis(_aPeriapsis),
		clockwise(_clockwise),
		standardGravTotal(_standardGravTotal)
	{
		periapsis = semimajorAxis * (1 - eccentricity);
		apoapsis = semimajorAxis * (1 + eccentricity);
		semiminorAxis = std::sqrt(periapsis * apoapsis);
		lPeriapsis = lAscending + aPeriapsis;
		meanAngularMotion = std::sqrt(standardGravTotal / std::pow(semimajorAxis * 1000.0, 3.0));
		if(clockwise)
			meanAngularMotion *= -1;
		period = std::sqrt(std::pow(semimajorAxis / 1.496e+9, 3)); // convert semimajorAxis from km to AU
	}

	// Get the center point of the ellipse representing this orbit
	Vec2 getCenter(const Vec2& focus) const;

	// Compute the Eccentric Anomaly of the orbit at an input mean anomaly
	double computeEccentricAnomaly(const double& meanAnomaly) const;

	// Compute the Mean Anomaly at a specified time
	double computeMeanAnomaly(const universeTime& time) const;

	// Compute the True Anomaly at a specified time
	double computeTrueAnomaly(const universeTime& time) const;

	// Compute the current coordinates at a specified time relative to an orbit focus
	Vec2 computeCoordinates(const Vec2& focus, const universeTime& time) const;

private:
	/** More or less immutable: */

	// Semimajor axis (km)
	double semimajorAxis;
	// Semiminor axis (km)
	double semiminorAxis;
	// Eccentricity
	double eccentricity;
	// Starting epoch (seconds since J2000)
	universeTime epochTime;
	// Mean anomaly (radians)
	double epochAnomaly;
	// Longitude of the periapsis (radians)
	double lPeriapsis;
	// Longitude of the ascending node
	double lAscending;
	// Argument of the periapsis (radians)
	double aPeriapsis;
	// Apsidal precession in rad
	double aPrecession;
	// True if the orbit is travelling clockwise relative to the viewport
	bool clockwise;

	// The closest distance to the parent (km)
	double periapsis;
	// The furthest distance from the focus
	double apoapsis;

	// The sum of the standard gravitional parameters
	double standardGravTotal;

	// Mean angular motion
	double meanAngularMotion;
	// Orbital period: time for one revolution, in years
	double period;
};