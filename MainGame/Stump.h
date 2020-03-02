#pragma once

#include "TerrainObject.h"

class Stump : public TerrainObject
{
public:
	Stump(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2f calculateTextureOffset() override;
	void initPedestal() override;
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
	int strength;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
};
