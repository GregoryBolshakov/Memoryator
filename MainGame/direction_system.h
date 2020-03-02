#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

#include "Tags.h"

const float pi = 3.14159265358979323846f;

using Vector2f = sf::Vector2f;

class direction_system
{
public:
	direction_system();
	~direction_system();
	void init(Vector2f* position, Vector2f* move_position) { this->position = position; this->move_position = move_position; }
	void calculate_direction();
	static side calculate_side(Vector2f position, sf::Vector2f other_object_position);
	static side invert_side(side side);
	static std::string side_to_string(side side);
	static std::string direction_to_string(direction direction);
	static direction cut_rights(direction direction);
	static side cut_rights(side side);
	static direction side_to_direction(side side);
	static direction invert_direction(direction dir);
	static direction cut_diagonals(direction dir);

	side side = down;
	direction direction = direction::STAND, last_direction = direction::DOWN;

	// owner info
	Vector2f* position = nullptr;
	Vector2f* move_position = nullptr;
	//-----------
};
