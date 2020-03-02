#include "deer.h"

using namespace sf;

deer::deer(const std::string& objectName, const Vector2f centerPosition) : neutral_mob(objectName, centerPosition)
{
	conditional_size_units_ = {360, 300};
	current_sprite_[0] = 1;
	timeForNewSprite = 0;
	moveSystem.defaultSpeed = 0.00085F;
	//defaultSpeed = 0.0002f;
	moveSystem.speed = moveSystem.defaultSpeed;
	animation_speed_ = 10;
	animationLength = 8;
	radius_ = 70;
	strength = 10;
	sightRange = conditional_size_units_.x * 3;
	morality = 5; // from 1 to 10
	fear = 0;
	health_point_ = 50;
	currentAction = relax;
	timeForNewHitSelf = long(6e5);
	timeAfterHitSelf = timeForNewHitSelf;
	timeForNewHit = 10e6;
	to_save_name_ = "deer";
	tag = entity_tag::deer;
}

deer::~deer()
= default;

Vector2f deer::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return {texture_box_.width / 2, texture_box_.height * 7 / 8};
}

void deer::behaviorWithStatic(world_object* target, long long elapsedTime)
{
}

void deer::behavior(const long long elapsedTime)
{
	endingPreviousAction();
	directionSystem.calculate_direction();
	fightInteract(elapsedTime);

	if (health_point_ <= 0)
	{
		changeAction(dead, true);
		directionSystem.direction = direction::STAND;
		return;
	}

	if (this->owner != nullptr)
	{
		moveSystem.speed = moveSystem.defaultSpeed;
		if (currentAction == commonHit)
		{
			laxMovePosition = {-1, -1};
			return;
		}
		directionSystem.side = direction_system::calculate_side(position_, owner->get_position());
		if (helper::getDist(position_, owner->get_position()) > sightRange / 2)
		{
			changeAction(moveSlowly, false, false);
			laxMovePosition = owner->get_position();
		}
		else if (helper::getDist(position_, owner->get_position()) <= sightRange / 2.5 && currentAction != relax)
		{
			changeAction(relax, true, false);
			laxMovePosition = {-1, -1};
		}
		return;
	}

	directionSystem.side = direction_system::calculate_side(position_, laxMovePosition);

	if (boundTarget == nullptr)
	{
		return;
	}

	const auto distanceToTarget = helper::getDist(this->position_, boundTarget->get_position());
	//speed = std::max(defaultSpeed, (defaultSpeed * 10) * (1 - (distanceToTarget) / sightRange * 1.5f));
	animation_speed_ = std::max(0.0004F, 0.0003F * moveSystem.speed / moveSystem.defaultSpeed);

	if (distanceToTarget <= sightRange)
	{
		changeAction(move, false, true);
		laxMovePosition = Vector2f(position_.x - (boundTarget->get_position().x - position_.x), position_.y - (boundTarget->get_position().y - position_.y));
	}
	else
	{
		if (currentAction == move)
		{
			if (distanceToTarget >= sightRange * 1.5)
			{
				changeAction(relax, true, true);
				directionSystem.direction = direction::STAND;
				laxMovePosition = {-1, -1};
			}
			else
			{
				laxMovePosition = Vector2f(position_.x - (boundTarget->get_position().x - position_.x), position_.y - (boundTarget->get_position().y - position_.y));
			}
		}
	}
	distanceToNearest = 10e6;
	boundTarget = nullptr;
}

Vector2f deer::get_build_position(std::vector<world_object*> /*visibleItems*/, float /*scaleFactor*/, Vector2f /*cameraPosition*/)
{
	return {-1, -1};
}

int deer::get_build_type(Vector2f /*ounPos*/, Vector2f /*otherPos*/)
{
	return 1;
}

void deer::endingPreviousAction()
{
	if (lastAction == commonHit)
	{
		currentAction = relax;
	}

	lastAction = relax;
}

void deer::jerk(float /*power*/, float /*deceleration*/, Vector2f /*destinationPoint*/)
{
}

