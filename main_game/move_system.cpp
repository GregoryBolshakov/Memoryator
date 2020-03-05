#include "move_system.h"

#include <SFML/Graphics/Color.hpp>

#include "helper.h"

move_system::move_system()
= default;

move_system::~move_system()
= default;

void move_system::init(float* radius, Vector2f* position, sf::Color* color)
{
	this->radius = radius;
	this->position = position;
	this->color = color;
	turned_on = true;
}

void move_system::push_by_bumping(Vector2f position, float radius, bool can_crash)
{
	if (can_crash_into_dynamic || can_crash || push_damage != 0)
		return;
	bumped_positions.emplace_back(position, false);
	bump_distance += radius;
}

void move_system::end_push(long long elapsed_time)
{
	push_direction = { 0, 0 };
	push_duration = 0;
	push_vector = { 0, 0 };
	push_damage = 0;
	bumped_positions.clear();
	bump_distance = 0;
}

void move_system::push_away(long long elapsed_time, float push_speed)
{
	if (push_speed == 0)
		push_speed = default_push_speed;

	const auto push_color = sf::Color(255, 100, 100, 255);

	if (!bumped_positions.empty() && this->can_crash_into_dynamic && push_damage == 0)
	{
		Vector2f bump_center = { 0, 0 };
		auto bump_count = 0;
		for (const auto bump : bumped_positions)
		{
			bump_count++;
			bump_center.x += bump.position.x;
			bump_center.y += bump.position.y;
		}
		if (bump_count == 0 || push_speed == 0)
			return;
		bump_center.x /= float(bump_count);
		bump_center.y /= float(bump_count);
		push_distance = (*radius + bump_distance) - helper::getDist(*position, bump_center);
		push_direction = Vector2f(position->x - bump_center.x, position->y - bump_center.y);
		push_duration = long(push_distance / push_speed);
		push_rest_duration = push_duration;
		bump_distance = 0;
		bumped_positions.clear();
		*color = push_color;
	}

	if (red_rest_duration > 0)
		*color = sf::Color(
			255,
			push_color.g + Uint8((255.0f - float(push_color.g)) * (1.0f - float(red_rest_duration) / float(red_duration))),
			push_color.b + Uint8((255.0f - float(push_color.b)) * (1.0f - float(red_rest_duration) / float(red_duration))),
			255);
	else
	{
		*color = sf::Color::White;
		red_rest_duration = 0;
	}

	red_rest_duration -= elapsed_time;

	if (push_rest_duration <= 0 || !turned_on)
	{
		end_push(elapsed_time);
		return;
	}

	push_rest_duration -= elapsed_time;

	const float elongation_coefficient = push_speed * float(elapsed_time) / sqrt(pow(push_direction.x, 2) + pow(push_direction.y, 2));
	if (push_direction != Vector2f(0, 0))
		push_vector = { elongation_coefficient * push_direction.x, elongation_coefficient * push_direction.y };
	else
		push_vector = { 0, 0 };
}
