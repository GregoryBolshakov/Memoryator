#pragma once

#include "Helper.h"
#include "NeutralMob.h"

using namespace sf;

class Hare : public NeutralMob
{
public:
	Hare(const std::string& objectName, Vector2f centerPosition);
	~Hare();
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
	long long timeForNewSprite;
	int animationLength{};
	long long timeAfterFear = 0;
	long long fearTime = long(2 * 10e5);
};
