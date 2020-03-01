#include "Deer.h"

using namespace sf;

Deer::Deer(const std::string& objectName, Vector2f centerPosition) : NeutralMob(objectName, centerPosition)
{
	conditionalSizeUnits = { 360, 300 };
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	defaultSpeed = 0.00045f;
	speed = 0.00045f;
	animationSpeed = 10;
	animationLength = 8;
	radius = 70;
	strength = 10;
	sightRange = conditionalSizeUnits.x * 3;
	morality = 5; // from 1 to 10
	fear = 0;
	healthPoint = 50;
	currentAction = relax;	
	timeForNewHitSelf = long(6e5);
	timeAfterHitSelf = timeForNewHitSelf;
	timeForNewHit = 10e6;
	toSaveName = "deer";
	tag = Tag::deer;
}

Deer::~Deer()
= default;

Vector2f Deer::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2, textureBox.height * 7 / 8 };
}

void Deer::behaviorWithStatic(WorldObject* target, long long elapsedTime)
{

}

void Deer::behavior(const long long elapsedTime)
{
	endingPreviousAction();
	fightInteract(elapsedTime);
	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		direction = Direction::STAND;
		return;
	}
	
	if (this->owner != nullptr)
	{		
		speed = defaultSpeed;
		if (currentAction == commonHit)
		{
			laxMovePosition = { -1, -1 };
			return;			
		}
		side = calculateSide(owner->getPosition());
		if (Helper::getDist(position, owner->getPosition()) > sightRange / 2)
		{
			changeAction(grab, false, false);
			laxMovePosition = owner->getPosition();
		}
		else
			if (Helper::getDist(position, owner->getPosition()) <= sightRange / 2.5 && currentAction != relax)
			{
				changeAction(relax, true, false);
				laxMovePosition = { -1, -1 };
			}
		return;
	}

	side = calculateSide(laxMovePosition);

	if (boundTarget == nullptr)
		return;
	const float distanceToTarget = Helper::getDist(this->position, boundTarget->getPosition());
	speed = std::max(defaultSpeed, defaultSpeed * 10 * (1 - distanceToTarget / sightRange * 1.5f));
	animationSpeed = std::max(0.0004f, 0.0003f * speed / defaultSpeed);

	if (distanceToTarget <= sightRange)
	{
		changeAction(move, false, true);
		laxMovePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x), position.y - (boundTarget->getPosition().y - position.y));
	}
	else
	{
		if (currentAction == move)
		{
			if (distanceToTarget >= sightRange * 1.5)
			{
				changeAction(relax, true, true);
				direction = Direction::STAND;
				laxMovePosition = { -1, -1 };
			}
			else
				laxMovePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x), position.y - (boundTarget->getPosition().y - position.y));
		}
	}
	distanceToNearest = 10e6;
	boundTarget = nullptr;
}

Vector2f Deer::getBuildPosition(std::vector<WorldObject*>, float, Vector2f)
{
	return { -1, -1 };
}

int Deer::getBuildType(Vector2f, Vector2f)
{
	return 1;
}

void Deer::endingPreviousAction()
{
	if (lastAction == commonHit)	
		currentAction = relax;

	lastAction = relax;
}

void Deer::jerk(float, float, Vector2f)
{
}

