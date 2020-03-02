#pragma once

#include "helper.h"
#include "neutral_mob.h"


using namespace sf;

class owl : public neutral_mob
{
public:
	owl(const std::string& objectName, Vector2f centerPosition);
	~owl() override;
	Vector2f calculate_texture_offset() override;
	std::vector<sprite_chain_element*> prepare_sprites(long long elapsedTime) override;
	void setTarget(dynamic_object& object) override;
	void behaviorWithStatic(world_object* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void endingPreviousAction();
	Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int get_build_type(Vector2f ounPos, Vector2f otherPos) override;
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
	world_object *owner = nullptr, *nearestTree = nullptr;
};
