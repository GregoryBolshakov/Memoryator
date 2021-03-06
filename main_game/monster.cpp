//#include "monster.h"
//
//using namespace sf;
//
//monster::monster(std::string objectName, const sf::Vector2f centerPosition) : dynamic_object(std::move(objectName), centerPosition)
//{
//	current_sprite_[0] = 1;
//	time_for_new_sprite_ = 0;
//	current_action_ = relax;
//	direction_system.side = down;
//	sight_range = 950.0f;
//	strikingSprite = 6;
//	time_for_new_hit = long(1e5);
//}
//
//monster::~monster()
//= default;
//
//void monster::behavior_with_static(world_object* target, long long elapsedTime)
//{
//
//}
//
//void monster::behavior(const long long elapsedTime)
//{
//	endingPreviousAction();
//	fight_interact(elapsedTime);
//}
//
//void monster::set_target(dynamic_object& object)
//{
//	bound_target_ = nullptr;
//	if (object.tag == entity_tag::hero)
//		return; //targetPosition = object.getPosition();
//}
//
//void monster::behavior_with_dynamic(dynamic_object* target, const long long elapsedTime)
//{
//	if (health_ <= 0)
//	{
//		change_action(dead, true);
//		direction_system.direction = direction::STAND;
//		return;
//	}
//
//	if (helper::getDist(position_, target->get_position()) <= radius_ + target->get_radius())
//		move_system.push_by_bumping(target->get_position(), target->get_radius(), target->move_system.can_crash_into_dynamic);
//
//	if (target->tag != entity_tag::hero)
//		return;
//
//	if (helper::getDist(target->get_position(), position_) > sight_range)
//	{		
//		move_system.lax_move_position = { -1, -1 };
//		change_action(relax, current_action_ != relax);
//		return;
//	}
//
//	bound_target_ = target;
//	direction_system.side = direction_system.calculate_side(position_, bound_target_->get_position(), elapsedTime);
//
//	if (helper::getDist(position_, bound_target_->get_position()) <= sight_range && time_after_hit >= time_for_new_hit)
//		move_system.speed = std::max((1 - helper::getDist(position_, bound_target_->get_position()) / sight_range) * move_system.default_speed / 2 + move_system.default_speed, move_system.default_speed);
//	else
//		move_system.speed = move_system.default_speed;
//	if (isAttack.count(current_action_) == 0)
//		time_after_hit += elapsedTime;
//
//	// fight with player	
//	doAttack(bound_target_);
//	if (helper::getDist(position_, bound_target_->get_position()) <= (this->radius_ + bound_target_->get_radius() + hitDistance) &&
//		isAttack.count(current_action_) > 0 && current_sprite_[0] == strikingSprite)
//	{
//		bound_target_->take_damage(this->strength_, position_);
//	}
//
//	if (isAttack.count(current_action_) == 0 && helper::getDist(position_, bound_target_->get_position()) <= (this->radius_ + bound_target_->get_radius() + hitDistance / 5))
//	{
//		stopping(true, false);
//		change_action(combat_state, false, false);								
//	}
//	//---------------------
//	
//	// move to player
//	if (helper::getDist(position_, bound_target_->get_position()) > (this->radius_ + bound_target_->get_radius() + hitDistance / 5))
//	{
//		if (isAttack.count(current_action_) == 0 && current_action_ != combat_state)
//		{
//			change_action(move, false);
//			move_system.lax_move_position = bound_target_->get_position();
//		}
//	}
//	else	
//		stopping(true, false);
//}
//
//void monster::endingPreviousAction()
//{
//	if (last_action_ == combat_state)
//		change_action(relax, true, false);
//	if (last_action_ == common_hit)
//		change_action(combat_state, true, false);
//	if (last_action_ == direct_hit)
//		change_action(combat_state, true, false);
//	last_action_ = relax;
//}
//
//void monster::stopping(const bool doStand, const bool forgetBoundTarget)
//{
//	if (doStand)
//	{
//		move_system.lax_move_position = { -1, -1 };
//		move_system.move_offset = { 0, 0 };
//		direction_system.direction = direction::STAND;
//	}
//
//	if (forgetBoundTarget && bound_target_ != nullptr)
//	{
//		bound_target_->is_processed = false;
//		bound_target_ = nullptr;
//	}
//}
//
//Vector2f monster::get_build_position(std::vector<world_object*>, float, sf::Vector2f)
//{
//	return { -1, -1 };
//}
//
//int monster::get_build_type(sf::Vector2f, sf::Vector2f)
//{
//	return 1;
//}
//
//void monster::jerk(float power, float deceleration, sf::Vector2f destinationPoint)
//{
//}
//
//void monster::fight_interact(const long long elapsedTime, dynamic_object* target)
//{
//	time_after_hitself_ += elapsedTime;
//	move_system.push_away(elapsedTime);
//}
