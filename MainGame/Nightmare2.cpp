#include "Nightmare2.h"

Nightmare2::Nightmare2(std::string objectName, Vector2f centerPosition) : Monster(objectName, centerPosition)
{
	conditionalSizeUnits = { 600, 600 };
	defaultSpeed = 0.0005f;
	speed = 0.0005f;
	radius = 120;
	hitDistance = 120;
	strength = 17;
	healthPoint = 180;
	currentAction = relax;
	timeForNewHitself = 2e5;
	timeAfterHitself = timeForNewHitself;
	timeForNewHit = 1e6;
	timeAfterHit = timeForNewHit;
	toSaveName = "nightmare2_";
	tag = Tag::nightmare2;
}


Nightmare2::~Nightmare2()
{
}


Vector2f Nightmare2::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2, textureBox.height * 6 / 8 };
}

void Nightmare2::doAttack(WorldObject* target)
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

std::vector<SpriteChainElement*> Nightmare2::prepareSprites(long long elapsedTime)
{
	auto body = new SpriteChainElement(PackTag::nightmare2Stand, PackPart::full, Direction::DOWN, 1, position, conditionalSizeUnits, textureBoxOffset, color, mirrored, false);
	animationSpeed = 10;

	Side spriteSide = side;
	Direction spriteDirection = lastDirection;

	if (side == right)
	{
		spriteSide = left;
		body->mirrored = true;
	}
	if (lastDirection == Direction::RIGHT || lastDirection == Direction::UPRIGHT || lastDirection == Direction::DOWNRIGHT)
	{
		spriteDirection = DirectionsSystem::cutRights(spriteDirection);
		body->mirrored = true;
	}

	body->direction = DirectionsSystem::sideToDirection(spriteSide);

	switch (currentAction)
	{
	case commonHit:
	{
		animationLength = 12;
		body->packTag = PackTag::nightmare2Hit;
		break;
	}
	case combatState:
	{
		animationLength = 9;
		body->packTag = PackTag::nightmare2Stand;
		break;
	}
	case relax:
	{
		animationLength = 9;
		body->packTag = PackTag::nightmare2Stand;
		break;
	}
	case dead:
	{
		animationLength = 1;
		body->packTag = PackTag::nightmare2Stand;
		currentSprite[0] = 1;
		deletePromiseOn();
		break;
	}
	case move:
	{
		animationLength = 8;
		body->packTag = PackTag::nightmare2Move;
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

