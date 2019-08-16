#pragma once

#include <string>
#include <memory>
#include <vector>

#include "typedefs.h"
#include "vec2.h"
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

// Some physical properties
class PhysicalProperties {
public:
	double radius = 0;
	double minRadius = 0;
	double mass = 0;
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
	const Entity* getParentEntity() const {
		return parentEntity;
	}
	std::vector<std::shared_ptr<Entity>> getChildEntities() const {
		return childEntities;
	}
	KeplerOrbit* getOrbitalProperties() const {
		return orbitalProperties.get();
	}
	Vec2* getPosition() const {
		return position.get();
	}
	
	// Set a new (or starting) position
	Vec2* setPosition(double x, double y);
	// Set an undefined position (to be later set)
	Vec2* setPosition(); 

	// Define a set of orbital properties
	KeplerOrbit* setOrbitalProperties(double _semimajorAxis, double _eccentricity, universeTime _epochTime,
		double _epochAnomaly, double _lAscending, double _aPeriapsis, double _standardGravTotal,
		bool _clockwise);
	
	// Define a parent entity
	const Entity* setParentEntity(const Entity* parent);

	// Assign a new ID to this entity (will not work if ID already set)
	void setID(entityID newID);

	// Entity's current position on 2D grid
	std::unique_ptr<Vec2> position;

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
	const Entity* parentEntity;

	// Vector of child entities, if any
	std::vector<std::shared_ptr<Entity>> childEntities;

	// A container for orbital properties and logic
	std::unique_ptr<KeplerOrbit> orbitalProperties;
};