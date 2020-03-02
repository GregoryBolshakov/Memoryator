#pragma once

#include "neutral_mob.h"
#include "helper.h"

using namespace sf;

class wolf : public neutral_mob
{
	float timeForNewSprite;

	int animationLength;
	bool wasHit = false;
	float timeAfterHit = 0, timeForNewHit = 100000;
	int strikingSprite = 4;
public:
	wolf(std::string objectName, Vector2f centerPosition);
	~wolf();
	Vector2f calculateTextureOffset() override;
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
	void behaviorWithStatic(world_object* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	Vector2f getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	void setTarget(dynamic_object& object) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;
};
