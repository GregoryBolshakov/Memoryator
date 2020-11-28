#include "sprite_chain_element.h"
#include "direction_system.h"

sprite_chain_element::sprite_chain_element() : drawable_chain_element()
	, pack_tag(pack_tag::empty)
	, pack_part(pack_part::full)
	, direction(direction::DOWN)
{
}

sprite_chain_element::sprite_chain_element(sprite_chain_element& element)
{
	if (this == &element)
		return;

	this->pack_tag = element.pack_tag;
	this->pack_part = element.pack_part;
	this->direction = element.direction;
	this->number = element.number;
	this->mirrored = element.mirrored;
	this->unscaled = element.unscaled;
	this->rotation = element.rotation;
}

sprite_chain_element::sprite_chain_element(
	const ::pack_tag tag,
	const ::pack_part part,
	const ::direction direction,
	const int number,
	const sf::Vector2f position,
	const sf::Vector2f size,
	const sf::Vector2f offset,
	const sf::Color color,
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
	const sf::Vector2f position,
	const sf::Vector2f size,
	const sf::Vector2f offset,
	const sf::Color color,
	const bool mirrored,
	const bool unscaled,
	const float rotation) : drawable_chain_element(position, size, offset, color)
{
	this->mirrored = mirrored;
	this->unscaled = unscaled;
	this->rotation = rotation;
}

void sprite_chain_element::set_draw_info(const ::pack_tag tag, const ::pack_part part, const ::direction direction, const int number) 
{
	this->pack_tag = tag;
	this->pack_part = part;
	this->direction = direction;
	this->number = number;
}
