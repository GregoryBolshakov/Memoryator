#pragma once

#include "DynamicObject.h"
#include "Helper.h"

using namespace sf;

class owl_boss : public DynamicObject
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
	std::vector<SpriteChainElement*> prepareSprites(long long elapsedTime) override;
	void behaviorWithDynamic(DynamicObject* target, long long elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	void setTarget(DynamicObject& object) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;
	void fightInteract(long long elapsedTime, DynamicObject* target = nullptr) override;
};
