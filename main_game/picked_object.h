#pragma once

#include "terrain_object.h"

class hero_bag;
struct cell;

class picked_object : public terrain_object
{
protected:
	entity_tag id;
	int count;
public:
	picked_object::picked_object(std::string objectName, sf::Vector2f centerPosition);
	virtual ~picked_object();
	bool pickUp(std::vector<hero_bag>& bags);
	entity_tag getId() { return id; }
	int getCount() { return count; }

	std::vector<cell> ownInventory;
};
