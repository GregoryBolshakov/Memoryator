#pragma once

#include <SFML/Graphics.hpp>

#include "drawable_chain_element.h"
#include "tags.h"

class sprite_chain_element : public drawable_chain_element
{
public:
	sprite_chain_element();
	sprite_chain_element(
	    pack_tag tag,
	    pack_part part,
	    direction direction,
	    int number = 1,
	    Vector2f position = {0, 0},
	    Vector2f size = {0, 0},
	    Vector2f offset = {0, 0},
		Color color = Color(255, 255, 255, 255),
		bool mirrored = false,
		bool unscaled = false,
		float rotation = 0);
	sprite_chain_element(
		Vector2f position,
		Vector2f size,
		Vector2f offset,
		Color color = Color(255, 255, 255, 255),
		bool mirrored = false,
		bool unscaled = false,
		float rotation = 0);
	sprite_chain_element(sprite_chain_element&& element) noexcept : drawable_chain_element(std::move(element))
{
		this->pack_tag = element.pack_tag;
		this->pack_part = element.pack_part;
		this->direction = element.direction;
		this->number = element.number;
		this->animation_length = element.animation_length;
		this->z_coordinate = element.z_coordinate;
		this->finish_sprite = element.finish_sprite;
		this->is_background = element.is_background;
		this->mirrored = element.mirrored;
		this->anti_transparent = element.anti_transparent;
		this->unscaled = element.unscaled;
	}
	virtual ~sprite_chain_element() = default;
	void set_draw_info(pack_tag tag, pack_part part, direction direction, int number = 1);
	//drawable_chain_element* up_cast() { drawable_chain_element* result = this; return result; }

	pack_tag pack_tag = pack_tag::empty; pack_part pack_part = pack_part::full; direction direction = direction::DOWN;

	int number = 1;
	int animation_length = 1;
	int z_coordinate = 1;
	int finish_sprite = 0;
	bool is_background = false;
	bool mirrored = false;
	bool anti_transparent = false;
	bool unscaled = false;
};
