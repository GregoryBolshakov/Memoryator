#include "Nightmare1.h"

Nightmare1::Nightmare1(std::string objectName, Vector2f centerPosition) : Monster(objectName, centerPosition)
{
	conditionalSizeUnits = { 375, 375 };
	defaultSpeed = 0.0005f;
	speed = 0.0005f;
	radius = 90;
	hitDistance = 70;
	strength = 10;
	healthPoint = 100;
	currentAction = relax;	
	timeForNewHitself = 2e5;
	timeAfterHitself = timeForNewHitself;
	timeForNewHit = 1e6;
	timeAfterHit = timeForNewHit;
	toSaveName = "nightmare1_";
	tag = Tag::nightmare1;
}


Nightmare1::~Nightmare1()
{
}


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

std::vector<SpriteChainElement*> Nightmare1::prepareSprites(long long elapsedTime)
{
    return {};
	/*spriteChainElement fullSprite;

	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);
	additionalSprites.clear();
	animationSpeed = 10;

	std::string sideStr = DynamicObject::sideToString(side), directionStr = DynamicObject::directionToString(direction);
	if (side == right)
	{
		sideStr = "left";
		fullSprite.mirrored = true;
	}
	if (direction == Direction::RIGHT || direction == Direction::UPRIGHT || direction == Direction::DOWNRIGHT)
	{
		directionStr = "left";
		fullSprite.mirrored = true;
	}

	switch (currentAction)
	{
		case commonHit:
		{
			animationLength = 9;
			fullSprite.path = "Game/worldSprites/nightmare1/hit/" + sideStr + '/';
			fullSprite.path += std::to_string(currentSprite[0]) + ".png";
			break;
		}
		case combatState:
		{
			animationLength = 13;
			fullSprite.path = "Game/worldSprites/nightmare1/stand/" + sideStr + '/';
			fullSprite.path += std::to_string(currentSprite[0]) + ".png";
			break;
		}
		case relax:
		{
			animationLength = 13;
			fullSprite.path = "Game/worldSprites/nightmare1/stand/" + sideStr + '/';
			fullSprite.path += std::to_string(currentSprite[0]) + ".png";
			break;
		}
		case dead:
		{
			animationLength = 1;
			fullSprite.path = "Game/worldSprites/nightmare1/stand/down/1.png";
			currentSprite[0] = 1;
			deletePromiseOn();
			break;
		}
		case move:
		{
			animationLength = 8;
			fullSprite.path = "Game/worldSprites/nightmare1/move/" + sideStr + '/';
			fullSprite.path += std::to_string(currentSprite[0]) + ".png";
			break;
		}
		default:;
	}	

	additionalSprites.push_back(fullSprite);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 1000000 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			lastAction = currentAction;		
			currentSprite[0] = 1;
		}
	}*/
}

