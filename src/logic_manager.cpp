#include "logic_manager.h"

#include <algorithm>
#include <fstream>
#include <ctime>

using json = nlohmann::json;

universeTime getTimeSinceJ2000() {
	time_t timestamp = std::time(nullptr);
	timestamp -= 946684800;
	return static_cast<universeTime>(timestamp);
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
			switch(type) {
				case EntityType::star:
					minRadius = 6;
					break;
			}
			if(object.contains("children")) {
				minRadius = 3;
			}

			PhysicalProperties* physicalProps = celestial->setPhysicalProperties(object["radius"], minRadius, object["mass"]);

			if(object.contains("orbit") && parent != nullptr) {
				json orbit = object["orbit"];
				double standardGravTotal = physicalProps->standardGrav + parent->getPhysicalProperties()->standardGrav;
				celestial->setOrbitalProperties(orbit["semimajorAxis"], orbit["eccentricity"], orbit["epochTime"],
					orbit["epochAnomaly"], orbit["lAscending"], orbit["aPeriapsis"],
					standardGravTotal, orbit.contains("retrograde") ? false : true);
				
				// Set the Celestial's starting position with the current universeClock
				celestial->setPosition(celestial->getOrbitalProperties()->computeCoordinates(*(parent->position), universeClock));
			} else {
				celestial->setPosition(0, 0);
			}
		}
		if(object.contains("children")) {
			json children = object["children"];
			for(auto iter = children.begin(); iter != children.end(); iter++) {
				Entity* childEntity = loadEntity(*iter, celestial);
				celestial->addChildEntity(childEntity);
			}
		}

		return celestial;
	};

	loadEntity(system, nullptr);
}