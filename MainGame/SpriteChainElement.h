#pragma once
#ifndef SPRITECHAINELEMENT_H
#define SPRITECHAINELEMENT_H

#include <SFML/Graphics.hpp>
#include "DrawableChainElement.h"
#include "Tags.h"

class SpriteChainElement : public DrawableChainElement
{
public:
    SpriteChainElement();
    SpriteChainElement(
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
	SpriteChainElement(
		Vector2f position,
		Vector2f size,
		Vector2f offset,
		Color color = Color(255, 255, 255, 255),
		bool mirrored = false,
		bool unscaled = false,
		float rotation = 0);
    virtual ~SpriteChainElement();
	void setDrawInfo(PackTag tag, PackPart part, Direction direction, int number = 1);
	DrawableChainElement* upCast() { DrawableChainElement* result = this; return result; }

	PackTag packTag = PackTag::empty; PackPart packPart = PackPart::full; Direction direction = Direction::DOWN;
	
	int number = 1, animationLength = 1; int zCoord = 1, finishSprite = 0;
    bool isBackground = false, mirrored = false, antiTransparent = false, unscaled = false;	
};

#endif