#pragma once

#include <SFML/Graphics.hpp>

#include "world_object.h"

class empty_object : public world_object
{
public:
	empty_object(std::string objectName, Vector2f centerPosition);
	~empty_object();
	std::vector<sprite_chain_element*> prepare_sprites(long long elapsedTime) override;
	int get_sprite_number() override;
	Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int get_build_type(Vector2f ounPos, Vector2f otherPos) override;
	void set_texture_size(Vector2f textureSize) override;
	Vector2f calculate_texture_offset() override;
	void init_pedestal() override;
};

