//#include "hare.h"
//#include "hare_trap.h"
//
//using namespace sf;
//
//hare::hare(const std::string& objectName, sf::Vector2f centerPosition) : neutral_mob(objectName, centerPosition)
//{
//	conditional_size_units_ = { 240, 200 };
//	move_system.default_speed = 0.0006f;
//	move_system.speed = 0.0006f;
//	radius_ = 70;
//	sight_range = 720;
//	run_away_range_ = 1300;
//	health_point_ = 50;
//	time_for_new_hit_self = long(6e5);
//	to_save_name_ = "hare";
//	tag = entity_tag::hare;
//}
//
//hare::~hare()
//= default;
//
//Vector2f hare::calculate_texture_offset()
//{
//	texture_box_.width = float(texture_box_.width) * get_scale_ratio().x;
//	texture_box_.height = float(texture_box_.height) * get_scale_ratio().y;
//	return { texture_box_.width / 2, texture_box_.height * 7 / 8 };
//}
//
//void hare::behavior_with_static(world_object* target, long long elapsedTime)
//{
//	const std::map<action, bool> actions_that_ignore_target_selection = { {trap, true}, {dead, true} };
//	if (actions_that_ignore_target_selection.count(current_action_))
//		return;
//	
//	const auto distance_to_target = helper::getDist(position_, target->get_position());
//	
//	if (target->tag == entity_tag::hareTrap && bound_target_ == nullptr &&
//		distance_to_target <= sight_range && time_after_fear_ >= fear_time_)
//		bound_target_ = target;
//}
//
//void hare::behavior_with_dynamic(dynamic_object* target, long long elapsedTime)
//{
//	if (target->tag == entity_tag::hero)
//		cheat_hero_pos_ = target->get_position();
//	
//	const std::map<action, bool> actions_that_ignore_target_selection = { {trap, true}, {dead, true} };
//	if (actions_that_ignore_target_selection.count(current_action_))
//		return;
//	
//	bound_target_ = nullptr; // earlier, than behaviour with static
//	
//	const auto distance_to_target = helper::getDist(position_, target->get_position());
//
//	if (distance_to_target <= sight_range || distance_to_target <= run_away_range_ && time_after_fear_ <= fear_time_ && target->tag == entity_tag::hero)
//		bound_target_ = target;
//}
//
//void hare::behavior(const long long elapsed_time)
//{
//	ending_previous_action();
//	fight_interact(elapsed_time);
//	
//	if (die_check())
//		return;
//
//	if (direction_system.direction != direction::STAND)
//		direction_system.last_direction = direction_system.direction;
//
//	if (run_away_from_enemy(elapsed_time))
//		return;
//
//	if (bounce_to_trap(elapsed_time))
//		return;
//
//	calm_behavior(elapsed_time);
//}
//
//bool hare::run_away_from_enemy(long long elapsed_time)
//{
//	if (bound_target_ && bound_target_->tag == entity_tag::hero)
//	{
//		const auto distance_to_target = helper::getDist(this->position_, bound_target_->get_position());
//		if (distance_to_target <= sight_range || distance_to_target <= run_away_range_ && time_after_fear_ <= fear_time_)
//		{
//			time_after_fear_ = 0;
//			calm_state_ = false;
//			change_action(move, current_action_ != move);
//			const auto k = run_away_range_ / distance_to_target;
//			move_system.speed = move_system.default_speed;
//			move_system.lax_move_position = sf::Vector2f(position_.x - (bound_target_->get_position().x - position_.x) * k,
//				position_.y - (bound_target_->get_position().y - position_.y) * k);
//			return true;
//		}
//	}
//
//	time_after_fear_ += elapsed_time;
//	return false;
//}
//
//bool hare::bounce_to_trap(long long elapsed_time)
//{
//	if (bound_target_ && bound_target_->tag == entity_tag::hareTrap)
//	{
//		const auto trap = dynamic_cast<hare_trap*>(bound_target_);
//		const auto minimal_contact_distance = (micro_block_size.x + micro_block_size.y) / 2;
//		
//		if (helper::getDist(position_, trap->get_enter_position()) <= minimal_contact_distance)
//		{
//			position_ = trap->get_enter_position();
//			change_action(action::trap, current_action_ != action::trap);
//			move_system.lax_move_position = { -1, -1 };
//		}
//
//		if (current_action_ == relax)
//		{
//			change_action(move, true);
//			move_system.speed = move_system.default_speed / 1.5f;
//			move_system.lax_move_position = trap->get_enter_position();
//		}
//		return true;
//	}
//
//	return false;
//}
//
//void hare::calm_behavior(const long long elapsed_time)
//{
//	if (time_after_fear_ >= fear_time_)
//	{
//		calm_state_ = true;
//		if (current_action_ == move)
//			change_action(relax, true);
//		direction_system.direction = direction::STAND;
//		move_system.lax_move_position = { -1, -1 };
//	}
//	else
//		return;
//
//	time_after_calm_state_ += elapsed_time;
//	if (time_after_calm_state_ < time_for_new_calm_state_ || current_action_ != relax)
//		return;
//
//	time_after_calm_state_ = rand() % (time_for_new_calm_state_ / 2);
//	
//	std::vector<std::pair<action, int>> action = {};
//
//	while (true)
//	{
//		if (stand_)
//			action = { {transition, 20}, {move, 35}, {jump, 15}, {look_around, 15}, {startle, 15} };
//		else
//			action = { {transition, 40}, {listening, 30}, {sniff, 30} };
//
//		auto action_discriminant = rand() % 100 + 1;
//
//		for (const auto action : action)
//		{
//			if (action.second >= action_discriminant)
//			{
//				change_action(action.first, true);
//				break;
//			}
//			action_discriminant -= action.second;
//		}
//
//		if (!calm_actions_stack.empty() && calm_actions_stack[calm_actions_stack.size() - 1] == transition && current_action_ == transition)
//			continue;
//		
//		// action 3 times repeat check
//		if (calm_actions_stack.size() < 2 || !(calm_actions_stack[0] == current_action_ && calm_actions_stack[1] == current_action_))
//		{
//			if (calm_actions_stack.size() >= 2)
//				calm_actions_stack.erase(calm_actions_stack.begin());
//			break;
//		}
//		//----------------------------
//	}
//
//	if (current_action_ == move)
//	{
//		move_system.speed = move_system.default_speed / 2;
//		
//		auto move_vector = position_ - cheat_hero_pos_;
//		move_vector.x = move_vector.x / (abs(move_vector.x) + 1) * float(rand() % long(sight_range) + 1);
//		move_vector.y = move_vector.y / (abs(move_vector.y) + 1) * float(rand() % long(sight_range) + 1);
//		
//		move_system.lax_move_position = position_ + move_vector;
//		time_after_fear_ = 0;
//	}
//}
//
//Vector2f hare::get_build_position(std::vector<world_object*> visible_items, float scale_factor, sf::Vector2f camera_position)
//{
//	return { -1, -1 };
//}
//
//int hare::get_build_type(sf::Vector2f oun_pos, sf::Vector2f other_pos)
//{
//	return 1;
//}
//
//void hare::ending_previous_action()
//{
//	if (last_action_ == common_hit || last_action_ == move)
//		current_action_ = relax;
//	if (last_action_ == trap)
//	{
//		auto trap = dynamic_cast<hare_trap*>(bound_target_);
//		trap->inventory[0] = std::make_pair(entity_tag::hare, 1);
//		delete_promise_on();
//	}
//	if (last_action_ == dead)
//		delete_promise_on();
//	if (last_action_ == transition)
//	{
//		stand_ = !stand_;
//		current_action_ = relax;
//		time_after_calm_state_ = 0;
//		calm_actions_stack.push_back(last_action_);
//	}
//	if (last_action_ == jump)
//		change_action(jump_end, true);
//	if (last_action_ == jump_end || last_action_ == look_around || last_action_ == startle || last_action_ == listening || last_action_ == sniff)
//	{
//		if (current_action_ == jump_end)
//			calm_actions_stack.push_back(jump);
//		else
//			calm_actions_stack.push_back(last_action_);
//		current_action_ = relax;
//		time_after_calm_state_ = 0;
//	}	
//	
//	last_action_ = relax;
//}
//
//void hare::jerk(float power, float deceleration, sf::Vector2f destination_point)
//{
//	return;
//}
//
//std::vector<sprite_chain_element*> hare::prepare_sprites(long long elapsedTime)
//{
//	auto body = new sprite_chain_element(pack_tag::hare, pack_part::full, direction::DOWN, 1, position_, conditional_size_units_, texture_box_offset_, color, mirrored_, false);
//	animation_speed_ = 12;
//	
//	auto inverse = false;
//	body->mirrored = mirrored_;
//	
//	switch (current_action_)
//	{
//		case relax:
//		{
//			body->animation_length = 1;
//			current_sprite_[0] = 1;
//			if (stand_)
//				body->pack_part = pack_part::jump;
//			else
//				body->pack_part = pack_part::listening;
//			break;
//		}
//		case jump:
//		{
//			body->animation_length = 17;
//			body->pack_part = pack_part::jump;
//			break;
//		}
//		case jump_end:
//		{
//			inverse = true;
//			body->animation_length = 17;
//			body->pack_part = pack_part::jump;
//			break;
//		}
//		case listening:
//		{
//			body->animation_length = 14;
//			body->pack_part = pack_part::listening;
//			break;
//		}
//		case look_around:
//		{
//			body->animation_length = 17;
//			body->pack_part = pack_part::look_around;
//			break;
//		}
//		case sniff:
//		{
//			body->animation_length = 14;
//			body->pack_part = pack_part::sniff;
//			break;
//		}
//		case startle:
//		{
//			body->animation_length = 14;
//			body->pack_part = pack_part::startle;
//			break;
//		}
//		case transition:
//		{
//			if (!stand_)
//				inverse = true;
//			body->animation_length = 3;
//			body->pack_part = pack_part::transition;
//			break;
//		}
//		case trap:
//		{
//			body->mirrored = false;				
//			body->animation_length = 12;
//			body->pack_part = pack_part::trap;
//			break;
//		}
//		case dead:
//		{
//			body->animation_length = 1;
//			current_sprite_[0] = 1;
//			body->pack_part = pack_part::listening;
//			break;
//		}
//		case move:
//		{
//			body->animation_length = 6;			
//			direction_system.set_direction_from_8(move_system.move_offset, elapsedTime);
//			body->direction = direction_system.direction;
//			body->pack_part = pack_part::move;
//			break;
//		}
//	default:;
//	}
//
//	if (inverse)
//		body->number = body->animation_length + 1 - current_sprite_[0];
//	else
//		body->number = current_sprite_[0];
//
//	time_for_new_sprite_ += elapsedTime;
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
