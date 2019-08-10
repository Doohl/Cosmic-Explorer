#include "entity.h"

void Point2D::set(double setX, double setY) {
	x = setX;
	y = setY;
}

void Entity::setID(entityID newID) {
	if(id == 0) id = newID;
}