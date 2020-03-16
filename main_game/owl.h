#pragma once

#include "helper.h"
#include "neutral_mob.h"


using namespace sf;

class owl : public neutral_mob
{
public:
	owl(const std::string& object_name, Vector2f center_position);
	~owl() override;
	Vector2f calculate_texture_offset() override;
	std::vector<sprite_chain_element*> prepare_sprites(long long elapsed_time) override;
	void behavior_with_dynamic(dynamic_object* target, long long elapsedTime) override;
	void behavior_with_static(world_object* target, long long elapsed_time) override;
	void behavior(long long elapsed_time) override;
	void ending_previous_action();
	void jerk(float power, float deceleration, Vector2f destination_point) override;
private:
	float min_flight_distance_ = 0;
	long long time_after_fear_ = long(1e6);
	long long fear_time_ = long(1e6);
	Vector2f cheat_hero_pos_ = { 0, 0 };
};
