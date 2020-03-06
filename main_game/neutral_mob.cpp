#include "neutral_mob.h"

using namespace sf;

neutral_mob::neutral_mob(std::string objectName, Vector2f centerPosition) : dynamic_object(std::move(objectName), centerPosition)
{
	current_sprite_[0] = 1;
	current_action_ = relax;
}

neutral_mob::~neutral_mob()
= default;

void neutral_mob::set_target(dynamic_object& object)
{
	/*if (object.tag == Tag::noose)
		return;
	if (Helper::getDist(position, object.getPosition()) < distanceToNearest)
	{
		boundTarget = &object;
		distanceToNearest = Helper::getDist(position, object.getPosition());
	}*/

	if (object.tag == entity_tag::hero)
		bound_target_ = &object;
	}

void neutral_mob::behavior_with_dynamic(dynamic_object* target, long long elapsedTime)
{
	
}

void neutral_mob::behavior_with_static(world_object* target, long long elapsedTime)
{

}

void neutral_mob::behavior(long long elapsedTime)
{
	if (health_point_ <= 0)
	{
		change_action(dead, true);
		direction_system_.direction = direction::STAND;
		return;
	}
	fight_interact(elapsedTime);

	direction_system_.side = direction_system::calculate_side(position_, move_position_);
	//return;
	if (bound_target_ == nullptr)
		return;
	const float distanceToTarget = helper::getDist(this->position_, bound_target_->get_position());

	if (distanceToTarget <= sight_range)
	{
		change_action(move, false, true);
		move_position_ = Vector2f(position_.x - (bound_target_->get_position().x - position_.x), position_.y - (bound_target_->get_position().y - position_.y));
	}
	else
	{
		if (current_action_ == move)
		{
			if (distanceToTarget >= sight_range * 1.5)
			{
				change_action(relax, true, true);
				direction_system_.direction = direction::STAND;
				move_position_ = { -1, -1 };
			}
			else
				move_position_ = Vector2f(position_.x - (bound_target_->get_position().x - position_.x), position_.y - (bound_target_->get_position().y - position_.y));
		}
	}

	distanceToNearest = 10e6;
	bound_target_ = nullptr;
}

void neutral_mob::fight_interact(long long elapsedTime, dynamic_object* target)
{
	move_system_.push_away(elapsedTime);
}