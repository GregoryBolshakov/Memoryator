#pragma once

#include "TerrainObject.h"
#include "hero_bag.h"

class PickedObject : public TerrainObject
{
protected:
	Tag id;
	int count;
public:
	PickedObject::PickedObject(std::string objectName, Vector2f centerPosition);
	virtual ~PickedObject();
	bool pickUp(std::vector<hero_bag> *bags);
	Tag getId() { return id; }
	int getCount() { return count; }

	std::vector<cell> ownInventory;
};
