#pragma once
#ifndef HARE_TRAP_H
#define HARE_TRAP_H

#include "picked_object.h"

class hare_trap : public picked_object
{
public:
	hare_trap(std::string object_name, Vector2f center_position, int type_of_object);
	Vector2f calculate_texture_offset() override;
	void init_pedestal() override;
	std::vector<sprite_chain_element*> prepare_sprites(long long elapsed_time) override;
	void setType(int type_of_object) override;
	Vector2f get_build_position(std::vector<world_object*> visible_items, float scale_factor, Vector2f camera_position) override;
	[[nodiscard]] Vector2f get_enter_position() const;
	int get_build_type(Vector2f oun_pos, Vector2f other_pos) override;
};

#endif