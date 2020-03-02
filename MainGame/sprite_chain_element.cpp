#include "sprite_chain_element.h"

sprite_chain_element::sprite_chain_element() : drawable_chain_element()
{
}

sprite_chain_element::sprite_chain_element(
    pack_tag tag,
    pack_part part,
    Direction direction,
    int number, 
    Vector2f position,
    Vector2f size,
    Vector2f offset,
	Color color,
	bool mirrored,
	bool unscaled,
    float rotation) : drawable_chain_element(position, size, offset, color)
{
    this->packTag = tag;
    this->packPart = part;
    this->direction = direction;
    this->number = number;
	this->mirrored = mirrored;
	this->unscaled = unscaled;
    this->rotation = rotation;
}

sprite_chain_element::sprite_chain_element(
	Vector2f position,
	Vector2f size,
	Vector2f offset,
	Color color,
	bool mirrored,
	bool unscaled,
	float rotation) : drawable_chain_element(position, size, offset, color)
{
	this->mirrored = mirrored;
	this->unscaled = unscaled;
	this->rotation = rotation;
}

sprite_chain_element::~sprite_chain_element()
= default;

void sprite_chain_element::setDrawInfo(pack_tag tag, pack_part part, Direction direction, int number) 
{
	this->packTag = tag;
	this->packPart = part;
	this->direction = direction;
	this->number = number;
}
