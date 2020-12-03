#pragma once

#include "terrain_object.h"

class ground : public static_object
{
public:
	ground(std::string name, sf::Vector2f position, int kind);
	std::vector<unique_ptr<sprite_chain_element>> prepare_sprites(long long elapsed_time) override;
};
