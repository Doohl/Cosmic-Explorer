#include "logic_manager.h"

std::shared_ptr<Entity>& Logic::newEntity(EntityType type, std::string name) {
	std::shared_ptr<Entity>& entity = Logic::entities.emplace_back(new Entity(type, name));
	return entity;
}

std::shared_ptr<Entity>& Logic::newEntity(EntityType type) {
	std::shared_ptr<Entity>& entity = Logic::entities.emplace_back(new Entity(type));
	return entity;
}

void Logic::clockForward(universeTime increment) {
	Logic::universeClock += increment;
}

void Logic::clockSet(universeTime newTime) {
	Logic::clockForward(newTime - Logic::universeClock);
}