//#pragma once
//#ifndef HARE_TRAP_H
//#define HARE_TRAP_H
//
//#include "picked_object.h"
//
//class hare_trap : public picked_object
//{
//public:
//	hare_trap(std::string object_name, sf::Vector2f center_position, int type_of_object);
//	sf::Vector2f calculate_texture_offset() override;
//	void init_pedestal() override;
//	std::vector<sprite_chain_element*> prepare_sprites(long long elapsed_time) override;
//	void setType(int type_of_object) override;
//	sf::Vector2f get_build_position(std::vector<world_object*> visible_items, float scale_factor, sf::Vector2f camera_position) override;
//	[[nodiscard]] sf::Vector2f get_enter_position() const;
//	int get_build_type(sf::Vector2f oun_pos, sf::Vector2f other_pos) override;
//};
//
//#endif