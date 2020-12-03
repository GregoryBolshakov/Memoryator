#pragma once

#include "terrain_object.h"

class forest_tree : public terrain_object
{
public:
	forest_tree(std::string name, sf::Vector2f position, int kind);
	void init_pedestal() override;
	std::vector<std::unique_ptr<sprite_chain_element>> prepare_sprites(long long elapsed_time) override;
};
