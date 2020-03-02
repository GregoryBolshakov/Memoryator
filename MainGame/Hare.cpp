#include "hare.h"

#include "hare_trap.h"

using namespace sf;

hare::hare(const std::string& objectName, Vector2f centerPosition) : neutral_mob(objectName, centerPosition)
{
	conditional_size_units_ = { 180, 150 };
	current_sprite_[0] = 1;
	timeForNewSprite = 0;
	moveSystem.default_speed = 0.0003f;
	moveSystem.speed = 0.0003f;
	radius_ = 70;
	strength = 10;
	sightRange = 720;
	health_point_ = 50;
	currentAction = relax;
	timeAfterHitSelf = 0;
	timeForNewHitSelf = long(6e5);
	timeForNewHit = 1000000;
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

void hare::setTarget(dynamic_object& object)
{
	if (object.tag == entity_tag::noose || currentAction == absorbs)
		return;
	if (helper::getDist(position_, object.get_position()) <= sightRange)
	{
		if (object.tag == entity_tag::hero)
		{
			boundTarget = &object;
			distanceToNearest = helper::getDist(position_, object.get_position());
		}
	}
}

void hare::behaviorWithStatic(world_object* target, long long elapsedTime)
{
	if (currentAction == absorbs)
		return;
	if (helper::getDist(position_, target->get_position()) <= sightRange && timeAfterFear >= fearTime)		
		if (target->tag == entity_tag::hareTrap)
		{
			if (boundTarget == nullptr)
			{
				boundTarget = target;
				distanceToNearest = helper::getDist(position_, target->get_position());
			}
		}
}

void hare::behavior(long long elapsedTime)
{	
	endingPreviousAction();
	fightInteract(elapsedTime);
	if (health_point_ <= 0)
	{
		changeAction(dead, true);
		directionSystem.direction = direction::STAND;
		return;
	}

	directionSystem.calculate_direction();
	if (directionSystem.direction != direction::STAND)
		directionSystem.last_direction = directionSystem.direction;

	// first-priority actions
	if (boundTarget)
	{
		if (boundTarget->tag == entity_tag::hero)
			timeAfterFear = 0;
	}
	else
		timeAfterFear += elapsedTime;

	if (currentAction == absorbs)
	{
		laxMovePosition = { -1, -1 };
		return;
	}
	//-----------------------	

	if (boundTarget == nullptr)
	{
		if (helper::getDist(position_, laxMovePosition) <= radius_)
		{
			changeAction(relax, true, true);
			laxMovePosition = { -1, -1 };
		}
		return;
	}
	
	const float distanceToTarget = helper::getDist(this->position_, boundTarget->get_position());

	// bouncing to a trap
	if (boundTarget->tag == entity_tag::hareTrap)
	{
		directionSystem.side = direction_system::calculate_side(position_, boundTarget->get_position());
		if (helper::getDist(position_, boundTarget->get_position()) <= radius_)
		{
			const auto trap = dynamic_cast<hare_trap*>(boundTarget);
			position_ = trap->getEnterPosition();
			changeAction(absorbs, true, false);
			laxMovePosition = { -1, -1 };
		}
		else
		{
			changeAction(move, false, true);
			laxMovePosition = boundTarget->get_position();
		}
	}
	//-------------------

	// runaway from enemy
	if (boundTarget->tag == entity_tag::hero)
	{
		directionSystem.side = direction_system::calculate_side(position_, laxMovePosition);
		moveSystem.speed = std::max(moveSystem.default_speed, (moveSystem.default_speed * 10) * (1 - (helper::getDist(position_, boundTarget->get_position()) / sightRange * 1.5f)));
		animation_speed_ = std::max(0.0004f, 0.0003f * moveSystem.speed / moveSystem.default_speed);
		if (distanceToTarget <= sightRange)
		{
			changeAction(move, false, true);
			float k = (sightRange/* * 1.5f - Helper::getDist(position, boundTarget->getPosition())*/) / helper::getDist(position_, boundTarget->get_position());
			laxMovePosition = Vector2f(position_.x - (boundTarget->get_position().x - position_.x) * k, position_.y - (boundTarget->get_position().y - position_.y) * k);
		}
		else
		{
			if (currentAction == move)
			{
				if (distanceToTarget >= sightRange * 1.5f)
				{
					changeAction(relax, true, true);
					directionSystem.direction = direction::STAND;
					laxMovePosition = { -1, -1 };
				}
				else
				{
					float k = (sightRange/* * 1.5f - Helper::getDist(position, boundTarget->getPosition())*/) / helper::getDist(position_, boundTarget->get_position());
					laxMovePosition = Vector2f(position_.x - (boundTarget->get_position().x - position_.x) * k, position_.y - (boundTarget->get_position().y - position_.y) * k);
				}
			}
		}
	}
	//-------------------

	if (currentAction != absorbs)
	{
		distanceToNearest = 10e6;
		boundTarget = nullptr;
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
	if (lastAction == commonHit)
		currentAction = relax;
	if (lastAction == absorbs)
	{
		auto trap = dynamic_cast<hare_trap*>(boundTarget);
		trap->inventory[0] = std::make_pair(entity_tag::hare, 1);
		delete_promise_on();
	}
	lastAction = relax;
}

void hare::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

std::vector<sprite_chain_element*> hare::prepare_sprites(long long elapsedTime)
{
	auto body = new sprite_chain_element(pack_tag::hare, pack_part::full, direction::DOWN, 1, position_, conditional_size_units_, texture_box_offset_, color, mirrored_, false);
	animation_speed_ = 10;

	side spriteSide = directionSystem.side;

	if (directionSystem.side == right)
	{
		spriteSide = left;
		body->mirrored = true;
	}
	if (directionSystem.last_direction == direction::RIGHT || directionSystem.last_direction == direction::UPRIGHT || directionSystem.last_direction == direction::DOWNRIGHT)
	{
		body->mirrored = true;
	}

	body->direction = direction_system::side_to_direction(spriteSide);

	switch (currentAction)
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
			lastAction = currentAction;
			current_sprite_[0] = 1;
		}
	}

	return { body };
}
