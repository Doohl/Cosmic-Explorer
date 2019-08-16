#include "entity.h"

void Entity::setID(entityID newID) {
	if(id == 0) id = newID;
}

Vec2* Entity::setPosition(double x, double y) {
	position = std::make_unique<Vec2>(x, y);
	return position.get();
}
Vec2* Entity::setPosition() {
	position = std::make_unique<Vec2>();
	return position.get();
}

KeplerOrbit* Entity::setOrbitalProperties(double _semimajorAxis, double _eccentricity, universeTime _epochTime,
	double _epochAnomaly, double _lAscending, double _aPeriapsis, double _standardGravTotal,
	bool _clockwise) {
	
	orbitalProperties = std::make_unique<KeplerOrbit>(_semimajorAxis, _eccentricity, _epochTime, _epochAnomaly, 
		_lAscending, _aPeriapsis, _standardGravTotal, _clockwise);
	
	return orbitalProperties.get();
}

const Entity* Entity::setParentEntity(const Entity* parent) {
	parentEntity = parent;
	return parent;
}