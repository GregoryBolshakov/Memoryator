#include "monster.h"

using namespace sf;

monster::monster(std::string objectName, const Vector2f centerPosition) : dynamic_object(std::move(objectName), centerPosition)
{
	current_sprite_[0] = 1;
	time_for_new_sprite_ = 0;
	current_action_ = relax;
	direction_system_.side = down;
	sight_range = 950.0f;
	strikingSprite = 6;
	time_for_new_hit = long(1e5);
}

monster::~monster()
= default;

void monster::behavior_with_static(world_object* target, long long elapsedTime)
{

}

void monster::behavior(const long long elapsedTime)
{
	endingPreviousAction();
	fight_interact(elapsedTime);
}

void monster::set_target(dynamic_object& object)
{
	bound_target_ = nullptr;
	if (object.tag == entity_tag::hero)
		return; //targetPosition = object.getPosition();
}

void monster::behavior_with_dynamic(dynamic_object* target, const long long elapsedTime)
{
	if (health_point_ <= 0)
	{
		change_action(dead, true);
		direction_system_.direction = direction::STAND;
		return;
	}

	if (helper::getDist(position_, target->get_position()) <= radius_ + target->get_radius())
		move_system_.push_by_bumping(target->get_position(), target->get_radius(), target->get_move_system().can_crash_into_dynamic);

	if (target->tag != entity_tag::hero)
		return;

	if (helper::getDist(target->get_position(), position_) > sight_range)
	{		
		lax_move_position = { -1, -1 };
		change_action(relax, current_action_ != relax);
		return;
	}

	bound_target_ = target;
	direction_system_.side = direction_system::calculate_side(position_, bound_target_->get_position());

	if (helper::getDist(position_, bound_target_->get_position()) <= sight_range && time_after_hit >= time_for_new_hit)
		move_system_.speed = std::max((1 - helper::getDist(position_, bound_target_->get_position()) / sight_range) * move_system_.default_speed / 2 + move_system_.default_speed, move_system_.default_speed);
	else
		move_system_.speed = move_system_.default_speed;
	if (isAttack.count(current_action_) == 0)
		time_after_hit += elapsedTime;

	// fight with player	
	doAttack(bound_target_);
	if (helper::getDist(position_, bound_target_->get_position()) <= (this->radius_ + bound_target_->get_radius() + hitDistance) &&
		isAttack.count(current_action_) > 0 && current_sprite_[0] == strikingSprite)
	{
		bound_target_->take_damage(this->strength_, position_);
	}

	if (isAttack.count(current_action_) == 0 && helper::getDist(position_, bound_target_->get_position()) <= (this->radius_ + bound_target_->get_radius() + hitDistance / 5))
	{
		stopping(true, false);
		change_action(combatState, false, false);								
	}
	//---------------------
	
	// move to player
	if (helper::getDist(position_, bound_target_->get_position()) > (this->radius_ + bound_target_->get_radius() + hitDistance / 5))
	{
		if (isAttack.count(current_action_) == 0 && current_action_ != combatState)
		{
			change_action(move, false);
			lax_move_position = bound_target_->get_position();
		}
	}
	else	
		stopping(true, false);
}

void monster::endingPreviousAction()
{
	if (last_action_ == combatState)
		change_action(relax, true, false);
	if (last_action_ == commonHit)
		change_action(combatState, true, false);
	if (last_action_ == directHit)
		change_action(combatState, true, false);
	last_action_ = relax;
}

void monster::stopping(const bool doStand, const bool forgetBoundTarget)
{
	if (doStand)
	{
		this->lax_move_position = { -1, -1 };
		move_system_.move_offset = { 0, 0 };
		direction_system_.direction = direction::STAND;
	}

	if (forgetBoundTarget && bound_target_ != nullptr)
	{
		bound_target_->is_processed = false;
		bound_target_ = nullptr;
	}
}

Vector2f monster::get_build_position(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

int monster::get_build_type(Vector2f, Vector2f)
{
	return 1;
}

void monster::jerk(float power, float deceleration, Vector2f destinationPoint)
{
}

void monster::fight_interact(const long long elapsedTime, dynamic_object* target)
{
	time_after_hitself_ += elapsedTime;
	move_system_.push_away(elapsedTime);
}
