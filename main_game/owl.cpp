//#include "owl.h"
//
//#include "fern.h"
//#include "forest_tree.h"
//
//using namespace sf;
//
//owl::owl(const std::string& object_name, Vector2f center_position) : neutral_mob(object_name, center_position)
//{
//	conditional_size_units_ = { 350, 250 };
//	move_system.default_speed = 0.0006f;
//	move_system.speed = 0.0006f;
//	radius_ = 70;
//	sight_range = 720;
//	run_away_range_ = 1300;
//	min_flight_distance_ = 500;
//	health_point_ = 50;
//	time_for_new_hit_self = long(6e5);
//	to_save_name_ = "owl";
//	tag = entity_tag::owl;
//	move_system.can_crash_into_static = false;
//	z_coordinate_ = 10;
//}
//
//owl::~owl()
//= default;
//
//Vector2f owl::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//	return { texture_box_.width / 2, texture_box_.height * 7 / 8 };
//}
//
//Vector2f abs(const Vector2f v) { return Vector2f(abs(v.x), abs(v.y)); }
//Vector2f operator / (const Vector2f v1, const Vector2f v2) { return Vector2f(v1.x / v2.x, v1.y / v2.y); }
//
//void owl::behavior_with_static(world_object* target, long long elapsed_time)
//{
//	const auto distance_to_target = helper::getDist(position_, target->get_position());
//
//	// set new landing pos
//	auto terrain = dynamic_cast<terrain_object*>(target);
//	if (current_action_ == relax && time_after_fear_ < fear_time_ &&
//		terrain && terrain->get_owl_landing_pos() != Vector2f(-1, -1))
//	{
//		const auto landing_pos = terrain->get_owl_landing_pos();
//		
//		if (!(landing_pos.x == position_.x || landing_pos.y == position_.y || 
//			position_.x == cheat_hero_pos_.x || position_.y == cheat_hero_pos_.y) &&
//				(landing_pos - position_) / abs(landing_pos - position_) ==
//					(position_ - cheat_hero_pos_) / abs(position_ - cheat_hero_pos_) &&
//						distance_to_target >= min_flight_distance_)
//		{
//			move_system.lax_move_position = landing_pos;
//			change_action(takeoff, true);
//		}
//	}
//	//--------------------
//}
//
//void owl::behavior_with_dynamic(dynamic_object* target, long long elapsedTime)
//{
//	if (target->tag == entity_tag::hero)
//		cheat_hero_pos_ = target->get_position();
//	
//	const auto distance_to_target = helper::getDist(position_, target->get_position());
//
//	if (distance_to_target <= sight_range && target->tag == entity_tag::hero)
//	{
//		time_after_fear_ = 0;
//		bound_target_ = target;
//	}
//}
//
//void owl::behavior(long long elapsed_time)
//{
//	ending_previous_action();
//	fight_interact(elapsed_time);
//	
//	if (die_check())
//		return;
//
//	calm_behavior(elapsed_time);
//	
//	land_on_a_tree();
//}
//
//void owl::land_on_a_tree()
//{
//	const auto minimal_contact_distance = (micro_block_size.x + micro_block_size.y) / 2;
//	if (helper::getDist(position_, move_system.lax_move_position) <= minimal_contact_distance)
//	{
//		time_after_fear_ = fear_time_;
//		current_action_ = landing;
//		move_system.lax_move_position = { -1, -1 };
//	}
//}
//
//void owl::calm_behavior(long long elapsed_time)
//{
//	if (time_after_fear_ < fear_time_)
//		return;
//
//	time_after_calm_state_ += elapsed_time;
//	if (time_after_calm_state_ < time_for_new_calm_state_ || current_action_ != relax)
//		return;
//
//	time_after_calm_state_ = rand() % (time_for_new_calm_state_ / 2);
//
//	std::vector<std::pair<actions, int>> actions = {};
//
//	actions = { {startle, 25} };
//
//	auto action_discriminant = rand() % 100 + 1;
//
//	for (const auto action : actions)
//	{
//		if (action.second >= action_discriminant)
//		{
//			change_action(action.first, true);
//			break;
//		}
//		action_discriminant -= action.second;
//	}
//}
//
//void owl::ending_previous_action()
//{
//	if (last_action_ == flap || last_action_ == soar)
//	{
//		const auto flap_probability = rand() % 2;
//		if (flap_probability)
//			current_action_ = flap;
//		else
//			current_action_ = soar;
//	}
//	if (last_action_ == takeoff)
//		current_action_ = flap;
//	if (last_action_ == flap || last_action_ == soar)
//	{
//		int soar_probability = rand() % 100 + 1;
//		if (soar_probability >= 50)
//			current_action_ = soar;
//		else 
//			current_action_ = flap;
//	}
//	if (last_action_ == landing)
//		current_action_ = relax;
//	if (last_action_ == common_hit || last_action_ == startle)
//		current_action_ = relax;
//	if (last_action_ == absorbs)
//	{
//		auto trap = dynamic_cast<fern*>(bound_target_);
//		trap->inventory.emplace_back(entity_tag::owl, 1);
//		delete_promise_on();
//	}
//	last_action_ = relax;
//}
//
//void owl::jerk(float power, float deceleration, Vector2f destination_point)
//{
//	return;
//}
//
//std::vector<sprite_chain_element*> owl::prepare_sprites(long long elapsed_time)
//{
//	auto body = new sprite_chain_element(pack_tag::owl, pack_part::head, direction::DOWN, 1, position_, conditional_size_units_, texture_box_offset_, color, mirrored_, false);
//	body->z_coordinate = z_coordinate_;
//	animation_speed_ = 15;
//
//	body->mirrored = mirrored_;
//
//	switch (current_action_)
//	{
//	case relax:
//	{
//		mirrored_ = false;
//		body->animation_length = 1;
//		const auto sight_angle = direction_system::calculate_angle(cheat_hero_pos_ - position_);	
//		current_sprite_[0] = int(sight_angle / 22.5f) + 1;		
//	}
//	case head:
//	{
//		body->animation_length = 16;
//		body->pack_part = pack_part::head;
//		break;
//	}
//	case landing:
//	{
//		body->animation_length = 14;
//		body->pack_part = pack_part::landing;
//		break;
//	}
//	case startle:
//	{
//		body->animation_length = 14;
//		body->pack_part = pack_part::startle;
//		break;
//	}
//	case takeoff:
//	{
//		body->animation_length = 10;
//		body->pack_part = pack_part::takeoff;
//		break;
//	}
//	case flap:
//	{
//		body->animation_length = 8;
//		direction_system.set_direction_from_12(move_system.move_offset, elapsed_time);
//		body->direction = direction_system.direction;
//		body->pack_part = pack_part::flap;
//		break;
//	}
//	case soar:
//	{
//		body->animation_length = 7;
//		direction_system.set_direction_from_12(move_system.move_offset, elapsed_time);
//		body->direction = direction_system.direction;
//		body->pack_part = pack_part::soar;
//		break;
//	}
//	case dead:
//	{
//		body->animation_length = 1;
//		current_sprite_[0] = 1;
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
//		if (++current_sprite_[0] > body->animation_length)
//		{
//			last_action_ = current_action_;
//			current_sprite_[0] = 1;
//		}
//	}
//
//	return { body };
//}