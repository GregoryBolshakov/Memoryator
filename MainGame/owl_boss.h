#pragma once

#include "dynamic_object.h"
#include "Helper.h"

using namespace sf;

class owl_boss : public dynamic_object
{
private:
	float timeForNewSprite = 0;
	float jerkDistance = 0;
	int animationLength;
	int strikingSprite = 1;
	int flapsBeforeJerkCount = 4;
	bool wasHit = false;
public:
	owl_boss(std::string objectName, Vector2f centerPosition);
	~owl_boss();
	Vector2f calculateTextureOffset() override;
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
	void behaviorWithDynamic(dynamic_object* target, long long elapsedTime) override;
	void behaviorWithStatic(world_object* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	Vector2f getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	void setTarget(dynamic_object& object) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;
	void fightInteract(long long elapsedTime, dynamic_object* target = nullptr) override;
};
