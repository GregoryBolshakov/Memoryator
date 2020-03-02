#pragma once

#include "Helper.h"
#include "neutral_mob.h"


using namespace sf;

class bear : public neutral_mob
{
public:
	bear(const std::string& objectName, Vector2f centerPosition);
	~bear() override;
	Vector2f calculateTextureOffset() override;
	std::vector<SpriteChainElement*> prepareSprites(long long elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void endingPreviousAction();
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;

	[[nodiscard]] WorldObject* getOwner() const
	{
		return owner;
	}

	//void setTarget(DynamicObject& object) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;

	void setOwner(WorldObject* owner)
	{
		this->owner = owner;
	}
protected:
	float timeForNewSprite;

	int animationLength;
	bool wasHit = false;
	float timeAfterHit = 0, timeForNewHit = 100000;
	int strikingSprite = 4;
	WorldObject* owner = nullptr;
};
