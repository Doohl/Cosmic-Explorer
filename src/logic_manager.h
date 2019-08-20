#pragma once

#include <string>
#include <memory>
#include <vector>

#include "typedefs.h"
#include "entity.h"

class LogicManager {
public:
	LogicManager()
		: idIncrement(0),
		viewingSystem("Sol"),
		universeClock(0)
	{}

	// Create a new entity of a specified type and name
	Entity* newEntity(EntityType type, std::string name);
	// Create a new entity of a specified type
	Entity* newEntity(EntityType type);

	// Delete an entity from the game. Returns true if deletion was successful
	bool deleteEntity(const Entity* entity);
	// Delete the first entity in the game matching a name. Returns true if deletion was successful
	bool deleteEntity(const std::string& name);
	// Delete an entity from the game, searching for its ID. Return true if deletion was successful
	bool deleteEntity(const entityID& id);

	// Get the number of entities in the game
	size_t getEntityCount() const;
	// Get the current universe clock
	universeTime getUniverseClock() const {
		return universeClock;
	}

	// Advance time by a given increment (in seconds since the J2000 Epoch)
	void clockForward(universeTime timeIncrement);

	// Set the universe clock to a new point in time (in seconds since the J2000 Epoch)
	void clockSet(universeTime newTime);

private:
	// When an entity is created, use this number as its ID then increment it +1
	entityID idIncrement = 0;

	// The name of the solar system client is viewing
	std::string viewingSystem = "Sol";

	// The current universe clock
	universeTime universeClock = 0;

	// A vector of all entities in the game
	std::vector<std::unique_ptr<Entity>> entities;
};