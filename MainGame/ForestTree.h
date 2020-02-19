#pragma once
#ifndef FORESTTREE_H
#define FORESTTREE_H

#include "TerrainObject.h"

class ForestTree : public TerrainObject
{
protected:
public:
	ForestTree(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2f calculateTextureOffset() override;	
	void initPedestal() override;
	std::vector<SpriteChainElement*> prepareSprites(long long elapsedTime) override;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	Vector2f getOwlBase();
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;

	int strength;
};

#endif