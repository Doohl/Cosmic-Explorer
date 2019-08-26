#pragma once

#include <string>
#include <memory>
#include <vector>

#include "typedefs.h"
#include "entity.h"
#include "json_fwd.hpp"

// Get universe time relative to the J2000 epoch
universeTime getTimeSinceJ2000();


class LogicManager {
public:
	LogicManager()
		: idIncrement(0),
		viewingSystem("Sol"),
		universeClock(0)
	{}

	LogicManager(universeTime startingTime)
		: idIncrement(0),
		viewingSystem("Sol"),
		universeClock(startingTime)
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
	// Get the current time_t
	time_t getUniverseTime() const {
		return static_cast<time_t>(universeClock + 946684800);
	}

	// Helper functions to iterate through the vector of entities
	std::vector<std::unique_ptr<Entity>>::iterator getEntitiesBegin();
	std::vector<std::unique_ptr<Entity>>::iterator getEntitiesEnd();

	// Advance time by a given increment (in seconds since the J2000 Epoch)
	void clockForward(universeTime timeIncrement);

	// Set the universe clock to a new point in time (in seconds since the J2000 Epoch)
	void clockSet(universeTime newTime);

	// Load up the SOL json from file, feed it to initializeSystem()
	void initializeSol();
	
	// Initialize an arbitrary system (read it from JSON)
	void initializeSystem(nlohmann::json system);
	
	// If true, the universe is advancing in realtime
	bool universeAdvancing = true;

	// The timescale factor for realtime advancement
	int timeScale = 1;

private:
	// When an entity is created, use this number as its ID then increment it +1
	entityID idIncrement = 0;

	// The name of the solar system client is viewing
	std::string viewingSystem = "Sol";

	// The current universe clock (seconds since the J2000 Epoch)
	universeTime universeClock = 0;

	// A vector of all entities in the game
	std::vector<std::unique_ptr<Entity>> entities;
};