#pragma once

#include <cmath>

#include "typedefs.h"
#include "vec2.h"
#include "utilities.h"
#include "json_fwd.hpp"

// Perform a Newton Approximation for Eccentric Anomaly
double eccentricAnomalyApproximation(int maxComputations, double eccentricAnomaly, 
	double meanAnomaly, double eccentricity, double error);

class KeplerOrbit {
public:
	// Create a 2D Kepler Orbit using 6 initial orbital elements
	KeplerOrbit(double _semimajorAxis, double _eccentricity, universeTime _epoch,
		double _meanAnomaly, double _lAscending, double _aPeriapsis, double _standardGravTotal,
		bool _clockwise);

	// Create a 2D Kepler Orbit from a JSON object
	KeplerOrbit(const nlohmann::json& object, double _standardGravTotal);

	void initializeElements();

	static universeTime getEpochTime(const std::string& epoch);
	static universeTime getEpochTime(const nlohmann::json& object);

	double getSemimajorAxis() const {
		return semimajorAxis;
	}
	double getSemiminorAxis() const {
		return semiminorAxis;
	}
	double getEccentricity() const {
		return eccentricity;
	}
	universeTime getepoch() const {
		return epoch;
	}
	double getmeanAnomaly() const {
		return meanAnomaly;
	}
	double getLPeriapsis() const {
		return lPeriapsis;
	}
	double getLAscending() const {
		return lAscending;
	}
	double getAPeriapsis() const {
		return aPeriapsis;
	}
	bool getClockwise() const {
		return clockwise;
	}
	double getPeriapsis() const {
		return periapsis;
	}
	double getApoapsis() const {
		return apoapsis;
	}
	double getStandardGravTotal() const {
		return standardGravTotal;
	}
	double getMeanAngularMotion() const {
		return meanAngularMotion;
	}
	universeTime getPeriod() const {
		return period;
	}

	// Get the center point of the ellipse representing this orbit
	Vec2 getCenter(const Vec2& focus) const;

	// Compute the Eccentric Anomaly of the orbit at an input mean anomaly
	double computeEccentricAnomaly(double meanAnomaly) const;

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
	universeTime epoch;
	// Mean anomaly at epoch (radians)
	double meanAnomaly;
	// Longitude of the periapsis (radians)
	double lPeriapsis;
	// Longitude of the ascending node (radians)
	double lAscending;
	// Argument of the periapsis (radians)
	double aPeriapsis;

	// The closest distance to the parent (km)
	double periapsis;
	// The furthest distance from the focus
	double apoapsis;

	// The sum of the standard gravitional parameters
	double standardGravTotal;

	// Mean angular motion
	double meanAngularMotion;
	// Orbital period: time for one revolution, in years
	universeTime period;

	// True if the orbit is travelling clockwise relative to the viewport
	bool clockwise;
};