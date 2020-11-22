#pragma once

#include "terrain_object.h"

class ground_connection : public static_object
{
public:
	ground_connection(std::string object_name, Vector2f center_position, int type_of_object);
	Vector2f calculate_texture_offset() override;
	std::vector<unique_ptr<sprite_chain_element>> prepare_sprites(long long elapsedTime) override;
	int strength{};
	void setType(int type_of_object) override;
	Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int get_build_type(Vector2f ounPos, Vector2f otherPos) override;
};
