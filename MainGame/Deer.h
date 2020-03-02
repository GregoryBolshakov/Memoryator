#pragma once

#include "Helper.h"
#include "neutral_mob.h"


using namespace sf;

class deer : public neutral_mob
{
public:
	deer(const std::string& objectName, Vector2f centerPosition);
	~deer() override;
	Vector2f calculateTextureOffset() override;
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void endingPreviousAction();
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	Vector2f getHeadPosition();
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
	long long timeForNewSprite;
	int animationLength;
	bool wasHit = false;
	float timeAfterHit = 0, timeForNewHit = 100000;
	int strikingSprite = 4;
	WorldObject* owner = nullptr;
};
