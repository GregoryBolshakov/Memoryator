#include "Monster.h"

using namespace sf;

Monster::Monster(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	currentAction = relax;
	directionSystem.side = down;
	sightRange = 950;
	strikingSprite = 6;
	timeForNewHit = 1e5;
}

Monster::~Monster()
{

}

void Monster::behaviorWithStatic(WorldObject* target, long long elapsedTime)
{

}

void Monster::behavior(long long elapsedTime)
{
	endingPreviousAction();
	fightInteract(elapsedTime);
}

void Monster::setTarget(DynamicObject& object)
{
	boundTarget = nullptr;
	if (object.tag == Tag::hero)
		return; //targetPosition = object.getPosition();
}

void Monster::behaviorWithDynamic(DynamicObject* target, long long elapsedTime)
{
	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		directionSystem.direction = Direction::STAND;
		return;
	}

	if (Helper::getDist(position, target->getPosition()) <= radius + target->getRadius())
		moveSystem.pushByBumping(target->getPosition(), target->getRadius(), target->getMoveSystem().canCrashIntoDynamic);

	if (target->tag != Tag::hero)
		return;

	if (Helper::getDist(target->getPosition(), position) > sightRange)
	{		
		laxMovePosition = { -1, -1 };
		changeAction(relax, currentAction != relax);
		return;
	}

	boundTarget = target;
	directionSystem.side = DirectionSystem::calculateSide(position, boundTarget->getPosition(), elapsedTime);

	if (Helper::getDist(position, boundTarget->getPosition()) <= sightRange && timeAfterHit >= timeForNewHit)
		moveSystem.speed = std::max((1 - Helper::getDist(position, boundTarget->getPosition()) / sightRange) * moveSystem.defaultSpeed / 2 + moveSystem.defaultSpeed, moveSystem.defaultSpeed);
	else
		moveSystem.speed = moveSystem.defaultSpeed;
	if (isAttack.count(currentAction) == 0)
		timeAfterHit += elapsedTime;

	// fight with player	
	doAttack(boundTarget);
	if (Helper::getDist(position, boundTarget->getPosition()) <= (this->radius + boundTarget->getRadius() + hitDistance) &&
		isAttack.count(currentAction) > 0 && currentSprite[0] == strikingSprite)
	{
		boundTarget->takeDamage(this->strength, position);
	}

	if (isAttack.count(currentAction) == 0 && Helper::getDist(position, boundTarget->getPosition()) <= (this->radius + boundTarget->getRadius() + hitDistance / 5))
	{
		stopping(true, false);
		changeAction(combatState, false, false);								
	}
	//---------------------
	
	// move to player
	if (Helper::getDist(position, boundTarget->getPosition()) > (this->radius + boundTarget->getRadius() + hitDistance / 5))
	{
		if (isAttack.count(currentAction) == 0 && currentAction != combatState)
		{
			changeAction(move, false);
			laxMovePosition = boundTarget->getPosition();
		}
	}
	else	
		stopping(true, false);
	//---------------
}

void Monster::endingPreviousAction()
{
	if (lastAction == combatState)
		changeAction(relax, true, false);
	if (lastAction == commonHit)
		changeAction(combatState, true, false);
	if (lastAction == directHit)
		changeAction(combatState, true, false);
	lastAction = relax;
}

void Monster::stopping(bool doStand, bool forgetBoundTarget)
{
	if (doStand)
	{
		this->laxMovePosition = { -1, -1 };
		moveSystem.moveOffset = { 0, 0 };
		directionSystem.direction = Direction::STAND;
	}

	if (forgetBoundTarget && boundTarget != nullptr)
	{
		boundTarget->isProcessed = false;
		boundTarget = nullptr;
	}
}

Vector2f Monster::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Monster::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Monster::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

void Monster::fightInteract(long long elapsedTime, DynamicObject* target)
{
	moveSystem.pushAway(elapsedTime);
}
