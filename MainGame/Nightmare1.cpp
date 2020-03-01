#include "Nightmare1.h"

Nightmare1::Nightmare1(std::string objectName, const Vector2f centerPosition) : Monster(std::move(objectName), centerPosition)
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


Nightmare1::~Nightmare1()
= default;


Vector2f Nightmare1::calculateTextureOffset()
{	
	initMicroBlocks();
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2, textureBox.height * 7 / 8 };
}

void Nightmare1::doAttack(WorldObject* target)
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

std::vector<SpriteChainElement*> Nightmare1::prepareSprites(const long long elapsedTime)
{
	auto body = new SpriteChainElement(PackTag::nightmare1, PackPart::stand, Direction::DOWN, 1, position, conditionalSizeUnits, textureBoxOffset, color, mirrored, false);
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
		spriteDirection = DirectionSystem::cutRights(spriteDirection);
		body->mirrored = true;
	}

	body->direction = DirectionSystem::sideToDirection(spriteSide);

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

