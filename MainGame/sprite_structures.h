#pragma once

#include <SFML/Graphics.hpp>

#include "sprite_pack.h"

using namespace sf;

struct board_sprite
{
	Texture texture;
	Sprite sprite;
};

struct cell_sprite {
	Sprite sprite;
	Texture texture;
	Vector2f offset;
};
