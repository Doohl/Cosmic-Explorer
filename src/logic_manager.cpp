#include "logic_manager.h"

Entity* Logic::newEntity(EntityType type, std::string name) {
	std::unique_ptr<Entity>& entity = Logic::entities.emplace_back(new Entity(type, name));
	entity->setID(++Logic::idIncrement);
	return entity.get();
}

Entity* Logic::newEntity(EntityType type) {
	std::unique_ptr<Entity>& entity = Logic::entities.emplace_back(new Entity(type));
	entity->setID(++Logic::idIncrement);
	return entity.get();
}

void Logic::clockForward(universeTime increment) {
	Logic::universeClock += increment;
}

void Logic::clockSet(universeTime newTime) {
	Logic::clockForward(newTime - Logic::universeClock);
}