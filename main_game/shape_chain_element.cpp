#include "shape_chain_element.h"

const float shape_chain_element::default_character_size = helper::GetScreenSize().y * 0.024f;

shape_chain_element::shape_chain_element() : drawable_chain_element()
{
}

shape_chain_element::shape_chain_element(
	Vector2f position,
	float radius,
	Vector2f offset,
	sf::Color color,
	float rotation) : drawable_chain_element(position, { 0, 0 }, offset, color)
{
	type = shape_type::circle;
	this->radius = radius;	
	this->rotation = rotation;
}

shape_chain_element::shape_chain_element(
	Vector2f position,
	Vector2f offset,
	sf::Color color,
	float rotation) : drawable_chain_element(position, { 0, 0 }, offset, color)
{
	type = shape_type::rectangle;
	this->rotation = rotation;
}

shape_chain_element::~shape_chain_element()
= default;
