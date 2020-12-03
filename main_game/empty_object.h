#pragma once

#include "world_object.h"

class empty_object : public world_object
{
public:
	empty_object(std::string objectName, sf::Vector2f centerPosition);
	~empty_object();
	std::vector<unique_ptr<sprite_chain_element>> prepare_sprites(long long elapsedTime) override;
	void init_pedestal() override;
};

