#include "drawable_chain_element.h"


drawable_chain_element::drawable_chain_element()
= default;

drawable_chain_element::drawable_chain_element(
	Vector2f position,
	Vector2f size,
	Vector2f offset,
	Color color,
	float rotation)
{
	this->position = position;
	this->size = size;
	this->offset = offset;
	this->color = color;
	this->rotation = rotation;
	initialized = true;
}
