//#include "hare_trap.h"
//
//#include "helper.h"
//
//hare_trap::hare_trap(std::string object_name, const Vector2f center_position, const int type_of_object) : picked_object(std::move(object_name), center_position)
//{
//	variety_of_types_ = 1;
//	this->type_of_object_ = type_of_object;
//	radius_ = 100;
//	to_save_name_ = "hareTrap";
//	hare_trap::setType(type_of_object);
//	current_sprite_[0] = 1;
//	id = entity_tag::hareTrap;
//	tag = entity_tag::hareTrap;
//	inventory = hero_bag::empty_inventory;
//}
//
//void hare_trap::setType(int type_of_object)
//{
//	this->type_of_object_ = type_of_object;
//	this->conditional_size_units_ = { 140, 111 };
//}
//
//Vector2f hare_trap::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//	return { texture_box_.width / 2.0f, texture_box_.height / 1.4f };
//}
//
//void hare_trap::init_pedestal()
//{
//	focus1_ = position_;
//	focus2_ = position_;
//	ellipse_size_multipliers[0] = { 0 };
//}
//
//Vector2f hare_trap::get_build_position(std::vector<world_object*> visible_items, float scale_factor, Vector2f camera_position)
//{
//	return { -1, -1 };
//}
//
//Vector2f hare_trap::get_enter_position() const
//{
//	return {position_.x + conditional_size_units_.x / 2.0f, position_.y/* + conditional_size_units_.y / 10.0f*/ };
//}
//
//int hare_trap::get_build_type(Vector2f oun_pos, Vector2f other_pos)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> hare_trap::prepare_sprites(long long elapsed_time)
//{
//	std::vector<sprite_chain_element*> result = {};
//	const auto body = new sprite_chain_element(pack_tag::craftObjects, pack_part::hareTrap, direction::DOWN, 1, position_, conditional_size_units_, texture_box_offset_);
//	const auto front_position = get_enter_position() + Vector2f(0, 1);
//	const auto front_offset = texture_box_offset_ + (front_position - position_);
//	const auto front = new sprite_chain_element(pack_tag::craftObjects, pack_part::hareTrap, direction::DOWN, 2, front_position, conditional_size_units_, front_offset);
//
//	result = { body, front };
//	set_unscaled(result);
//	
//	return result;
//}