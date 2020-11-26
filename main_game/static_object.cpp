#include "static_object.h"

static_object::static_object(std::string objectName, sf::Vector2f centerPosition) : world_object(std::move(objectName), centerPosition)
{
}

void static_object::set_position(sf::Vector2f new_position)
{
	position_ = sf::Vector2f(new_position);
	texture_box_.left = ceil(new_position.x - texture_box_offset_.x);
	texture_box_.top = ceil(new_position.y - texture_box_offset_.y);
}

static_object::~static_object()
= default;
