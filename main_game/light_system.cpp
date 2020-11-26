#include "light_system.h"


light_system::light_system()
= default;


light_system::~light_system()
= default;

void light_system::init(sf::FloatRect visibleArea)
{
	this->visible_area_ = visibleArea;
	circle_center_ = { visibleArea.left + visibleArea.width / 2, visibleArea.top + visibleArea.height / 2 };
	calculations_radius_ = sqrt(pow(visibleArea.width / 2, 2) + pow(visibleArea.height / 2, 2));
	calculations_center_ = circle_center_; calculations_center_.y += calculations_radius_;
}

float light_system::calculate_horizontal_offset(const float day_part)
{
	if (day_part <= 0.5f) // high noon
		return -calculations_radius_ * (0.5f - day_part) / 0.5f;
	return calculations_radius_ * (day_part - 0.5f) / 0.5f;
}

sf::Vector2f light_system::calculate_sun_position(const float day_part)
{
	sf::Vector2f result = calculations_center_;
	const float horizontalOffset = calculate_horizontal_offset(day_part);
	result.x += horizontalOffset;
	const float discriminant = 4 * pow(calculations_radius_, 2) - 4 * pow(horizontalOffset, 2);
	result.y -= (2 * calculations_radius_ + sqrt(discriminant)) / 2;
	return result;
}
