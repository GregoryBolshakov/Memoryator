#include "DrawableChainElement.h"


DrawableChainElement::DrawableChainElement()
= default;

DrawableChainElement::DrawableChainElement(
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

DrawableChainElement::~DrawableChainElement()
= default;
