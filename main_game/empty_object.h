#pragma once

#include "world_object.h"

class empty_object : public world_object
{
public:
	empty_object(std::string objectName, sf::Vector2f centerPosition);
	~empty_object();
	std::vector<unique_ptr<sprite_chain_element>> prepare_sprites(long long elapsedTime) override;
	int get_sprite_number() override;
	sf::Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition) override;
	int get_build_type(sf::Vector2f ounPos, sf::Vector2f otherPos) override;
	void set_texture_size(sf::Vector2f textureSize) override;
	sf::Vector2f calculate_texture_offset() override;
	void init_pedestal() override;
};

