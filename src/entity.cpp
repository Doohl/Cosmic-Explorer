#include "entity.h"

unsigned int Entity::getColor() const {
	if(type == EntityType::planet) {
		return 0x98a7d6ff; // have to add 'FF' for the alpha mask at the end!!!
	} else if(type == EntityType::star) {
		return 0xfff1e5ff;
	}
	return 0x00;
}

void Entity::setID(entityID newID) {
	if(id == 0) id = newID;
}

Vec2* Entity::setPosition(double x, double y) {
	position = std::make_unique<Vec2>(x, y);
	return position.get();
}
Vec2* Entity::setPosition(Vec2 vec) {
	position = std::make_unique<Vec2>(vec);
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

PhysicalProperties* Entity::setPhysicalProperties(double _radius, double _minRadius, double _mass) {
	physicalProperties = std::make_unique<PhysicalProperties>(_radius, _minRadius, _mass);
	return physicalProperties.get();
}

Entity* Entity::setParentEntity(Entity* parent) {
	parentEntity = parent;
	return parent;
}
Entity* Entity::addChildEntity(Entity* child) {
	childEntities.push_back(child);
	child->setParentEntity(this);
	return child;
}

std::vector<Entity*> Entity::getEntityNetwork() {
	std::vector<Entity*> network = {this};
	std::vector<Entity*> toExplore = getChildEntities();

	while(!toExplore.empty()) {
		std::vector<Entity*> tempExplore = toExplore;
		toExplore.clear();
		for(auto iter = tempExplore.begin(); iter != tempExplore.end(); iter++) {
			Entity* entity = *iter;
			network.push_back(entity);

			std::vector<Entity*> children = entity->getChildEntities();
			if(!children.empty()) {
				toExplore.insert(toExplore.end(), children.begin(), children.end());
			}
		}
	}
	return network;
}