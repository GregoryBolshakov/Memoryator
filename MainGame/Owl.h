#pragma once

#include "neutral_mob.h"
#include "Helper.h"

using namespace sf;

class Owl : public neutral_mob
{
private:
	float timeForNewSprite;

	int animationLength;
	bool wasHit = false;
	float timeAfterHit = 0, timeForNewHit = 10e5, timeAfterFear = 0, fearTime = 2 * 10e5;
	int strikingSprite = 4;
	WorldObject* owner = nullptr, *nearestTree = nullptr;
public:
	Owl(const std::string& objectName, Vector2f centerPosition);
	~Owl();
	Vector2f calculateTextureOffset() override;
	std::vector<SpriteChainElement*> prepareSprites(long long elapsedTime) override;
	void setTarget(DynamicObject& object) override;
	void behaviorWithStatic(WorldObject* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void endingPreviousAction();
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;
};
