#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

class light_system
{
public:
	light_system();
	~light_system();
	void init(sf::FloatRect visibleArea);
	sf::Vector2f calculate_sun_position(float day_part = 0); // from 0 to 1
private:
	float calculate_horizontal_offset(float day_part);

	sf::FloatRect visible_area_ = { 0, 0, 0, 0 };
	sf::Vector2f circle_center_ = { 0, 0 }, calculations_center_ = { 0, 0 };
	float calculations_radius_ = 0;
};
