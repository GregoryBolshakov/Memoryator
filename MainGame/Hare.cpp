#include "Hare.h"
#include "HareTrap.h"

using namespace sf;

Hare::Hare(const std::string objectName, Vector2f centerPosition) : NeutralMob(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i (180, 150);
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	defaultSpeed = 0.0003f;
	speed = 0.0003f;	
	radius = 70;
	strength = 10;
	sightRange = 720;
	healthPoint = 50;
	currentAction = relax;
	timeAfterHitself = 0;
	timeForNewHitself = 6e5;
	timeForNewHit = 1000000;
	toSaveName = "hare";
	tag = Tag::hare;
}

Hare::~Hare()
{
}

Vector2i Hare::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, textureBox.height * 7 / 8);
}

void Hare::setTarget(DynamicObject& object)
{
	if (object.tag == Tag::noose || currentAction == absorbs)
		return;
	if (Helper::getDist(position, object.getPosition()) <= sightRange)
	{
		if (object.tag == Tag::hero1)
		{
			boundTarget = &object;
			distanceToNearest = Helper::getDist(position, object.getPosition());
		}
	}
}

void Hare::behaviorWithStatic(WorldObject* target, float elapsedTime)
{
	if (currentAction == absorbs)
		return;
	if (Helper::getDist(position, target->getPosition()) <= sightRange && timeAfterFear >= fearTime)		
		if (target->tag == Tag::hareTrap)
		{
			if (boundTarget == nullptr)
			{
				boundTarget = target;
				distanceToNearest = Helper::getDist(position, target->getPosition());
			}
		}
}

void Hare::behavior(float elapsedTime)
{	
	endingPreviousAction();
	fightLogic(elapsedTime);
	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		direction = Direction::STAND;
		return;
	}

	direction = calculateDirection();
	if (direction != Direction::STAND)
		lastDirection = direction;

	// first-priority actions
	if (boundTarget)
	{
		if (boundTarget->tag == Tag::hero1)
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
		if (Helper::getDist(position, laxMovePosition) <= radius)
		{
			changeAction(relax, true, true);
			laxMovePosition = { -1, -1 };
		}
		return;
	}
	
	const float distanceToTarget = Helper::getDist(this->position, boundTarget->getPosition());

	// bouncing to a trap
	if (boundTarget->tag == Tag::hareTrap)
	{
		side = calculateSide(boundTarget->getPosition(), elapsedTime);
		if (Helper::getDist(position, boundTarget->getPosition()) <= radius)
		{
			const auto trap = dynamic_cast<HareTrap*>(boundTarget);
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
	if (boundTarget->tag == Tag::hero1)
	{
		side = calculateSide(laxMovePosition, elapsedTime);
		speed = std::max(defaultSpeed, (defaultSpeed * 10) * (1 - (Helper::getDist(position, boundTarget->getPosition()) / sightRange * 1.5f)));
		animationSpeed = std::max(0.0004f, 0.0003f * speed / defaultSpeed);
		if (distanceToTarget <= sightRange)
		{
			changeAction(move, false, true);
			float k = (sightRange/* * 1.5f - Helper::getDist(position, boundTarget->getPosition())*/) / Helper::getDist(position, boundTarget->getPosition());
			laxMovePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x) * k, position.y - (boundTarget->getPosition().y - position.y) * k);
		}
		else
		{
			if (currentAction == move)
			{
				if (distanceToTarget >= sightRange * 1.5f)
				{
					changeAction(relax, true, true);
					direction = Direction::STAND;
					laxMovePosition = { -1, -1 };
				}
				else
				{
					float k = (sightRange/* * 1.5f - Helper::getDist(position, boundTarget->getPosition())*/) / Helper::getDist(position, boundTarget->getPosition());
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

Vector2f Hare::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Hare::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Hare::endingPreviousAction()
{
	if (lastAction == commonHit)
		currentAction = relax;
	if (lastAction == absorbs)
	{
		auto trap = dynamic_cast<HareTrap*>(boundTarget);
		trap->inventory[0] = std::make_pair(Tag::hare, 1);
		deletePromiseOn();
	}
	lastAction = relax;
}

void Hare::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

std::vector<SpriteChainElement> Hare::prepareSprites(long long elapsedTime)
{
    return {};
	/*spriteChainElement fullSprite;

	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);
	additionalSprites.clear();
	animationSpeed = 10;
	std::string sideStr = DynamicObject::sideToString(side), directionStr = DynamicObject::directionToString(lastDirection);
	if (side == right)
	{
		sideStr = "left";
		fullSprite.mirrored = true;
	}
	if (lastDirection == Direction::RIGHT || lastDirection == Direction::UPRIGHT || lastDirection == Direction::DOWNRIGHT)
	{
		directionStr = "left";
		fullSprite.mirrored = true;
	}

	switch (currentAction)
	{
		case relax:
		{
			animationLength = 1;
			fullSprite.path = "Game/worldSprites/hare/stand/" + directionStr + "/1.png";
			break;
		}
		case absorbs:
		{
			animationLength = 10;
			fullSprite.path = "Game/worldSprites/hare/trap/" + std::to_string(currentSprite[0]) + ".png";
			break;
		}
		case dead:
		{
			animationLength = 1;
			fullSprite.path = "Game/worldSprites/hare/stand/down/1.png";
			currentSprite[0] = 1;
			break;
		}
		case move:
		{
			animationLength = 5;			
			fullSprite.path = "Game/worldSprites/hare/move/" + sideStr + '/' + std::to_string(currentSprite[0]) + ".png";
			break;
		}
	default:;
	}

	additionalSprites.push_back(fullSprite);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 1e6 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			lastAction = currentAction;
			currentSprite[0] = 1;
		}
	}*/
}
