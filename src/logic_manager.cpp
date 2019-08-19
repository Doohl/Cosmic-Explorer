#include "logic_manager.h"

#include <algorithm>

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

bool LogicManager::deleteEntity(const Entity* entity) {
	auto position = std::find_if(entities.begin(), entities.end(), [&entity](const auto& iterEntity) {
		return iterEntity.get() == entity;
	});
	// If the entity was found, erase it from the vector; deleting the unique_ptr
	if(position != entities.end()) {
		entities.erase(position);
		return true;
	}
	return false;
}
bool LogicManager::deleteEntity(const std::string& name) {
	auto position = std::find_if(entities.begin(), entities.end(), [&name](const auto& iterEntity) {
		return iterEntity->getName() == name;
	});
	// If the entity was found, erase it from the vector; deleting the unique_ptr
	if(position != entities.end()) {
		entities.erase(position);
		return true;
	}
	return false;
}
bool LogicManager::deleteEntity(const entityID& id) {
	auto position = std::find_if(entities.begin(), entities.end(), [&id](const auto& iterEntity) {
		return iterEntity->getID() == id;
	});
	// If the entity was found, erase it from the vector; deleting the unique_ptr
	if(position != entities.end()) {
		entities.erase(position);
		return true;
	}
	return false;
}

size_t LogicManager::getEntityCount() const {
	return entities.size();
}

void LogicManager::clockForward(universeTime increment) {
	universeClock += increment;

	for(auto iter = entities.begin(); iter != entities.end(); iter++) {
		Entity* entity = iter->get();
		KeplerOrbit* orbit = entity->getOrbitalProperties();
		Vec2* currentPos = entity->getPosition();
		Entity* parent = entity->getParentEntity();

		// If the orbit is a valid celestial orbiting something, compute its next position
		if(orbit != nullptr && parent != nullptr && currentPos != nullptr)
			currentPos->set(orbit->computeCoordinates(*(parent->position), universeClock));
	}
}

void LogicManager::clockSet(universeTime newTime) {
	clockForward(newTime - universeClock);
}