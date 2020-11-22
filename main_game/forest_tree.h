#pragma once

#include "terrain_object.h"

class forest_tree : public terrain_object
{
public:
	forest_tree(std::string object_name, Vector2f center_position, int type_of_object);
	Vector2f calculate_texture_offset() override;	
	void init_pedestal() override;
	std::vector<std::unique_ptr<sprite_chain_element>> prepare_sprites(long long elapsed_time) override;
	void setType(int type_of_object) override;
	Vector2f get_build_position(std::vector<world_object*> visible_items, float scale_factor, Vector2f camera_position) override;
	Vector2f get_owl_landing_pos() override;
	int get_build_type(Vector2f oun_pos, Vector2f other_pos) override;

	int strength;
};
