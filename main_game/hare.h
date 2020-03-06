#pragma once

#include "helper.h"
#include "neutral_mob.h"

using namespace sf;

class hare : public neutral_mob
{
public:
	hare(const std::string& objectName, Vector2f centerPosition);
	~hare();
	Vector2f calculate_texture_offset() override;
	std::vector<sprite_chain_element*> prepare_sprites(long long elapsedTime) override;
	void set_target(dynamic_object& object) override;
	void behavior_with_static(world_object* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void endingPreviousAction();
	Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int get_build_type(Vector2f ounPos, Vector2f otherPos) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;

private:
	long long timeForNewSprite;
	int animationLength{};
	long long timeAfterFear = 0;
	long long fearTime = long(2 * 10e5);
};
