#pragma once

#include "terrain_object.h"

class ground_connection : public static_object
{
public:
	ground_connection(std::string name, sf::Vector2f position, int kind);
	std::vector<unique_ptr<sprite_chain_element>> prepare_sprites(long long elapsed_time) override;
private:
	static sf::Vector2f calculate_size(int kind);
	static sf::Vector2f calculate_offset(int kind, const sf::Vector2f size);
	static int calculate_z_coord(int kind);
};
