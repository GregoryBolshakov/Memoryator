#pragma once

#include <SFML/Graphics.hpp>

#include "sprite_pack.h"

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
