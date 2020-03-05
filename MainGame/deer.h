#pragma once

#include "helper.h"
#include "neutral_mob.h"


using namespace sf;

class deer : public neutral_mob
{
public:
	deer(const std::string& objectName, Vector2f centerPosition);
	~deer() override;
	Vector2f calculate_texture_offset() override;
	std::vector<sprite_chain_element*> prepare_sprites(long long elapsedTime) override;
	void behaviorWithStatic(world_object* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void endingPreviousAction();
	Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	Vector2f getHeadPosition();
	int get_build_type(Vector2f ounPos, Vector2f otherPos) override;

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
	long long timeForNewSprite;
	int animationLength;
	bool wasHit = false;
	float timeAfterHit = 0, timeForNewHit = 100000;
	int strikingSprite = 4;
	world_object* owner = nullptr;
};
