#include "entity.h"

void Point2D::set(long int setX, long int setY) {
	x = setX;
	y = setY;
}

void Entity::setID(entityID newID) {
	if(id == 0) id = newID;
}