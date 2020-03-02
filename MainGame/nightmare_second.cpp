#include "nightmare_second.h"

nightmare_second::nightmare_second(std::string objectName, Vector2f centerPosition) : monster(std::move(objectName), centerPosition)
{
	conditional_size_units_ = { 600, 600 };
	moveSystem.defaultSpeed = 0.0005f;
	moveSystem.speed = 0.0005f;
	radius_ = 120;
	hitDistance = 120;
	strength = 17;
	health_point_ = 180;
	currentAction = relax;
	timeForNewHitSelf = long(2e5);
	timeAfterHitSelf = timeForNewHitSelf;
	timeForNewHit = long(1e6);
	timeAfterHit = timeForNewHit;
	to_save_name_ = "nightmare2_";
	tag = entity_tag::nightmare2;
}


nightmare_second::~nightmare_second()
= default;


Vector2f nightmare_second::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return { texture_box_.width / 2, texture_box_.height * 6 / 8 };
}

void nightmare_second::doAttack(world_object* target)
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

std::vector<sprite_chain_element*> nightmare_second::prepare_sprites(long long elapsedTime)
{
	auto body = new sprite_chain_element(pack_tag::nightmare2Stand, pack_part::full, direction::DOWN, 1, position_, conditional_size_units_, texture_box_offset_, color, mirrored_, false);
	shakeSpeed = -1;
	animation_speed_ = 10;

	side spriteSide = directionSystem.side;
	direction spriteDirection = directionSystem.lastDirection;

	if (directionSystem.side == right)
	{
		spriteSide = left;
		body->mirrored = true;
	}
	if (directionSystem.lastDirection == direction::RIGHT || directionSystem.lastDirection == direction::UPRIGHT || directionSystem.lastDirection == direction::DOWNRIGHT)
	{
		spriteDirection = direction_system::cut_rights(spriteDirection);
		body->mirrored = true;
	}

	body->direction = direction_system::side_to_direction(spriteSide);

	switch (currentAction)
	{
	case commonHit:
	{
		animationLength = 12;
		body->pack_tag = pack_tag::nightmare2Hit;
		break;
	}
	case combatState:
	{
		animationLength = 9;
		body->pack_tag = pack_tag::nightmare2Stand;
		break;
	}
	case relax:
	{
		animationLength = 9;
		body->pack_tag = pack_tag::nightmare2Stand;
		break;
	}
	case dead:
	{
		animationLength = 1;
		body->pack_tag = pack_tag::nightmare2Stand;
		current_sprite_[0] = 1;
		delete_promise_on();
		break;
	}
	case move:
	{
		animationLength = 8;
		body->pack_tag = pack_tag::nightmare2Move;
		if (current_sprite_[0] == 2 || current_sprite_[0] == 6)
			shakeSpeed = 0;
		break;
	}
	default:;
	}

	body->number = current_sprite_[0];

	time_for_new_sprite_ += elapsedTime;

	if (time_for_new_sprite_ >= 1e6 / animation_speed_)
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

