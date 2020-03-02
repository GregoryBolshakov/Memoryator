#include "hare.h"

#include "hare_trap.h"

using namespace sf;

hare::hare(const std::string& objectName, Vector2f centerPosition) : neutral_mob(objectName, centerPosition)
{
	conditionalSizeUnits = { 180, 150 };
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	moveSystem.defaultSpeed = 0.0003f;
	moveSystem.speed = 0.0003f;
	radius = 70;
	strength = 10;
	sightRange = 720;
	healthPoint = 50;
	currentAction = relax;
	timeAfterHitSelf = 0;
	timeForNewHitSelf = long(6e5);
	timeForNewHit = 1000000;
	toSaveName = "hare";
	tag = entity_tag::hare;
}

hare::~hare()
= default;

Vector2f hare::calculateTextureOffset()
{
	textureBox.width = float(textureBox.width)*getScaleRatio().x;
	textureBox.height = float(textureBox.height)*getScaleRatio().y;
	return { textureBox.width / 2, textureBox.height * 7 / 8 };
}

void hare::setTarget(dynamic_object& object)
{
	if (object.tag == entity_tag::noose || currentAction == absorbs)
		return;
	if (helper::getDist(position, object.getPosition()) <= sightRange)
	{
		if (object.tag == entity_tag::hero)
		{
			boundTarget = &object;
			distanceToNearest = helper::getDist(position, object.getPosition());
		}
	}
}

void hare::behaviorWithStatic(world_object* target, long long elapsedTime)
{
	if (currentAction == absorbs)
		return;
	if (helper::getDist(position, target->getPosition()) <= sightRange && timeAfterFear >= fearTime)		
		if (target->tag == entity_tag::hareTrap)
		{
			if (boundTarget == nullptr)
			{
				boundTarget = target;
				distanceToNearest = helper::getDist(position, target->getPosition());
			}
		}
}

void hare::behavior(long long elapsedTime)
{	
	endingPreviousAction();
	fightInteract(elapsedTime);
	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		directionSystem.direction = Direction::STAND;
		return;
	}

	directionSystem.calculateDirection();
	if (directionSystem.direction != Direction::STAND)
		directionSystem.lastDirection = directionSystem.direction;

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
		if (helper::getDist(position, laxMovePosition) <= radius)
		{
			changeAction(relax, true, true);
			laxMovePosition = { -1, -1 };
		}
		return;
	}
	
	const float distanceToTarget = helper::getDist(this->position, boundTarget->getPosition());

	// bouncing to a trap
	if (boundTarget->tag == entity_tag::hareTrap)
	{
		directionSystem.side = direction_system::calculateSide(position, boundTarget->getPosition(), elapsedTime);
		if (helper::getDist(position, boundTarget->getPosition()) <= radius)
		{
			const auto trap = dynamic_cast<hare_trap*>(boundTarget);
			position = trap->getEnterPosition();
			changeAction(absorbs, true, false);
			laxMovePosition = { -1, -1 };
		}
		else
		{
			changeAction(move, false, true);
			laxMovePosition = boundTarget->getPosition();
		}
	}
	//-------------------

	// runaway from enemy
	if (boundTarget->tag == entity_tag::hero)
	{
		directionSystem.side = direction_system::calculateSide(position, laxMovePosition, elapsedTime);
		moveSystem.speed = std::max(moveSystem.defaultSpeed, (moveSystem.defaultSpeed * 10) * (1 - (helper::getDist(position, boundTarget->getPosition()) / sightRange * 1.5f)));
		animationSpeed = std::max(0.0004f, 0.0003f * moveSystem.speed / moveSystem.defaultSpeed);
		if (distanceToTarget <= sightRange)
		{
			changeAction(move, false, true);
			float k = (sightRange/* * 1.5f - Helper::getDist(position, boundTarget->getPosition())*/) / helper::getDist(position, boundTarget->getPosition());
			laxMovePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x) * k, position.y - (boundTarget->getPosition().y - position.y) * k);
		}
		else
		{
			if (currentAction == move)
			{
				if (distanceToTarget >= sightRange * 1.5f)
				{
					changeAction(relax, true, true);
					directionSystem.direction = Direction::STAND;
					laxMovePosition = { -1, -1 };
				}
				else
				{
					float k = (sightRange/* * 1.5f - Helper::getDist(position, boundTarget->getPosition())*/) / helper::getDist(position, boundTarget->getPosition());
					laxMovePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x) * k, position.y - (boundTarget->getPosition().y - position.y) * k);
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

Vector2f hare::getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int hare::getBuildType(Vector2f ounPos, Vector2f otherPos)
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
		deletePromiseOn();
	}
	lastAction = relax;
}

void hare::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

std::vector<sprite_chain_element*> hare::prepareSprites(long long elapsedTime)
{
	auto body = new sprite_chain_element(pack_tag::hare, pack_part::full, Direction::DOWN, 1, position, conditionalSizeUnits, textureBoxOffset, color, mirrored, false);
	animationSpeed = 10;

	Side spriteSide = directionSystem.side;

	if (directionSystem.side == right)
	{
		spriteSide = left;
		body->mirrored = true;
	}
	if (directionSystem.lastDirection == Direction::RIGHT || directionSystem.lastDirection == Direction::UPRIGHT || directionSystem.lastDirection == Direction::DOWNRIGHT)
	{
		body->mirrored = true;
	}

	body->direction = direction_system::sideToDirection(spriteSide);

	switch (currentAction)
	{
		case relax:
		{
			animationLength = 1;
			body->packPart = pack_part::stand;
			break;
		}
		case absorbs:
		{
			animationLength = 10;
			body->packPart = pack_part::trap;
			break;
		}
		case dead:
		{
			animationLength = 1;
			body->packPart = pack_part::stand;
			currentSprite[0] = 1;
			break;
		}
		case move:
		{
			animationLength = 5;
			body->packPart = pack_part::move;						
			break;
		}
	default:;
	}

	body->number = currentSprite[0];

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= long(1e6 / animationSpeed))
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			lastAction = currentAction;
			currentSprite[0] = 1;
		}
	}

	return { body };
}