Vector2f Deer::getHeadPosition()
{
	const Vector2f upperLeft = Vector2f(position.x - textureBoxOffset.x, position.y - textureBoxOffset.y);

	if (side == up && laxMovePosition == position || laxMovePosition != position && direction == Direction::UP)
	{
		if (currentSprite[0] == 1)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.569f, upperLeft.y + conditionalSizeUnits.y * 0.111f);
		if (currentSprite[0] == 2)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.575f, upperLeft.y + conditionalSizeUnits.y * 0.155f);
		if (currentSprite[0] == 3)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.577f, upperLeft.y + conditionalSizeUnits.y * 0.202f);
		if (currentSprite[0] == 4)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.580f, upperLeft.y + conditionalSizeUnits.y * 0.150f);
		if (currentSprite[0] == 5)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.570f, upperLeft.y + conditionalSizeUnits.y * 0.106f);
		if (currentSprite[0] == 6)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.570f, upperLeft.y + conditionalSizeUnits.y * 0.093f);
		if (currentSprite[0] == 7)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.575f, upperLeft.y + conditionalSizeUnits.y * 0.075f);
	}
	if (side == down && laxMovePosition == position || laxMovePosition != position && direction == Direction::DOWN)
	{
		if (currentSprite[0] == 1)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.446f, upperLeft.y + conditionalSizeUnits.y * 0.222f);
		if (currentSprite[0] == 2)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.437f, upperLeft.y + conditionalSizeUnits.y * 0.243f);
		if (currentSprite[0] == 3)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.437f, upperLeft.y + conditionalSizeUnits.y * 0.258f);
		if (currentSprite[0] == 4)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.437f, upperLeft.y + conditionalSizeUnits.y * 0.242f);
		if (currentSprite[0] == 5)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.440f, upperLeft.y + conditionalSizeUnits.y * 0.168f);
		if (currentSprite[0] == 6)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.444f, upperLeft.y + conditionalSizeUnits.y * 0.150f);
		if (currentSprite[0] == 7)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.445f, upperLeft.y + conditionalSizeUnits.y * 0.182f);
	}
	if (side == left && laxMovePosition == position || (laxMovePosition != position && direction == Direction::LEFT || direction == Direction::UPLEFT || direction == Direction::DOWNLEFT))
	{
		if (currentSprite[0] == 1)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.313f, upperLeft.y + conditionalSizeUnits.y * 0.147f);
		if (currentSprite[0] == 2)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.325f, upperLeft.y + conditionalSizeUnits.y * 0.159f);
		if (currentSprite[0] == 3)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.338f, upperLeft.y + conditionalSizeUnits.y * 0.169f);
		if (currentSprite[0] == 4)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.319f, upperLeft.y + conditionalSizeUnits.y * 0.138f);
		if (currentSprite[0] == 5)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.287f, upperLeft.y + conditionalSizeUnits.y * 0.123f);
		if (currentSprite[0] == 6)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.282f, upperLeft.y + conditionalSizeUnits.y * 0.130f);
		if (currentSprite[0] == 7)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.277f, upperLeft.y + conditionalSizeUnits.y * 0.138f);
	}
	if (side == right && laxMovePosition == position || (laxMovePosition != position && direction == Direction::RIGHT || direction == Direction::UPRIGHT || direction == Direction::DOWNRIGHT))
	{
		if (currentSprite[0] == 1)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.6f, upperLeft.y + conditionalSizeUnits.y * 0.17f);
		if (currentSprite[0] == 2)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.59f, upperLeft.y + conditionalSizeUnits.y * 0.19f);
		if (currentSprite[0] == 3)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.56f, upperLeft.y + conditionalSizeUnits.y * 0.2f);
		if (currentSprite[0] == 4)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.59f, upperLeft.y + conditionalSizeUnits.y * 0.17f);
		if (currentSprite[0] == 5)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.62f, upperLeft.y + conditionalSizeUnits.y * 0.16f);
		if (currentSprite[0] == 6)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.63f, upperLeft.y + conditionalSizeUnits.y * 0.16f);
		if (currentSprite[0] == 7)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.63f, upperLeft.y + conditionalSizeUnits.y * 0.17f);
	}
	throw std::logic_error("Unable to calculate the position of the head.");
}

std::vector<SpriteChainElement*> Deer::prepareSprites(long long elapsedTime)
{
    return {};
	/*spriteChainElement fullSprite;

	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);
	additionalSprites.clear();
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
		case relax:
		{
			animationLength = 1;
			animationSpeed = 10;
			fullSprite.path = "Game/worldSprites/deer/stand/" + sideStr + '/';
			fullSprite.path += std::to_string(currentSprite[0]) + ".png";
			break;
		}
		case commonHit:
		{
			animationLength = 6;
			animationSpeed = 10;
			fullSprite.path = "Game/worldSprites/deer/hunt/" + sideStr + '/';
			fullSprite.path += std::to_string(currentSprite[0]) + ".png";
			break;
		}
		case dead:
		{
			animationLength = 1;
			animationSpeed = 10;
			fullSprite.path = "Game/worldSprites/deer/stand/down/1.png";
			currentSprite[0] = 1;
		}
	default:;
	}

	if (currentAction == move || currentAction == grab)
	{
		animationLength = 7;
		animationSpeed = 10;
		fullSprite.path = "Game/worldSprites/deer/move/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
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