#pragma once
#ifndef MOVE_SYSTEM_H
#define MOVE_SYSTEM_H

#include <SFML/Graphics/Color.hpp>
#include <vector>

#include "grid_list.h"
#include "terrain_object.h"
#include "direction_system.h"

struct bump_chain_element
{
	bump_chain_element(Vector2f position, bool cancelable) : position{ position }, cancelable{ cancelable } {};
	Vector2f position;
	bool cancelable = true;
};

class move_system
{
public:
	move_system();
	virtual ~move_system();
	void init(entity_tag* tag, float* radius, Vector2f* position, sf::Color* color, actions* current_action, direction_system* direction_system);
	void turn_off() { turned_on = false; }
	void push_away(long long elapsed_time, float push_speed = 0);
	void end_push();
	void push_by_bumping(Vector2f position, float radius, bool can_crash);
	void set_move_offset(long long elapsedTime);
	[[nodiscard]] Vector2f ellipse_slip(Vector2f new_pos, Vector2f destination, Vector2f f1, Vector2f f2, float ellipse_size, float height, long long elapsed_time) const;
	Vector2f do_move(long long elapsedTime);
	[[nodiscard]] sf::Vector2f do_slip(
		Vector2f new_position,
		std::vector<static_object*>& local_static_items,
		float height,
		long long elapsed_time) const;
	void is_route_needed(std::vector<std::vector<bool>>& micro_block_matrix, Vector2f& micro_block_size);
	void make_route(long long elapsed_time, grid_list* grid_list, float zone_offset);
	void pass_route_beginning(Vector2f micro_block_size);
	void reset_timers() { time_after_new_route = time_for_new_route; direction_system_->time_after_new_direction = direction_system_->time_for_new_direction; }
	
	std::vector<std::pair<int, int>> route = { {} };
	bool turned_on = false, need_route = false, route_generation_ability = true;;
	bool can_crash_into_dynamic = true;
	bool can_crash_into_static = true;
	Vector2f move_offset = { -1, -1 }, move_position = { -1, -1 }, lax_move_position = { -1, -1 };
	float default_speed = 0;
	float speed = 0;
	long long time_for_new_route = long(3e5), time_after_new_route = long(3e5);

	std::vector<bump_chain_element> bumped_positions = {};
	float bump_distance = 0;
	float push_damage = 0;
	float push_distance = 0;
	Vector2f push_direction = { 0, 0 };
	Vector2f push_vector = { 0, 0 };
	long long push_duration = 0;
	long long push_rest_duration = 0;
	long long red_duration = 0;
	long long red_rest_duration = 0;
	const long long default_push_duration = long(2e5);
	const float default_push_speed = 0.00025f;

private:
	// owner info
	entity_tag* tag = nullptr;
	float* radius_ = nullptr;
	Vector2f* position_ = nullptr;
	sf::Color* color_ = nullptr;
	actions* current_action_ = nullptr;
	direction_system* direction_system_ = nullptr;
};

#endif