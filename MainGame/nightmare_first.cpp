#include "nightmare_first.h"

nightmare_first::nightmare_first(std::string objectName, const Vector2f centerPosition) : monster(std::move(objectName), centerPosition)
{
	conditional_size_units_ = { 375, 375 };
	moveSystem.defaultSpeed = 0.0005f;
	moveSystem.speed = 0.0005f;
	radius_ = 90;
	hitDistance = 70;
	strength = 10;
	health_point_ = 100;
	currentAction = relax;	
	timeForNewHitSelf = long(2e5);
	timeAfterHitSelf = timeForNewHitSelf;
	timeForNewHit = long(1e6);
	timeAfterHit = timeForNewHit;
	to_save_name_ = "nightmare1_";
	tag = entity_tag::nightmare1;
}


nightmare_first::~nightmare_first()
= default;


Vector2f nightmare_first::calculate_texture_offset()
{	
	init_micro_blocks();
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return { texture_box_.width / 2, texture_box_.height * 7 / 8 };
}

void nightmare_first::doAttack(world_object* target)
{
	if (timeAfterHit >= timeForNewHit)
	{
		if (helper::getDist(position_, boundTarget->get_position()) <= (this->radius_ + boundTarget->get_radius() + hitDistance / 5))
		{
			changeAction(commonHit, true, false);
			timeAfterHit = 0;
		}
	}
}

std::vector<sprite_chain_element*> nightmare_first::prepare_sprites(const long long elapsedTime)
{
	auto body = new sprite_chain_element(pack_tag::nightmare1, pack_part::stand, direction::DOWN, 1, position_, conditional_size_units_, texture_box_offset_, color, mirrored_, false);
	animation_speed_ = 10;

	side spriteSide = directionSystem.side;
	direction spriteDirection = directionSystem.last_direction;

	if (directionSystem.side == right)
	{
		spriteSide = left;
		body->mirrored = true;
	}
	if (directionSystem.last_direction == direction::RIGHT || directionSystem.last_direction == direction::UPRIGHT || directionSystem.last_direction == direction::DOWNRIGHT)
	{
		spriteDirection = direction_system::cut_rights(spriteDirection);
		body->mirrored = true;
	}

	body->direction = direction_system::side_to_direction(spriteSide);

	switch (currentAction)
	{
	case commonHit:
	{
		animationLength = 9;
		body->pack_tag = pack_tag::nightmare1;
		body->pack_part = pack_part::hit;		
		break;
	}
	case combatState:
	case relax:
	{
		animationLength = 13;
		body->pack_tag = pack_tag::nightmare1;
		body->pack_part = pack_part::stand;
		break;
	}
	case dead:
	{
		animationLength = 1;
		body->pack_tag = pack_tag::nightmare1;
		body->pack_part = pack_part::stand;
		current_sprite_[0] = 1;
		delete_promise_on();
		break;
	}
	case move:
	{
		animationLength = 8;
		body->pack_tag = pack_tag::nightmare1;
		body->pack_part = pack_part::move;
		break;
	}
	default:;
	}

	body->number = current_sprite_[0];

	time_for_new_sprite_ += elapsedTime;

	if (time_for_new_sprite_ >= long(1e6 / animation_speed_))
	{
		time_for_new_sprite_ = 0;

		if (++current_sprite_[0] > animationLength)
		{
			lastAction = currentAction;
			current_sprite_[0] = 1;
		}
	}

	return { body };	
}

