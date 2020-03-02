#include "monster.h"

using namespace sf;

monster::monster(std::string objectName, const Vector2f centerPosition) : dynamic_object(std::move(objectName), centerPosition)
{
	current_sprite_[0] = 1;
	time_for_new_sprite_ = 0;
	currentAction = relax;
	directionSystem.side = down;
	sightRange = 950.0f;
	strikingSprite = 6;
	timeForNewHit = long(1e5);
}

monster::~monster()
= default;

void monster::behaviorWithStatic(world_object* target, long long elapsedTime)
{

}

void monster::behavior(const long long elapsedTime)
{
	endingPreviousAction();
	fightInteract(elapsedTime);
}

void monster::setTarget(dynamic_object& object)
{
	boundTarget = nullptr;
	if (object.tag == entity_tag::hero)
		return; //targetPosition = object.getPosition();
}

void monster::behaviorWithDynamic(dynamic_object* target, const long long elapsedTime)
{
	if (health_point_ <= 0)
	{
		changeAction(dead, true);
		directionSystem.direction = direction::STAND;
		return;
	}

	if (helper::getDist(position_, target->get_position()) <= radius_ + target->get_radius())
		moveSystem.pushByBumping(target->get_position(), target->get_radius(), target->getMoveSystem().canCrashIntoDynamic);

	if (target->tag != entity_tag::hero)
		return;

	if (helper::getDist(target->get_position(), position_) > sightRange)
	{		
		laxMovePosition = { -1, -1 };
		changeAction(relax, currentAction != relax);
		return;
	}

	boundTarget = target;
	directionSystem.side = direction_system::calculate_side(position_, boundTarget->get_position());

	if (helper::getDist(position_, boundTarget->get_position()) <= sightRange && timeAfterHit >= timeForNewHit)
		moveSystem.speed = std::max((1 - helper::getDist(position_, boundTarget->get_position()) / sightRange) * moveSystem.defaultSpeed / 2 + moveSystem.defaultSpeed, moveSystem.defaultSpeed);
	else
		moveSystem.speed = moveSystem.defaultSpeed;
	if (isAttack.count(currentAction) == 0)
		timeAfterHit += elapsedTime;

	// fight with player	
	doAttack(boundTarget);
	if (helper::getDist(position_, boundTarget->get_position()) <= (this->radius_ + boundTarget->get_radius() + hitDistance) &&
		isAttack.count(currentAction) > 0 && current_sprite_[0] == strikingSprite)
	{
		boundTarget->take_damage(this->strength, position_);
	}

	if (isAttack.count(currentAction) == 0 && helper::getDist(position_, boundTarget->get_position()) <= (this->radius_ + boundTarget->get_radius() + hitDistance / 5))
	{
		stopping(true, false);
		changeAction(combatState, false, false);								
	}
	//---------------------
	
	// move to player
	if (helper::getDist(position_, boundTarget->get_position()) > (this->radius_ + boundTarget->get_radius() + hitDistance / 5))
	{
		if (isAttack.count(currentAction) == 0 && currentAction != combatState)
		{
			changeAction(move, false);
			laxMovePosition = boundTarget->get_position();
		}
	}
	else	
		stopping(true, false);
}

void monster::endingPreviousAction()
{
	if (lastAction == combatState)
		changeAction(relax, true, false);
	if (lastAction == commonHit)
		changeAction(combatState, true, false);
	if (lastAction == directHit)
		changeAction(combatState, true, false);
	lastAction = relax;
}

void monster::stopping(const bool doStand, const bool forgetBoundTarget)
{
	if (doStand)
	{
		this->laxMovePosition = { -1, -1 };
		moveSystem.moveOffset = { 0, 0 };
		directionSystem.direction = direction::STAND;
	}

	if (forgetBoundTarget && boundTarget != nullptr)
	{
		boundTarget->is_processed = false;
		boundTarget = nullptr;
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

void monster::fightInteract(const long long elapsedTime, dynamic_object* target)
{
	moveSystem.pushAway(elapsedTime);
}
