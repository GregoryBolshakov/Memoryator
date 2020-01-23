#pragma once
#ifndef TERRAINOBJECT_H
#define TERRAINOBJECT_H

#include "StaticObject.h"

class TerrainObject : public StaticObject
{
protected:
	Vector2f focus1, focus2;
	Vector2f dot1, dot2, currentDot = Vector2f (-1, -1);
public:
	TerrainObject(std::string objectName, Vector2f centerPosition);
	virtual ~TerrainObject();
	void initMicroBlocks() override;
	Vector2f getFocus1() const { return focus1; }
	Vector2f getFocus2() const { return focus2; }
	void setFocuses(std::vector<Vector2f> focuses);
	Vector2f getDot1() const { return dot1; }
	Vector2f getDot2() const { return dot2; }
	Vector2f getCurrentDot() { return currentDot; }
	std::vector<float> ellipseSizeMultipliers = { 1.4 };
	int getEllipseSize(int i = 0);
	bool isIntersected(Vector2f curPosition, Vector2f newPosition); //const;
	std::vector<int> getMultiellipseIntersect(Vector2f position) const;
	Vector2f newSlippingPositionForDotsAdjusted(Vector2f position, Vector2f motionVector, float speed, float elapsedTime);	

	std::vector<std::pair<Vector2f, Vector2f>> internalEllipses; // focus1 - focus2
};

#endif