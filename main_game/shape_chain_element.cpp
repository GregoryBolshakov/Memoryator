#include "shape_chain_element.h"
#include "world_metrics.h"

const float shape_chain_element::default_character_size = world_metrics::window_size.y * 0.024f;

shape_chain_element::shape_chain_element() : drawable_chain_element()
{
}

shape_chain_element::shape_chain_element(
	sf::Vector2f position,
	float radius,
	sf::Vector2f offset,
	bool hollow,
	sf::Color color,
	float rotation) : drawable_chain_element(position, { 0, 0 }, offset, color, rotation)
	, type(shape_type::circle)
	, radius(radius)
	, hollow(hollow)
{
}

shape_chain_element::shape_chain_element(
	sf::Vector2f position,
	sf::Vector2f size,
	sf::Vector2f offset,
	bool hollow,
	sf::Color color,
	float rotation) : drawable_chain_element(position, size, offset, color, rotation)
	, type(shape_type::rectangle)
	, hollow(hollow)
{
}
