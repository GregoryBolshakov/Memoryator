#include "hare.h"

#include "hare_trap.h"

using namespace sf;

hare::hare(const std::string& objectName, Vector2f centerPosition) : neutral_mob(objectName, centerPosition)
{
	conditional_size_units_ = { 180, 150 };
	current_sprite_[0] = 1;
	timeForNewSprite = 0;
	move_system_.default_speed = 0.0003f;
	move_system_.speed = 0.0003f;
	radius_ = 70;
	strength_ = 10;
	sight_range = 720;
	health_point_ = 50;
	current_action_ = relax;
	time_after_hitself_ = 0;
	time_for_new_hit_self = long(6e5);
	time_for_new_hit = 1000000;
	to_save_name_ = "hare";
	tag = entity_tag::hare;
}

hare::~hare()
= default;

Vector2f hare::calculate_texture_offset()
{
	texture_box_.width = float(texture_box_.width)*get_scale_ratio().x;
	texture_box_.height = float(texture_box_.height)*get_scale_ratio().y;
	return { texture_box_.width / 2, texture_box_.height * 7 / 8 };
}

void hare::set_target(dynamic_object& object)
{
	if (object.tag == entity_tag::noose || current_action_ == absorbs)
		return;
	if (helper::getDist(position_, object.get_position()) <= sight_range)
	{
		if (object.tag == entity_tag::hero)
		{
			bound_target_ = &object;
			distanceToNearest = helper::getDist(position_, object.get_position());
		}
	}
}

void hare::behavior_with_static(world_object* target, long long elapsedTime)
{
	if (current_action_ == absorbs)
		return;
	if (helper::getDist(position_, target->get_position()) <= sight_range && timeAfterFear >= fearTime)		
		if (target->tag == entity_tag::hareTrap)
		{
			if (bound_target_ == nullptr)
			{
				bound_target_ = target;
				distanceToNearest = helper::getDist(position_, target->get_position());
			}
		}
}

void hare::behavior(long long elapsedTime)
{	
	endingPreviousAction();
	fight_interact(elapsedTime);
	if (health_point_ <= 0)
	{
		change_action(dead, true);
		direction_system_.direction = direction::STAND;
		return;
	}

	direction_system_.calculate_direction();
	if (direction_system_.direction != direction::STAND)
		direction_system_.last_direction = direction_system_.direction;

	// first-priority actions
	if (bound_target_)
	{
		if (bound_target_->tag == entity_tag::hero)
			timeAfterFear = 0;
	}
	else
		timeAfterFear += elapsedTime;

	if (current_action_ == absorbs)
	{
		lax_move_position = { -1, -1 };
		return;
	}
	//-----------------------	

	if (bound_target_ == nullptr)
	{
		if (helper::getDist(position_, lax_move_position) <= radius_)
		{
			change_action(relax, true, true);
			lax_move_position = { -1, -1 };
		}
		return;
	}
	
	const float distanceToTarget = helper::getDist(this->position_, bound_target_->get_position());

	// bouncing to a trap
	if (bound_target_->tag == entity_tag::hareTrap)
	{
		direction_system_.side = direction_system::calculate_side(position_, bound_target_->get_position());
		if (helper::getDist(position_, bound_target_->get_position()) <= radius_)
		{
			const auto trap = dynamic_cast<hare_trap*>(bound_target_);
			position_ = trap->getEnterPosition();
			change_action(absorbs, true, false);
			lax_move_position = { -1, -1 };
		}
		else
		{
			change_action(move, false, true);
			lax_move_position = bound_target_->get_position();
		}
	}
	//-------------------

	// runaway from enemy
	if (bound_target_->tag == entity_tag::hero)
	{
		direction_system_.side = direction_system::calculate_side(position_, lax_move_position);
		move_system_.speed = std::max(move_system_.default_speed, (move_system_.default_speed * 10) * (1 - (helper::getDist(position_, bound_target_->get_position()) / sight_range * 1.5f)));
		animation_speed_ = std::max(0.0004f, 0.0003f * move_system_.speed / move_system_.default_speed);
		if (distanceToTarget <= sight_range)
		{
			change_action(move, false, true);
			float k = (sight_range/* * 1.5f - Helper::getDist(position, boundTarget->getPosition())*/) / helper::getDist(position_, bound_target_->get_position());
			lax_move_position = Vector2f(position_.x - (bound_target_->get_position().x - position_.x) * k, position_.y - (bound_target_->get_position().y - position_.y) * k);
		}
		else
		{
			if (current_action_ == move)
			{
				if (distanceToTarget >= sight_range * 1.5f)
				{
					change_action(relax, true, true);
					direction_system_.direction = direction::STAND;
					lax_move_position = { -1, -1 };
				}
				else
				{
					float k = (sight_range/* * 1.5f - Helper::getDist(position, boundTarget->getPosition())*/) / helper::getDist(position_, bound_target_->get_position());
					lax_move_position = Vector2f(position_.x - (bound_target_->get_position().x - position_.x) * k, position_.y - (bound_target_->get_position().y - position_.y) * k);
				}
			}
		}
	}
	//-------------------

	if (current_action_ != absorbs)
	{
		distanceToNearest = 10e6;
		bound_target_ = nullptr;
	}
}

Vector2f hare::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int hare::get_build_type(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void hare::endingPreviousAction()
{
	if (last_action_ == commonHit)
		current_action_ = relax;
	if (last_action_ == absorbs)
	{
		auto trap = dynamic_cast<hare_trap*>(bound_target_);
		trap->inventory[0] = std::make_pair(entity_tag::hare, 1);
		delete_promise_on();
	}
	last_action_ = relax;
}

void hare::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

std::vector<sprite_chain_element*> hare::prepare_sprites(long long elapsedTime)
{
	auto body = new sprite_chain_element(pack_tag::hare, pack_part::full, direction::DOWN, 1, position_, conditional_size_units_, texture_box_offset_, color, mirrored_, false);
	animation_speed_ = 10;

	side spriteSide = direction_system_.side;

	if (direction_system_.side == right)
	{
		spriteSide = left;
		body->mirrored = true;
	}
	if (direction_system_.last_direction == direction::RIGHT || direction_system_.last_direction == direction::UPRIGHT || direction_system_.last_direction == direction::DOWNRIGHT)
	{
		body->mirrored = true;
	}

	body->direction = direction_system::side_to_direction(spriteSide);

	switch (current_action_)
	{
		case relax:
		{
			animationLength = 1;
			body->pack_part = pack_part::stand;
			break;
		}
		case absorbs:
		{
			animationLength = 10;
			body->pack_part = pack_part::trap;
			break;
		}
		case dead:
		{
			animationLength = 1;
			body->pack_part = pack_part::stand;
			current_sprite_[0] = 1;
			break;
		}
		case move:
		{
			animationLength = 5;
			body->pack_part = pack_part::move;						
			break;
		}
	default:;
	}

	body->number = current_sprite_[0];

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= long(1e6 / animation_speed_))
	{
		timeForNewSprite = 0;

		if (++current_sprite_[0] > animationLength)
		{
			last_action_ = current_action_;
			current_sprite_[0] = 1;
		}
	}

	return { body };
}
