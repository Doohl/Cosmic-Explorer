#include "kepler_orbit.h"

#include <cmath>
#include <sstream>

#include "entity.h"
#include "utilities.h"
#include "json.hpp"

using json = nlohmann::json;

KeplerOrbit::KeplerOrbit(double _semimajorAxis, double _eccentricity, universeTime _epoch,
	double _meanAnomaly, double _lAscending, double _aPeriapsis, double _standardGravTotal,
	bool _clockwise)
	: semimajorAxis(_semimajorAxis),
	eccentricity(_eccentricity),
	epoch(_epoch),
	meanAnomaly(_meanAnomaly),
	lAscending(_lAscending),
	aPeriapsis(_aPeriapsis),
	standardGravTotal(_standardGravTotal),
	clockwise(_clockwise)
{
	if(semimajorAxis <= 0)
		throw "Invalid semimajor axis";
	if(eccentricity < 0 || eccentricity >= 1.0)
		throw "Eccentricity out of bounds";
	if(standardGravTotal <= 0)
		throw "Invalid standard grav total";

	initializeElements();
}

KeplerOrbit::KeplerOrbit(const json& object, double _standardGravTotal)
	: semimajorAxis(object.contains("semimajorAxisAU") ? object["semimajorAxisAU"].get<double>() * Util::Au2Km : object["semimajorAxis"].get<double>()),
	eccentricity(object["eccentricity"].get<double>()),
	epoch(KeplerOrbit::GetEpochTime(object)),
	meanAnomaly(KeplerOrbit::GetAnomaly(object)),
	lAscending(object.contains("lAscendingDeg") ? object["lAscendingDeg"].get<double>() * Util::Deg2Rad : object["lAscending"].get<double>()),
	aPeriapsis(object.contains("aPeriapsisDeg") ? object["aPeriapsisDeg"].get<double>() * Util::Deg2Rad : object["aPeriapsis"].get<double>()),
	standardGravTotal(_standardGravTotal),
	clockwise(object.contains("retrograde") ? false : true)
{

	if(semimajorAxis <= 0)
		throw "Invalid semimajor axis";
	if(eccentricity < 0 || eccentricity >= 1.0)
		throw "Eccentricity out of bounds";
	if(standardGravTotal <= 0)
		throw "Invalid standard grav total";

	initializeElements();
}

void KeplerOrbit::initializeElements() {
	periapsis = semimajorAxis * (1 - eccentricity);
	apoapsis = semimajorAxis * (1 + eccentricity);
	semiminorAxis = std::sqrt(periapsis * apoapsis);
	lPeriapsis = lAscending + aPeriapsis;
	meanAngularMotion = std::sqrt(standardGravTotal / std::pow(semimajorAxis * 1000.0, 3.0));
	if(clockwise)
		meanAngularMotion *= -1;
	period = std::sqrt(std::pow(semimajorAxis / Util::Au2Km, 3)); // convert semimajorAxis from km to AU
}

universeTime KeplerOrbit::GetEpochTime(const std::string& epoch) {
	if(epoch == "J2000")
		return 0.0;
	return 0.0;
}

universeTime KeplerOrbit::GetEpochTime(const json& object) {
	if(object.contains("epoch")) {
		if(object["epoch"].is_number())
			return object["epoch"];
		if(object["epoch"].is_string())
			return KeplerOrbit::GetEpochTime(object["epoch"]);
	}
	return 0.0;
}

double KeplerOrbit::GetAnomaly(const json& object) {
	if(object.contains("meanAnomalyDeg")) {
		if(object["meanAnomalyDeg"].is_number())
			return object["meanAnomalyDeg"].get<double>() * Util::Deg2Rad;
		// Assume the meanAnomalyDeg is a string, which takes the form of "degrees arcminutes arcseconds"
		double rads = 0.0;

		std::stringstream stream(object["meanAnomalyDeg"].get<std::string>());
		double num;
		if(stream >> num) {
			rads = num * Util::Deg2Rad;
		} else {
			throw "meanAnomalyDeg cannot be defined and empty!";
		}
		if(stream >> num)
			rads += num * Util::ArcMin2Rad;
		if(stream >> num)
			rads += num * Util::ArcSec2Rad;

		return rads;
	}
	return object["meanAnomaly"].get<double>();
}

double eccentricAnomalyApproximation(int maxComputations, double eccentricAnomaly, 
	double meanAnomaly, double eccentricity, double error) {

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

double KeplerOrbit::computeEccentricAnomaly(double currentMeanAnomaly) const {
	// Orbits of e > 0.8 -> initial value of pi used
	if(eccentricity > 0.9) {
		return eccentricAnomalyApproximation(1000, Util::PI, currentMeanAnomaly, eccentricity, 10e-15);
	} else if(eccentricity > 0.8) {
		return eccentricAnomalyApproximation(500, Util::PI, currentMeanAnomaly, eccentricity, 10e-15);
	} else {
		return eccentricAnomalyApproximation(150, currentMeanAnomaly, currentMeanAnomaly, eccentricity, 10e-15);
	}
}

double KeplerOrbit::computeMeanAnomaly(const universeTime& time) const {
	return (meanAngularMotion * time) - meanAnomaly;
}

double KeplerOrbit::computeTrueAnomaly(const universeTime& time) const {
	// Compute the current mean anomaly (offset by the epoch mean anomaly)
	double currentMeanAnomaly = computeMeanAnomaly(time);
	double eccentricAnomaly = computeEccentricAnomaly(currentMeanAnomaly);
	return 2 * std::atan2(
		std::sqrt(1 + eccentricity) * std::sin(eccentricAnomaly / 2),
		std::sqrt(1 - eccentricity) * std::cos(eccentricAnomaly / 2)
	);
}

Vec2 KeplerOrbit::computeCoordinates(const Vec2& focus, const universeTime& time) const {
	double timeSinceEpoch = time - epoch;
	double trueAnomaly = computeTrueAnomaly(timeSinceEpoch);

	// The distance from the focus in km
	double distance = (semimajorAxis * 1000) * (1 - eccentricity*eccentricity) / (1 + eccentricity * std::cos(trueAnomaly)) / 1000; // convert from m to km at the end
	
	double x = focus.x + (distance * std::cos(trueAnomaly));
	double y = focus.y + (distance * std::sin(trueAnomaly));

	return rotatePoint(Vec2(x, y), focus, -lPeriapsis);
}
