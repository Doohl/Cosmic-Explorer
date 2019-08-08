#pragma once

#include <string>
#include <memory>
#include <vector>

// An unsigned 32-bit integer given to every entity on creation
typedef unsigned int entityID;

// Seconds relative to the J2000 Epoch
typedef unsigned long int universeTime;

#include "entity.h"

namespace Logic {
	// When an entity is created, use this number as its ID then increment it +1
	entityID idIncrement = 0;

	// The name of the solar system client is viewing
	std::string viewingSystem = "Sol";

	// The current universe clock
	universeTime universeClock = 0;

	// A vector of all entities in the game
	std::vector<std::shared_ptr<Entity>> entities;

	// Create a new entity of a specified type and name
	std::shared_ptr<Entity>& newEntity(EntityType type, std::string name);
	// Create a new entity of a specified type
	std::shared_ptr<Entity>& newEntity(EntityType type);

	// Advance time by a given increment (in seconds since the J2000 Epoch)
	void clockForward(universeTime timeIncrement);

	// Set the universe clock to a new point in time (in seconds since the J2000 Epoch)
	void clockSet(universeTime newTime);
}