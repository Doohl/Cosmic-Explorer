#include "kepler_orbit.h"

#include <cmath>

#include "entity.h"

#define M_PI		3.14159265358979323846

double eccentricAnomalyApproximation(const int& maxComputations, double eccentricAnomaly, 
	const double& meanAnomaly, const double& eccentricity, const double& error) {

	for(int i = 0; i < maxComputations; i++) {
		double previous = eccentricAnomaly;
		eccentricAnomaly = eccentricAnomaly - (eccentricAnomaly - eccentricity * std::sin(eccentricAnomaly) - meanAnomaly) / (1.0 - eccentricity * std::cos(eccentricAnomaly));
		if(std::abs(eccentricAnomaly - previous) < error) {
			break;
		}
	}
	return eccentricAnomaly;
}

Vec2 KeplerOrbit::getCenter(const Vec2& focus) const {
	double x = focus.x - (eccentricity * semimajorAxis);
	double y = focus.y;
	return rotatePoint(Vec2(x, y), focus, -lPeriapsis);
}

double KeplerOrbit::computeEccentricAnomaly(const double& meanAnomaly) const {
	// Orbits of e > 0.8 -> initial value of pi used
	if(eccentricity > 0.9) {
		return eccentricAnomalyApproximation(1000, M_PI, meanAnomaly, eccentricity, 10e-15);
	} else if(eccentricity > 0.8) {
		return eccentricAnomalyApproximation(500, M_PI, meanAnomaly, eccentricity, 10e-15);
	} else {
		return eccentricAnomalyApproximation(150, meanAnomaly, meanAnomaly, eccentricity, 10e-15);
	}
}

double KeplerOrbit::computeMeanAnomaly(const universeTime& time) const {
	return (meanAngularMotion * time) - epochAnomaly;
}

double KeplerOrbit::computeTrueAnomaly(const universeTime& time) const {
	double meanAnomaly = computeMeanAnomaly(time);
	double eccentricAnomaly = computeEccentricAnomaly(meanAnomaly);
	return 2 * std::atan2(
		std::sqrt(1 + eccentricity) * std::sin(eccentricAnomaly / 2),
		std::sqrt(1 - eccentricity) * std::cos(eccentricAnomaly / 2)
	);
}

Vec2 KeplerOrbit::computeCoordinates(const Vec2& focus, const universeTime& time) const {
	double timeSinceEpoch = time - epochTime;
	double trueAnomaly = computeTrueAnomaly(timeSinceEpoch);

	// The distance from the focus in km
	double distance = (semimajorAxis * 1000) * (1 - std::pow(eccentricity, 2)) / (1 + eccentricity * std::cos(trueAnomaly)) / 1000; // convert from m to km at the end
	
	double x = focus.x + (distance * std::cos(trueAnomaly));
	double y = focus.y + (distance * std::sin(trueAnomaly));

	return rotatePoint(Vec2(x, y), focus, -lPeriapsis);
}
