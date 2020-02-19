#pragma once
#ifndef SPRITETEXTURES_H
#define SPRITETEXTURES_H

#include <SFML/Graphics.hpp>
#include "SpritePack.h"

using namespace sf;

struct BoardSprite
{
	Texture texture;
	Sprite sprite;
};

struct CellSprite {
	Sprite sprite;
	Texture texture;
	Vector2f offset;
};

#endif
