#pragma once

#include <SFML/Graphics/Color.hpp>
#include <vector>

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
	void init(float* radius, Vector2f* position, sf::Color* color);
	void turn_off() { turned_on = false; }
	void push_away(long long elapsed_time, float push_speed = 0);
	void end_push(long long elapsed_time);
	void push_by_bumping(Vector2f position, float radius, bool can_crash);

	bool turned_on = false;
	bool can_crash_into_dynamic = true;
	bool can_crash_into_static = false;
	Vector2f move_offset = Vector2f(-1, -1);
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

	// owner info
	float* radius = nullptr;
	Vector2f* position = nullptr;
	sf::Color* color = nullptr;
};
