#pragma once

#include <string>
#include <memory>
#include <vector>

#include "typedefs.h"
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
class Point2D {
public:

	Point2D()
		: x(0),
		y(0)
	{}

	Point2D(long int initX, long int initY)
		: x(initX),
		y(initY)
	{}

	void set(long int setX, long int setY);

	long int x;
	long int y;
};

// Some physical properties
class PhysicalProperties {
public:

	long int radius = 0;
	long int minRadius = 0;
};


class Entity {
public:
	Entity()
		: position(),
		physicalProperties(),
		id(0),
		name("unknown"),
		type(EntityType::unknown),
		parentEntity(nullptr),
		orbitalProperties(nullptr)
	{}
	Entity(EntityType initType, std::string initName)
		: position(),
		physicalProperties(),
		id(0),
		name(initName),
		type(initType),
		parentEntity(nullptr),
		orbitalProperties(nullptr)
	{}
	Entity(EntityType initType)
		: position(),
		physicalProperties(),
		id(0),
		name("unknown"),
		type(initType),
		parentEntity(nullptr),
		orbitalProperties(nullptr)
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
	Entity* getParentEntity() const {
		return parentEntity;
	}
	std::vector<std::shared_ptr<Entity>> getChildEntities() const {
		return childEntities;
	}
	KeplerOrbit* getOrbitalProperties() const {
		return orbitalProperties.get();
	}

	// Assign a new ID to this entity (will not work if ID already set)
	void setID(entityID newID);

	// Entity's current position on 2D grid
	std::unique_ptr<Point2D> position;

	// Entity's associated physical properties (used primarily for rendering)
	std::unique_ptr<PhysicalProperties> physicalProperties;

private:
	// Unique ID assigned to this entity upon creation
	entityID id;

	// The non-unique name given to this entity
	std::string name;

	// Immutable type that is assigned to this entity on creation
	EntityType type;

	// Pointer to the parent entity where this entity is located, if any.
	// 	Example: a star is the parent of a planet
	Entity* parentEntity;

	// Vector of child entities, if any
	std::vector<std::shared_ptr<Entity>> childEntities;

	// A container for orbital properties and logic
	std::unique_ptr<KeplerOrbit> orbitalProperties;
};