Vector2f deer::getHeadPosition()
{
	const auto upperLeft = Vector2f(position_.x - texture_box_offset_.x, position_.y - texture_box_offset_.y);

	if (directionSystem.lastDirection == direction::UP)
	{
		if (current_sprite_[0] == 1)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.569F, upperLeft.y + conditional_size_units_.y * 0.111F};
		}
		if (current_sprite_[0] == 2)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.575F, upperLeft.y + conditional_size_units_.y * 0.155F};
		}
		if (current_sprite_[0] == 3)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.577F, upperLeft.y + conditional_size_units_.y * 0.202F};
		}
		if (current_sprite_[0] == 4)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.580F, upperLeft.y + conditional_size_units_.y * 0.150F};
		}
		if (current_sprite_[0] == 5)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.570F, upperLeft.y + conditional_size_units_.y * 0.106F};
		}
		if (current_sprite_[0] == 6)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.570F, upperLeft.y + conditional_size_units_.y * 0.093F};
		}
		if (current_sprite_[0] == 7)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.575F, upperLeft.y + conditional_size_units_.y * 0.075F};
		}
	}
	if (directionSystem.lastDirection == direction::DOWN)
	{
		if (current_sprite_[0] == 1)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.446F, upperLeft.y + conditional_size_units_.y * 0.222F};
		}
		if (current_sprite_[0] == 2)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.437F, upperLeft.y + conditional_size_units_.y * 0.243F};
		}
		if (current_sprite_[0] == 3)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.437F, upperLeft.y + conditional_size_units_.y * 0.258F};
		}
		if (current_sprite_[0] == 4)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.437F, upperLeft.y + conditional_size_units_.y * 0.242F};
		}
		if (current_sprite_[0] == 5)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.440F, upperLeft.y + conditional_size_units_.y * 0.168F};
		}
		if (current_sprite_[0] == 6)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.444F, upperLeft.y + conditional_size_units_.y * 0.150F};
		}
		if (current_sprite_[0] == 7)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.445F, upperLeft.y + conditional_size_units_.y * 0.182F};
		}
	}
	if (direction_system::cut_diagonals(directionSystem.lastDirection) == direction::LEFT)
	{
		if (current_sprite_[0] == 1)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.313F, upperLeft.y + conditional_size_units_.y * 0.147F};
		}
		if (current_sprite_[0] == 2)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.325F, upperLeft.y + conditional_size_units_.y * 0.159F};
		}
		if (current_sprite_[0] == 3)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.338F, upperLeft.y + conditional_size_units_.y * 0.169F};
		}
		if (current_sprite_[0] == 4)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.319F, upperLeft.y + conditional_size_units_.y * 0.138F};
		}
		if (current_sprite_[0] == 5)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.287F, upperLeft.y + conditional_size_units_.y * 0.123F};
		}
		if (current_sprite_[0] == 6)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.282F, upperLeft.y + conditional_size_units_.y * 0.130F};
		}
		if (current_sprite_[0] == 7)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.277F, upperLeft.y + conditional_size_units_.y * 0.138F};
		}
	}
	if (direction_system::cut_diagonals(directionSystem.lastDirection) == direction::RIGHT)
	{
		if (current_sprite_[0] == 1)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.6F, upperLeft.y + conditional_size_units_.y * 0.17F};
		}
		if (current_sprite_[0] == 2)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.59F, upperLeft.y + conditional_size_units_.y * 0.19F};
		}
		if (current_sprite_[0] == 3)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.56F, upperLeft.y + conditional_size_units_.y * 0.2F};
		}
		if (current_sprite_[0] == 4)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.59F, upperLeft.y + conditional_size_units_.y * 0.17F};
		}
		if (current_sprite_[0] == 5)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.62F, upperLeft.y + conditional_size_units_.y * 0.16F};
		}
		if (current_sprite_[0] == 6)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.63F, upperLeft.y + conditional_size_units_.y * 0.16F};
		}
		if (current_sprite_[0] == 7)
		{
			return {upperLeft.x + conditional_size_units_.x * 0.63F, upperLeft.y + conditional_size_units_.y * 0.17F};
		}
	}
	return position_;
}

std::vector<sprite_chain_element*> deer::prepare_sprites(long long elapsedTime)
{
	auto body = new sprite_chain_element(pack_tag::deer, pack_part::stand, direction::DOWN, 1, position_, conditional_size_units_, texture_box_offset_, color, mirrored_, false);
	animation_speed_ = 10;

	auto spriteDirection = direction_system::cut_diagonals(directionSystem.lastDirection);

	if (directionSystem.side == right)
	{
		body->mirrored = true;
	}
	if (directionSystem.lastDirection == direction::RIGHT || directionSystem.lastDirection == direction::UPRIGHT || directionSystem.lastDirection == direction::DOWNRIGHT)
	{
		spriteDirection = direction_system::cut_rights(spriteDirection);
		body->mirrored = true;
	}

	body->direction = spriteDirection;

	switch (currentAction)
	{
	case relax:
		{
			animationLength = 1;
			break;
		}
	case commonHit:
		{
			animationLength = 6;
			body->pack_part = pack_part::hunt;
			break;
		}
	case dead:
		{
			animationLength = 1;
			body->direction = direction::DOWN;
			current_sprite_[0] = 1;
			break;
		}
	case move:
	case moveSlowly:
		{
			animationLength = 7;
			body->pack_part = pack_part::move;
			break;
		}
	default: ;
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

	return {body};
}
