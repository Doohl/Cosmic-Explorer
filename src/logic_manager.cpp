#include "logic_manager.h"

Entity* LogicManager::newEntity(EntityType type, std::string name) {
	std::unique_ptr<Entity>& entity = entities.emplace_back(new Entity(type, name));
	entity->setID(++idIncrement);
	return entity.get();
}

Entity* LogicManager::newEntity(EntityType type) {
	std::unique_ptr<Entity>& entity = entities.emplace_back(new Entity(type));
	entity->setID(++idIncrement);
	return entity.get();
}

void LogicManager::clockForward(universeTime increment) {
	universeClock += increment;
}

void LogicManager::clockSet(universeTime newTime) {
	clockForward(newTime - universeClock);
}