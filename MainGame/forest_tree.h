#pragma once

#include "terrain_object.h"

class forest_tree : public terrain_object
{
public:
	forest_tree(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2f calculateTextureOffset() override;	
	void initPedestal() override;
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	Vector2f getOwlBase() const;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;

	int strength;
};
