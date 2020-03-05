#include "sprite_chain_element.h"

sprite_chain_element::sprite_chain_element() : drawable_chain_element()
{
}

sprite_chain_element::sprite_chain_element(
	const ::pack_tag tag,
	const ::pack_part part,
	const ::direction direction,
	const int number,
	const Vector2f position,
	const Vector2f size,
	const Vector2f offset,
	const Color color,
	const bool mirrored,
	const bool unscaled,
	const float rotation) : drawable_chain_element(position, size, offset, color)
{
    this->pack_tag = tag;
    this->pack_part = part;
    this->direction = direction;
    this->number = number;
	this->mirrored = mirrored;
	this->unscaled = unscaled;
    this->rotation = rotation;
}

sprite_chain_element::sprite_chain_element(
	const Vector2f position,
	const Vector2f size,
	const Vector2f offset,
	const Color color,
	const bool mirrored,
	const bool unscaled,
	const float rotation) : drawable_chain_element(position, size, offset, color)
{
	this->mirrored = mirrored;
	this->unscaled = unscaled;
	this->rotation = rotation;
}

sprite_chain_element::~sprite_chain_element()
= default;

void sprite_chain_element::set_draw_info(const ::pack_tag tag, const ::pack_part part, const ::direction direction, const int number) 
{
	this->pack_tag = tag;
	this->pack_part = part;
	this->direction = direction;
	this->number = number;
}
