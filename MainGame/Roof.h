#pragma once
#ifndef ROOF_H
#define ROOF_H

#include "TerrainObject.h"

class Roof : public TerrainObject
{
protected:
public:
	Roof(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	void prepareSpriteNames(long long elapsedTime, float scaleFactor) override;
	int strength;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
};

#endif