#include "logic_manager.h"

#include "doctest.h"
#include <iostream>

TEST_CASE("Entities can be properly registered to LogicManager") {
	LogicManager logic;

	Entity* createdEntity = logic.newEntity(EntityType::vessel);
	CHECK(createdEntity->getID() == 1);

	logic.newEntity(EntityType::planet);
	logic.newEntity(EntityType::moon);
	logic.newEntity(EntityType::unknown);
	CHECK(logic.getEntityCount() == 4);

	createdEntity = logic.newEntity(EntityType::unknown, "something");
	CHECK(createdEntity->getName() == "something");
	CHECK(createdEntity->getID() == 5);
}

TEST_CASE("Entities can be deleted from LogicManager") {
	LogicManager logic;
	logic.newEntity(EntityType::planet, "Earth");
	logic.newEntity(EntityType::moon, "Moon");
	
	Entity* createdEntity = logic.newEntity(EntityType::unknown);
	CHECK(logic.deleteEntity(createdEntity));
	CHECK(logic.getEntityCount() == 2);

	SUBCASE("Entities can be deleted via name") {
		LogicManager subLogic;
		subLogic.newEntity(EntityType::planet, "Earth");
		CHECK(subLogic.deleteEntity("Earth"));
		CHECK(subLogic.getEntityCount() == 0);
	}
	SUBCASE("Entities can be deleted via ID") {
		LogicManager subLogic;
		createdEntity = subLogic.newEntity(EntityType::unknown);
		CHECK(subLogic.deleteEntity(createdEntity->getID()));
		CHECK(subLogic.getEntityCount() == 0);
	}
	SUBCASE("New entities can be registered after entities were deleted") {
		createdEntity = logic.newEntity(EntityType::unknown);
		CHECK(logic.getEntityCount() == 3);
		CHECK(createdEntity->getID() == 4);
	}
}

TEST_CASE("A mock two-body system can be stored and simulated using LogicManager's clock functions") {
	LogicManager logic;
	Entity* centralThing = logic.newEntity(EntityType::star);
	centralThing->setPosition(0.0, 0.0);
	
	Entity* orbiter = logic.newEntity(EntityType::planet);
	orbiter->setPosition(0.0, 0.0);
	orbiter->setOrbitalProperties(50, 0.5, 0, 0.2, 0.2, 0.5, 50, true);
	orbiter->setParentEntity(centralThing);

	logic.clockForward(100);

	// Central thing should not move
	CHECK(centralThing->position->x == 0.0);
	CHECK(centralThing->position->y == 0.0);
	
	// Orbiter should move - we don't really care how
	CHECK(orbiter->position->x != 0.0);
	CHECK(orbiter->position->y != 0.0);
}