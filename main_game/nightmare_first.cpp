//#include "nightmare_first.h"
//
//nightmare_first::nightmare_first(std::string objectName, const sf::Vector2f centerPosition) : monster(std::move(objectName), centerPosition)
//{
//	size_ = { 375, 375 };
//	move_system.default_speed = 0.0005f;
//	move_system.speed = 0.0005f;
//	radius_ = 90;
//	hitDistance = 70;
//	strength_ = 10;
//	health_ = 100;
//	current_action_ = relax;	
//	time_for_new_hit_self = long(2e5);
//	time_after_hitself_ = time_for_new_hit_self;
//	time_for_new_hit = long(1e6);
//	time_after_hit = time_for_new_hit;
//	to_save_name_ = "nightmare1_";
//	tag = entity_tag::nightmare1;
//}
//
//
//nightmare_first::~nightmare_first()
//= default;
//
//
//Vector2f nightmare_first::calculate_texture_offset()
//{	
//	init_route_blocks();
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//	return { texture_box_.width / 2, texture_box_.height * 7 / 8 };
//}
//
//void nightmare_first::doAttack(world_object* target)
//{
//	if (time_after_hit >= time_for_new_hit)
//	{
//		if (helper::getDist(position_, bound_target_->get_position()) <= (this->radius_ + bound_target_->get_radius() + hitDistance / 5))
//		{
//			change_action(common_hit, true, false);
//			time_after_hit = 0;
//		}
//	}
//}
//
//std::vector<unique_ptr<sprite_chain_element>> nightmare_first::prepare_sprites(const long long elapsed_time)
//{
//	std::vector<std::unique_ptr<sprite_chain_element>> result;
//	auto body = make_unique<sprite_chain_element>(pack_tag::nightmare1, pack_part::stand, direction::DOWN, 1, position_, size_, offset_, color, mirrored_, false);
//	animation_speed_ = 10;
//
//	side spriteSide = direction_system.side;
//	direction spriteDirection = direction_system.last_direction;
//
//	if (direction_system.side == right)
//	{
//		spriteSide = left;
//		body->mirrored = true;
//	}
//	if (direction_system.last_direction == direction::RIGHT || direction_system.last_direction == direction::UPRIGHT || direction_system.last_direction == direction::DOWNRIGHT)
//	{
//		spriteDirection = direction_system::cut_rights(spriteDirection);
//		body->mirrored = true;
//	}
//
//	body->direction = direction_system::side_to_direction(spriteSide);
//
//	switch (current_action_)
//	{
//	case common_hit:
//	{
//		animationLength = 9;
//		body->pack_tag = pack_tag::nightmare1;
//		body->pack_part = pack_part::hit;
//		break;
//	}
//	case combat_state:
//	case relax:
//	{
//		animationLength = 13;
//		body->pack_tag = pack_tag::nightmare1;
//		body->pack_part = pack_part::stand;
//		break;
//	}
//	case dead:
//	{
//		animationLength = 1;
//		body->pack_tag = pack_tag::nightmare1;
//		body->pack_part = pack_part::stand;
//		current_sprite_[0] = 1;
//		delete_promise_on();
//		break;
//	}
//	case move:
//	{
//		animationLength = 8;
//		body->pack_tag = pack_tag::nightmare1;
//		body->pack_part = pack_part::move;
//		break;
//	}
//	default:;
//	}
//
//	body->number = current_sprite_[0];
//
//	time_for_new_sprite_ += elapsed_time;
//
//	if (time_for_new_sprite_ >= long(1e6 / animation_speed_))
//	{
//		time_for_new_sprite_ = 0;
//
//		if (++current_sprite_[0] > animationLength)
//		{
//			last_action_ = current_action_;
//			current_sprite_[0] = 1;
//		}
//	}
//
//	result.emplace_back(std::move(body));
//
//	return result;
//}
//
