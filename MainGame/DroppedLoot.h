#pragma once

#include "PickedObject.h"

class DroppedLoot : public PickedObject
{
public:
	DroppedLoot(std::string objectName, Vector2f centerPosition, int typeOfObject, int count = 1);
	Vector2f calculateTextureOffset() override;
	void initPedestal() override;
	std::vector<SpriteChainElement*> prepareSprites(long long elapsedTime) override;
	int strength{};
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;	
};
