#include "nightmare_first.h"

nightmare_first::nightmare_first(std::string objectName, const Vector2f centerPosition) : monster(std::move(objectName), centerPosition)
{
	conditionalSizeUnits = { 375, 375 };
	moveSystem.defaultSpeed = 0.0005f;
	moveSystem.speed = 0.0005f;
	radius = 90;
	hitDistance = 70;
	strength = 10;
	healthPoint = 100;
	currentAction = relax;	
	timeForNewHitSelf = long(2e5);
	timeAfterHitSelf = timeForNewHitSelf;
	timeForNewHit = long(1e6);
	timeAfterHit = timeForNewHit;
	toSaveName = "nightmare1_";
	tag = Tag::nightmare1;
}


nightmare_first::~nightmare_first()
= default;


Vector2f nightmare_first::calculateTextureOffset()
{	
	initMicroBlocks();
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2, textureBox.height * 7 / 8 };
}

void nightmare_first::doAttack(world_object* target)
{
	if (timeAfterHit >= timeForNewHit)
	{
		if (Helper::getDist(position, boundTarget->getPosition()) <= (this->radius + boundTarget->getRadius() + hitDistance / 5))
		{
			changeAction(commonHit, true, false);
			timeAfterHit = 0;
		}
	}
}

std::vector<sprite_chain_element*> nightmare_first::prepareSprites(const long long elapsedTime)
{
	auto body = new sprite_chain_element(PackTag::nightmare1, PackPart::stand, Direction::DOWN, 1, position, conditionalSizeUnits, textureBoxOffset, color, mirrored, false);
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
		animationLength = 9;
		body->packTag = PackTag::nightmare1;
		body->packPart = PackPart::hit;		
		break;
	}
	case combatState:
	case relax:
	{
		animationLength = 13;
		body->packTag = PackTag::nightmare1;
		body->packPart = PackPart::stand;
		break;
	}
	case dead:
	{
		animationLength = 1;
		body->packTag = PackTag::nightmare1;
		body->packPart = PackPart::stand;
		currentSprite[0] = 1;
		deletePromiseOn();
		break;
	}
	case move:
	{
		animationLength = 8;
		body->packTag = PackTag::nightmare1;
		body->packPart = PackPart::move;
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

