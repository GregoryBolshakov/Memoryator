#include "nightmare_second.h"

nightmare_second::nightmare_second(std::string objectName, Vector2f centerPosition) : monster(std::move(objectName), centerPosition)
{
	conditionalSizeUnits = { 600, 600 };
	moveSystem.defaultSpeed = 0.0005f;
	moveSystem.speed = 0.0005f;
	radius = 120;
	hitDistance = 120;
	strength = 17;
	healthPoint = 180;
	currentAction = relax;
	timeForNewHitSelf = long(2e5);
	timeAfterHitSelf = timeForNewHitSelf;
	timeForNewHit = long(1e6);
	timeAfterHit = timeForNewHit;
	toSaveName = "nightmare2_";
	tag = entity_tag::nightmare2;
}


nightmare_second::~nightmare_second()
= default;


Vector2f nightmare_second::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2, textureBox.height * 6 / 8 };
}

void nightmare_second::doAttack(world_object* target)
{
	if (timeAfterHit >= timeForNewHit)
	{
		if (helper::getDist(position, boundTarget->getPosition()) <= (this->radius + boundTarget->getRadius() + hitDistance / 5))
		{
			changeAction(commonHit, true, false);
			timeAfterHit = 0;
		}
	}
}

std::vector<sprite_chain_element*> nightmare_second::prepareSprites(long long elapsedTime)
{
	auto body = new sprite_chain_element(pack_tag::nightmare2Stand, pack_part::full, Direction::DOWN, 1, position, conditionalSizeUnits, textureBoxOffset, color, mirrored, false);
	shakeSpeed = -1;
	animationSpeed = 10;

	Side spriteSide = directionSystem.side;
	Direction spriteDirection = directionSystem.lastDirection;

	if (directionSystem.side == right)
	{
		spriteSide = left;
		body->mirrored = true;
	}
	if (directionSystem.lastDirection == Direction::RIGHT || directionSystem.lastDirection == Direction::UPRIGHT || directionSystem.lastDirection == Direction::DOWNRIGHT)
	{
		spriteDirection = direction_system::cutRights(spriteDirection);
		body->mirrored = true;
	}

	body->direction = direction_system::sideToDirection(spriteSide);

	switch (currentAction)
	{
	case commonHit:
	{
		animationLength = 12;
		body->packTag = pack_tag::nightmare2Hit;
		break;
	}
	case combatState:
	{
		animationLength = 9;
		body->packTag = pack_tag::nightmare2Stand;
		break;
	}
	case relax:
	{
		animationLength = 9;
		body->packTag = pack_tag::nightmare2Stand;
		break;
	}
	case dead:
	{
		animationLength = 1;
		body->packTag = pack_tag::nightmare2Stand;
		currentSprite[0] = 1;
		deletePromiseOn();
		break;
	}
	case move:
	{
		animationLength = 8;
		body->packTag = pack_tag::nightmare2Move;
		if (currentSprite[0] == 2 || currentSprite[0] == 6)
			shakeSpeed = 0;
		break;
	}
	default:;
	}

	body->number = currentSprite[0];

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 1e6 / animationSpeed)
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

