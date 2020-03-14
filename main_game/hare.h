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
	void behavior_with_static(world_object* target, long long elapsedTime) override;
	void behavior_with_dynamic(dynamic_object* target, long long elapsedTime) override;
	void behavior(long long elapsed_time) override;
	void bounce_to_trap();
	void run_away_from_enemy(long long elapsed_time);
	void calm_behavior(long long elapsed_time);
	void endingPreviousAction();
	Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int get_build_type(Vector2f ounPos, Vector2f otherPos) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;

private:
	bool calm_state_ = true, stand_ = true;
	std::vector<actions> calm_actions_stack = {};
	long long time_for_new_calm_state_ = long(15e5), time_after_calm_state_ = long(15e5);
	long long timeForNewSprite;
	int animationLength{};
	long long timeAfterFear = 0;
	long long fearTime = long(2 * 10e5);
};
