#include "entity.h"

#include <iostream>
#include "doctest.h"


TEST_SUITE("Entity") {

	TEST_CASE("Entities can be properly constructed") {
		Entity entity;
		CHECK(entity.getPosition() == nullptr);
		CHECK(entity.getPhysicalProperties() == nullptr);
		CHECK(entity.getID() == 0);
		CHECK(entity.getName() == "unknown");
		CHECK(entity.getType() == EntityType::unknown);
		CHECK(entity.getParentEntity() == nullptr);
		CHECK(entity.getOrbitalProperties() == nullptr);

		Entity secondEntity(EntityType::asteroid, "Asteroid");
		CHECK(secondEntity.getType() == EntityType::asteroid);
		CHECK(secondEntity.getName() == "Asteroid");
	}

	TEST_CASE("Entities can have parent/children set") {
		Entity parent;
		Entity child;

		parent.addChildEntity(&child);
		CHECK(parent.getChildEntities().size() == 1);
		CHECK(parent.getChildEntities()[0] == &child);
		CHECK(child.getParentEntity() == &parent);
	}

	TEST_CASE("An Entity network can be properly instantiated") {
		Entity parent(EntityType::unknown, "Parent");
		Entity child(EntityType::unknown, "Child");
		parent.addChildEntity(&child);

		std::vector<Entity*> network = parent.getEntityNetwork();
		CHECK(network.size() == 2);

		SUBCASE("A more complicated network can be instantiated") {
			Entity grandChild(EntityType::unknown, "GrandChild");
			Entity grandFather(EntityType::unknown, "GrandFather");
			Entity sibling(EntityType::unknown, "Sibling");

			grandFather.addChildEntity(&parent);
			parent.addChildEntity(&sibling);
			child.addChildEntity(&grandChild);
			// network = grandFather.getEntityNetwork();
			// for(auto iter = network.begin(); iter != network.end(); iter++) {
			// 	std::cout << (*iter)->getName() << std::endl;
			// }
			CHECK(grandFather.getEntityNetwork().size() == 5);
		}
	}

}