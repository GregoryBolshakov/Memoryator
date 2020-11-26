#include "drawable_chain_element.h"


drawable_chain_element::drawable_chain_element()
= default;

drawable_chain_element::drawable_chain_element(
	sf::Vector2f position,
	sf::Vector2f size,
	sf::Vector2f offset,
	sf::Color color,
	float rotation)
{
	this->position = position;
	this->size = size;
	this->offset = offset;
	this->color = color;
	this->rotation = rotation;
	initialized = true;
}
