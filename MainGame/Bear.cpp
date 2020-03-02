#include "bear.h"

using namespace sf;

bear::bear(const std::string& objectName, Vector2f centerPosition) : neutral_mob(objectName, centerPosition)
{
	conditionalSizeUnits = { 432, 384 };
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	moveSystem.defaultSpeed = 0.0003f;
	moveSystem.speed = 0.0003f;
	animationSpeed = float(5e-4);
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
	toSaveName = "bear";
	tag = entity_tag::bear;
}

bear::~bear()
= default;

Vector2f bear::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2, textureBox.height * 7 / 8 };
}

void bear::behaviorWithStatic(world_object* target, long long elapsedTime)
{

}

void bear::behavior(long long elapsedTime)
{
	endingPreviousAction();
	fightInteract(elapsedTime);
	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		directionSystem.direction = direction::STAND;
		return;
	}

	if (this->owner != nullptr)
	{
		moveSystem.speed = moveSystem.defaultSpeed;
		if (currentAction == commonHit)
		{
			movePosition = position;
			return;
		}
		directionSystem.side = direction_system::calculate_side(position, owner->getPosition());
		if (helper::getDist(position, owner->getPosition()) > sightRange / 2)
		{
			changeAction(grab, false, false);
			movePosition = owner->getPosition();
		}
		else
			if (helper::getDist(position, owner->getPosition()) <= sightRange / 2.5 && currentAction != relax)
			{
				changeAction(relax, true, false);
				movePosition = position;
			}
		return;
	}

	directionSystem.side = direction_system::calculate_side(position, movePosition);

	if (boundTarget == nullptr)
		return;
	const float distanceToTarget = helper::getDist(this->position, boundTarget->getPosition());
	moveSystem.speed = std::max(moveSystem.defaultSpeed, (moveSystem.defaultSpeed * 10) * (1 - (distanceToTarget) / sightRange * 1.5f));
	//animationSpeed = std::max(0.0003f, 0.0003f * speed / defaultSpeed);

	if (distanceToTarget <= sightRange)
	{
		changeAction(move, false, true);
		movePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x), position.y - (boundTarget->getPosition().y - position.y));
	}
	else
	{
		if (currentAction == move)
		{
			if (distanceToTarget >= sightRange * 1.5)
			{
				changeAction(relax, true, true);
				directionSystem.direction = direction::STAND;
				movePosition = { -1, -1 };
			}
			else
				movePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x), position.y - (boundTarget->getPosition().y - position.y));
		}
	}
	distanceToNearest = 10e6;
	boundTarget = nullptr;
}

Vector2f bear::getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int bear::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void bear::endingPreviousAction()
{
	if (lastAction == commonHit)
		currentAction = relax;

	lastAction = relax;
}

void bear::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

std::vector<sprite_chain_element*> bear::prepareSprites(long long elapsedTime)
{
    return {};
	/*spriteChainElement fullSprite;

	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);
	additionalSprites.clear();
	std::string sideStr = DynamicObject::sideToString(side), directionStr = DynamicObject::directionToString(direction);
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
		fullSprite.path = "Game/worldSprites/bear/stand/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case commonHit:
	{
		animationLength = 6;
		animationSpeed = 10;
		fullSprite.path = "Game/worldSprites/bear/hunt/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		animationSpeed = 10;
		fullSprite.path = "Game/worldSprites/bear/stand/down/1.png";
		currentSprite[0] = 1;
	}
	default:;
	}

	if (currentAction == move || currentAction == grab)
	{
		animationLength = 7;
		animationSpeed = 10;
		fullSprite.path = "Game/worldSprites/bear/move/" + directionStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
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