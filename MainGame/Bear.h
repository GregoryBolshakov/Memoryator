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
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
	void behaviorWithStatic(world_object* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void endingPreviousAction();
	Vector2f getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;

	[[nodiscard]] world_object* getOwner() const
	{
		return owner;
	}

	//void setTarget(DynamicObject& object) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;

	void setOwner(world_object* owner)
	{
		this->owner = owner;
	}
protected:
	float timeForNewSprite;

	int animationLength;
	bool wasHit = false;
	float timeAfterHit = 0, timeForNewHit = 100000;
	int strikingSprite = 4;
	world_object* owner = nullptr;
};
