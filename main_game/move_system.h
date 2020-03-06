#pragma once
#ifndef MOVESYSTEM_H
#define MOVESYSTEM_H

#include <SFML/Graphics/Color.hpp>
#include <vector>

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
	void end_push(long long elapsed_time);
	void push_by_bumping(Vector2f position, float radius, bool can_crash);
	void set_move_offset(long long elapsedTime);
	[[nodiscard]] Vector2f ellipseSlip(Vector2f newPos, Vector2f destination, Vector2f f1, Vector2f f2, float ellipseSize, float height, long long elapsedTime) const;
	Vector2f doMove(long long elapsedTime);
	Vector2f doSlip(Vector2f newPosition, const std::vector<static_object*>& localStaticItems, float height, long long elapsedTime);	
	
	bool turned_on = false;
	bool can_crash_into_dynamic = true;
	bool can_crash_into_static = false;
	Vector2f move_offset = Vector2f(-1, -1), move_position = { -1, -1 };
	float default_speed = 0;
	float speed = 0;

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