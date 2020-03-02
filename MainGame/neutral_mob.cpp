#include "neutral_mob.h"

using namespace sf;

neutral_mob::neutral_mob(std::string objectName, Vector2f centerPosition) : DynamicObject(std::move(objectName), centerPosition)
{
	currentSprite[0] = 1;
	currentAction = relax;
}

neutral_mob::~neutral_mob()
= default;

void neutral_mob::setTarget(DynamicObject& object)
{
	/*if (object.tag == Tag::noose)
		return;
	if (Helper::getDist(position, object.getPosition()) < distanceToNearest)
	{
		boundTarget = &object;
		distanceToNearest = Helper::getDist(position, object.getPosition());
	}*/

	if (object.tag == Tag::hero)
		boundTarget = &object;
	}

void neutral_mob::behaviorWithDynamic(DynamicObject* target, long long elapsedTime)
{
	
}

void neutral_mob::behaviorWithStatic(WorldObject* target, long long elapsedTime)
{

}

void neutral_mob::behavior(long long elapsedTime)
{
	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		directionSystem.direction = Direction::STAND;
		return;
	}
	fightInteract(elapsedTime);

	directionSystem.side = DirectionSystem::calculateSide(position, movePosition, elapsedTime);
	//return;
	if (boundTarget == nullptr)
		return;
	const float distanceToTarget = Helper::getDist(this->position, boundTarget->getPosition());

	if (distanceToTarget <= sightRange)
	{
		changeAction(move, false, true);
		movePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x), position.y - (boundTarget->getPosition().y - position.y));
	}
	else
	{
		if (currentAction == move)
		{
			if (distanceToTarget >= sightRange * 1.5)
			{
				changeAction(relax, true, true);
				directionSystem.direction = Direction::STAND;
				movePosition = { -1, -1 };
			}
			else
				movePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x), position.y - (boundTarget->getPosition().y - position.y));
		}
	}

	distanceToNearest = 10e6;
	boundTarget = nullptr;
}

void neutral_mob::fightInteract(long long elapsedTime, DynamicObject* target)
{
	moveSystem.pushAway(elapsedTime);
}