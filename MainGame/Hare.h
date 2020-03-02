#pragma once

#include "Helper.h"
#include "neutral_mob.h"

using namespace sf;

class hare : public neutral_mob
{
public:
	hare(const std::string& objectName, Vector2f centerPosition);
	~hare();
	Vector2f calculateTextureOffset() override;
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
	void setTarget(dynamic_object& object) override;
	void behaviorWithStatic(world_object* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void endingPreviousAction();
	Vector2f getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;

private:
	long long timeForNewSprite;
	int animationLength{};
	long long timeAfterFear = 0;
	long long fearTime = long(2 * 10e5);
};
