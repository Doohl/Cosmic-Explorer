#pragma once

#include <string>
#include <memory>
#include <vector>

#include "logic_manager.h"
#include "kepler_orbit.h"

// Mutually-exclusive entity types
enum EntityType {
	unknown,
	asteroid,
	comet,
	planet,
	moon,
	star,
	blackhole,
	neutron,

	vessel,
	colony
};

// Flags for entity modifier bitfield
enum EntityModifier {
	// A natural celestial body (asteroid, comet, planet, star, etc)
	celestial = 0x01,
	// A manmade body (vessel, station, drone, etc)
	unit = 0x02,
	
	gas = 0x04,
	ice = 0x08,
	desert = 0x10,
	dwarf = 0x20,
	giant = 0x40
};

// A simple (x,y) coordinate system
typedef struct {
	long int x = 0;
	long int y = 0;
} coordinate2D;

// Some physical properties
typedef struct {
	long int radius = 0;
	long int minRadius = 0;
} physicalProperties;


class Entity {
public:
	Entity()
		: name("unknown"),
		type(EntityType::unknown),
		parentEntity(nullptr),
		position(),
		physicalProperties()
	{}
	Entity(EntityType initType, std::string initName)
		: name(initName),
		type(initType),
		parentEntity(nullptr),
		position(),
		physicalProperties()
	{}
	Entity(EntityType initType)
		: name("unknown"),
		type(initType),
		parentEntity(nullptr),
		position(),
		physicalProperties()
	{}

	entityID getID() const {
		return id;
	}
	std::string getName() const {
		return name;
	}
	EntityType getType() const {
		return type;
	}
	std::shared_ptr<Entity> getParentEntity() const {
		return parentEntity;
	}
	std::vector<std::shared_ptr<Entity>> getChildEntities() const {
		return childEntities;
	}
	std::shared_ptr<KeplerOrbit> getOrbitalProperties() const {
		return orbitalProperties;
	}

	// Entity's current position on 2D grid
	coordinate2D position;

	// Entity's associated physical properties (used primarily for rendering)
	physicalProperties physicalProperties;

private:
	// Unique ID assigned to this entity upon creation
	entityID id;

	// The non-unique name given to this entity
	std::string name;

	// Immutable type that is assigned to this entity on creation
	EntityType type;

	// Pointer to the parent entity where this entity is located, if any.
	// 	Example: a star is the parent of a planet
	std::shared_ptr<Entity> parentEntity;

	// Vector of child entities, if any
	std::vector<std::shared_ptr<Entity>> childEntities;

	// A container for orbital properties and logic
	std::shared_ptr<KeplerOrbit> orbitalProperties;
};