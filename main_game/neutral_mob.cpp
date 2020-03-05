#include "neutral_mob.h"

using namespace sf;

neutral_mob::neutral_mob(std::string objectName, Vector2f centerPosition) : dynamic_object(std::move(objectName), centerPosition)
{
	current_sprite_[0] = 1;
	currentAction = relax;
}

neutral_mob::~neutral_mob()
= default;

void neutral_mob::setTarget(dynamic_object& object)
{
	/*if (object.tag == Tag::noose)
		return;
	if (Helper::getDist(position, object.getPosition()) < distanceToNearest)
	{
		boundTarget = &object;
		distanceToNearest = Helper::getDist(position, object.getPosition());
	}*/

	if (object.tag == entity_tag::hero)
		boundTarget = &object;
	}

void neutral_mob::behaviorWithDynamic(dynamic_object* target, long long elapsedTime)
{
	
}

void neutral_mob::behaviorWithStatic(world_object* target, long long elapsedTime)
{

}

void neutral_mob::behavior(long long elapsedTime)
{
	if (health_point_ <= 0)
	{
		changeAction(dead, true);
		directionSystem.direction = direction::STAND;
		return;
	}
	fightInteract(elapsedTime);

	directionSystem.side = direction_system::calculate_side(position_, movePosition);
	//return;
	if (boundTarget == nullptr)
		return;
	const float distanceToTarget = helper::getDist(this->position_, boundTarget->get_position());

	if (distanceToTarget <= sightRange)
	{
		changeAction(move, false, true);
		movePosition = Vector2f(position_.x - (boundTarget->get_position().x - position_.x), position_.y - (boundTarget->get_position().y - position_.y));
	}
	else
	{
		if (currentAction == move)
		{
			if (distanceToTarget >= sightRange * 1.5)
			{
				changeAction(relax, true, true);
				directionSystem.direction = direction::STAND;
				movePosition = { -1, -1 };
			}
			else
				movePosition = Vector2f(position_.x - (boundTarget->get_position().x - position_.x), position_.y - (boundTarget->get_position().y - position_.y));
		}
	}

	distanceToNearest = 10e6;
	boundTarget = nullptr;
}

void neutral_mob::fightInteract(long long elapsedTime, dynamic_object* target)
{
	moveSystem.push_away(elapsedTime);
}