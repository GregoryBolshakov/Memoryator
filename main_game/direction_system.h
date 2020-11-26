#pragma once

#include "tags.h"

#include <SFML/System/Vector2.hpp>
#include <string>

class direction_system
{
public:
	direction_system(sf::Vector2f& position, sf::Vector2f& move_position, bool& mirrored);
	~direction_system() = default;
	void calculate_direction(const long long elapsed_time);
	void set_direction_from_8(const sf::Vector2f move_offset, const long long elapsed_time);
	void set_direction_from_12(const sf::Vector2f move_offset, const long long elapsed_time);
	side calculate_side(sf::Vector2f position, sf::Vector2f other_object_position, const long long elapsed_time);
	static side invert_side(side side);
	static std::string side_to_string(side side);
	static std::string direction_to_string(direction direction);
	static direction cut_rights(direction direction);
	static side cut_rights(side side);
	static direction side_to_direction(side side);
	static direction invert_direction(direction dir);
	static direction cut_diagonals(direction dir);
	static float calculate_angle(sf::Vector2f move_offset);

	side side = side::down;
	direction direction = direction::STAND, last_direction = direction::DOWN;
	long long time_for_new_direction = long(1e5), time_after_new_direction = long(1e5);
	long long time_for_new_side = long(1e5), time_after_new_side = long(1e5);

	// owner info
	sf::Vector2f& position;
	sf::Vector2f& move_position;
	bool& mirrored;
	//-----------
};
