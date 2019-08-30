#include "logic_manager.h"

#include <algorithm>
#include <fstream>
#include <ctime>

#include "json.hpp"

using json = nlohmann::json;

universeTime LogicManager::GetTimeSinceJ2000() {
	time_t timestamp = std::time(nullptr);
	return static_cast<universeTime>(timestamp - Util::J2000Unix);
}

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

std::vector<std::unique_ptr<Entity>>::iterator LogicManager::getEntitiesBegin() {
	return entities.begin();
}
std::vector<std::unique_ptr<Entity>>::iterator LogicManager::getEntitiesEnd() {
	return entities.end();
}

void LogicManager::forEntities(const std::function<void(Entity*)>& fn) {
	for(auto& uPtr : entities) {
		fn(uPtr.get());
	}
}
void LogicManager::forEntities(const std::function<void(Entity*)>& fn, 
	std::vector<std::unique_ptr<Entity>>::iterator& begin,
	std::vector<std::unique_ptr<Entity>>::iterator& end
	) 
{
	for(; begin != end; begin++) {
		fn(begin->get());
	}
}

void LogicManager::clockForward(universeTime increment) {
	universeClock += increment;
	for(auto& uPtr : entities) {
		Entity* entity = uPtr.get();
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

void LogicManager::initializeSol() {
	std::ifstream i("sol.json");
	json sol;
	i >> sol;
	
	initializeSystem(sol);
}

void LogicManager::initializeSystem(json system) {

	// Process a single stage in the JSON object tree, then propagate down the tree
	std::function<Entity*(const json& object, const Entity* parent)> loadEntity = [this, &loadEntity](const json& object, const Entity* parent) -> Entity* {
		EntityType type;
		if(object.contains("stellarClass")) {
			type = EntityType::star;
		} else {
			type = EntityType::planet; // TODO: proper automated celestial classification
		}
		Entity* celestial = newEntity(type, object["name"]);

		if(object.contains("mass")) {
			double minRadius = 2;
			if(type == EntityType::star) {
				minRadius = 6;
			} else if(parent->getType() == EntityType::star) {
				minRadius = 3;
			}

			PhysicalProperties* physicalProps = celestial->setPhysicalProperties(object["radius"], minRadius, object["mass"]);

			if(object.contains("orbit") && parent != nullptr) {
				json orbit = object["orbit"];
				double standardGravTotal = physicalProps->standardGrav + parent->getPhysicalProperties()->standardGrav;
				celestial->loadOrbitalProperties(orbit, standardGravTotal);
				// Set the Celestial's starting position with the current universeClock
				celestial->setPosition(celestial->getOrbitalProperties()->computeCoordinates(*(parent->position), universeClock));
			} else {
				celestial->setPosition(0, 0);
			}
		}
		if(object.contains("children")) {
			json children = object["children"];
			for(const json& child : children) {
				Entity* childEntity = loadEntity(child, celestial);
				celestial->addChildEntity(childEntity);
			}
		}

		return celestial;
	};

	loadEntity(system, nullptr);
}