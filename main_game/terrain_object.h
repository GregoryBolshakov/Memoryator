#pragma once
#ifndef TERRAINOBJECT_H
#define TERRAINOBJECT_H

#include "static_object.h"

class terrain_object : public static_object
{
protected:
	Vector2f focus1, focus2;
	Vector2f dot1, dot2, currentDot = Vector2f (-1, -1);
public:
	terrain_object(std::string objectName, Vector2f centerPosition);
	virtual ~terrain_object();
	void init_micro_blocks() override;
	[[nodiscard]] Vector2f getFocus1() const { return focus1; }
	[[nodiscard]] Vector2f getFocus2() const { return focus2; }
	void setFocuses(std::vector<Vector2f> focuses);
	[[nodiscard]] Vector2f getDot1() const { return dot1; }
	[[nodiscard]] Vector2f getDot2() const { return dot2; }
	[[nodiscard]] Vector2f getCurrentDot() const { return currentDot; }
	std::vector<float> ellipseSizeMultipliers = { 1.4f };
	float getEllipseSize(int i = 0);
	bool isIntersected(Vector2f curPosition, Vector2f newPosition); //const;
	[[nodiscard]] std::vector<int> getMultiellipseIntersect(Vector2f position) const;
	[[nodiscard]] Vector2f newSlippingPositionForDotsAdjusted(Vector2f motionVector, float speed, long long elapsedTime) const;	

	std::vector<std::pair<Vector2f, Vector2f>> internalEllipses; // focus1 - focus2
};

#endif