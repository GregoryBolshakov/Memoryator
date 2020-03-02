#pragma once

#include <SFML/Graphics.hpp>

#include "drawable_chain_element.h"
#include "Tags.h"

class sprite_chain_element : public drawable_chain_element
{
public:
    sprite_chain_element();
    sprite_chain_element(
        PackTag tag,
        PackPart part,
        Direction direction,
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
    virtual ~sprite_chain_element();
	void setDrawInfo(PackTag tag, PackPart part, Direction direction, int number = 1);
	drawable_chain_element* upCast() { drawable_chain_element* result = this; return result; }

	PackTag packTag = PackTag::empty; PackPart packPart = PackPart::full; Direction direction = Direction::DOWN;
	
	int number = 1, animationLength = 1; int zCoord = 1, finishSprite = 0;
    bool isBackground = false, mirrored = false, antiTransparent = false, unscaled = false;	
};
