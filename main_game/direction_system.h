#pragma once
#define DIRECTIONSYSTEM_H
#include <SFML/System/Vector2.hpp>
#include <string>

#include "tags.h"

using Vector2f = sf::Vector2f;

class direction_system
{
public:
	direction_system();
	~direction_system();
	void init(Vector2f* position, Vector2f* move_position, bool* mirrored) { this->position = position; this->move_position = move_position; this->mirrored = mirrored; }
	void calculate_direction(const long long elapsed_time);
	void set_mob_direction(const Vector2f move_offset, const long long elapsed_time);
	side calculate_side(Vector2f position, sf::Vector2f other_object_position, const long long elapsed_time);
	static side invert_side(side side);
	static std::string side_to_string(side side);
	static std::string direction_to_string(direction direction);
	static direction cut_rights(direction direction);
	static side cut_rights(side side);
	static direction side_to_direction(side side);
	static direction invert_direction(direction dir);
	static direction cut_diagonals(direction dir);
	static float calculate_angle(Vector2f move_offset);

	side side = down;
	direction direction = direction::STAND, last_direction = direction::DOWN;
	long long time_for_new_direction = long(1e5), time_after_new_direction = long(1e5);
	long long time_for_new_side = long(1e5), time_after_new_side = long(1e5);

	// owner info
	Vector2f* position = nullptr;
	Vector2f* move_position = nullptr;
	bool* mirrored = nullptr;
	//-----------
};
