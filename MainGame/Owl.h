#pragma once

#include "Helper.h"
#include "neutral_mob.h"


using namespace sf;

class owl : public neutral_mob
{
public:
	owl(const std::string& objectName, Vector2f centerPosition);
	~owl() override;
	Vector2f calculateTextureOffset() override;
	std::vector<SpriteChainElement*> prepareSprites(long long elapsedTime) override;
	void setTarget(DynamicObject& object) override;
	void behaviorWithStatic(WorldObject* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void endingPreviousAction();
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;
private:
	float timeForNewSprite;

	int animationLength;
	bool wasHit = false;
	float timeAfterHit = 0;
	float timeForNewHit = 10e5;
	float timeAfterFear = 0;
	float fearTime = 2 * 10e5;
	int strikingSprite = 4;
	WorldObject *owner = nullptr, *nearestTree = nullptr;
};
