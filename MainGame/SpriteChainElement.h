#pragma once
#ifndef SPRITECHAINELEMENT_H
#define SPRITECHAINELEMENT_H

#include <SFML/Graphics.hpp>
#include "Tags.h"

using namespace sf;

class SpriteChainElement
{
public:
    SpriteChainElement();
    SpriteChainElement(
        PackTag tag,
        PackPart part,
        Direction direction,
        int number = 1,
        Vector2f position = {0, 0},
        Vector2i size = {0, 0},
        Vector2f offset = {0, 0},
		Color color = Color(255, 255, 255, 255),
		bool mirrored = false,
        float rotation = 0);
    ~SpriteChainElement();

	PackTag packTag = PackTag::empty; PackPart packPart = PackPart::full; Direction direction = Direction::DOWN;
	Vector2f offset = { 0, 0 }, position = { 0, 0 };
    Vector2i size = {0, 0};
	float rotation = 0;
	int number = 1, animationLength = 1; int zCoord = 1, finishSprite = 0;
    bool isBackground = false, mirrored = false, antiTransparent = false;
	Tag tag = Tag::emptyCell;
	Color color = Color(255, 255, 255);
};

#endif