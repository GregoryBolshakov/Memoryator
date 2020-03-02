#include "deer.h"

using namespace sf;

deer::deer(const std::string& objectName, const Vector2f centerPosition) : neutral_mob(objectName, centerPosition)
{
	conditionalSizeUnits = {360, 300};
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	moveSystem.defaultSpeed = 0.00085F;
	//defaultSpeed = 0.0002f;
	moveSystem.speed = moveSystem.defaultSpeed;
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

deer::~deer()
= default;

Vector2f deer::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return {textureBox.width / 2, textureBox.height * 7 / 8};
}

void deer::behaviorWithStatic(WorldObject* target, long long elapsedTime)
{
}

void deer::behavior(const long long elapsedTime)
{
	endingPreviousAction();
	directionSystem.calculateDirection();
	fightInteract(elapsedTime);

	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		directionSystem.direction = Direction::STAND;
		return;
	}

	if (this->owner != nullptr)
	{
		moveSystem.speed = moveSystem.defaultSpeed;
		if (currentAction == commonHit)
		{
			laxMovePosition = {-1, -1};
			return;
		}
		directionSystem.side = DirectionSystem::calculateSide(position, owner->getPosition(), elapsedTime);
		if (Helper::getDist(position, owner->getPosition()) > sightRange / 2)
		{
			changeAction(moveSlowly, false, false);
			laxMovePosition = owner->getPosition();
		}
		else if (Helper::getDist(position, owner->getPosition()) <= sightRange / 2.5 && currentAction != relax)
		{
			changeAction(relax, true, false);
			laxMovePosition = {-1, -1};
		}
		return;
	}

	directionSystem.side = DirectionSystem::calculateSide(position, laxMovePosition, elapsedTime);

	if (boundTarget == nullptr)
	{
		return;
	}

	const auto distanceToTarget = Helper::getDist(this->position, boundTarget->getPosition());
	//speed = std::max(defaultSpeed, (defaultSpeed * 10) * (1 - (distanceToTarget) / sightRange * 1.5f));
	animationSpeed = std::max(0.0004F, 0.0003F * moveSystem.speed / moveSystem.defaultSpeed);

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
				directionSystem.direction = Direction::STAND;
				laxMovePosition = {-1, -1};
			}
			else
			{
				laxMovePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x), position.y - (boundTarget->getPosition().y - position.y));
			}
		}
	}
	distanceToNearest = 10e6;
	boundTarget = nullptr;
}

Vector2f deer::getBuildPosition(std::vector<WorldObject*> /*visibleItems*/, float /*scaleFactor*/, Vector2f /*cameraPosition*/)
{
	return {-1, -1};
}

int deer::getBuildType(Vector2f /*ounPos*/, Vector2f /*otherPos*/)
{
	return 1;
}

void deer::endingPreviousAction()
{
	if (lastAction == commonHit)
	{
		currentAction = relax;
	}

	lastAction = relax;
}

void deer::jerk(float /*power*/, float /*deceleration*/, Vector2f /*destinationPoint*/)
{
}

Vector2f deer::getHeadPosition()
{
	const auto upperLeft = Vector2f(position.x - textureBoxOffset.x, position.y - textureBoxOffset.y);

	if (directionSystem.lastDirection == Direction::UP)
	{
		if (currentSprite[0] == 1)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.569F, upperLeft.y + conditionalSizeUnits.y * 0.111F};
		}
		if (currentSprite[0] == 2)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.575F, upperLeft.y + conditionalSizeUnits.y * 0.155F};
		}
		if (currentSprite[0] == 3)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.577F, upperLeft.y + conditionalSizeUnits.y * 0.202F};
		}
		if (currentSprite[0] == 4)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.580F, upperLeft.y + conditionalSizeUnits.y * 0.150F};
		}
		if (currentSprite[0] == 5)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.570F, upperLeft.y + conditionalSizeUnits.y * 0.106F};
		}
		if (currentSprite[0] == 6)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.570F, upperLeft.y + conditionalSizeUnits.y * 0.093F};
		}
		if (currentSprite[0] == 7)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.575F, upperLeft.y + conditionalSizeUnits.y * 0.075F};
		}
	}
	if (directionSystem.lastDirection == Direction::DOWN)
	{
		if (currentSprite[0] == 1)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.446F, upperLeft.y + conditionalSizeUnits.y * 0.222F};
		}
		if (currentSprite[0] == 2)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.437F, upperLeft.y + conditionalSizeUnits.y * 0.243F};
		}
		if (currentSprite[0] == 3)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.437F, upperLeft.y + conditionalSizeUnits.y * 0.258F};
		}
		if (currentSprite[0] == 4)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.437F, upperLeft.y + conditionalSizeUnits.y * 0.242F};
		}
		if (currentSprite[0] == 5)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.440F, upperLeft.y + conditionalSizeUnits.y * 0.168F};
		}
		if (currentSprite[0] == 6)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.444F, upperLeft.y + conditionalSizeUnits.y * 0.150F};
		}
		if (currentSprite[0] == 7)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.445F, upperLeft.y + conditionalSizeUnits.y * 0.182F};
		}
	}
	if (DirectionSystem::cutDiagonals(directionSystem.lastDirection) == Direction::LEFT)
	{
		if (currentSprite[0] == 1)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.313F, upperLeft.y + conditionalSizeUnits.y * 0.147F};
		}
		if (currentSprite[0] == 2)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.325F, upperLeft.y + conditionalSizeUnits.y * 0.159F};
		}
		if (currentSprite[0] == 3)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.338F, upperLeft.y + conditionalSizeUnits.y * 0.169F};
		}
		if (currentSprite[0] == 4)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.319F, upperLeft.y + conditionalSizeUnits.y * 0.138F};
		}
		if (currentSprite[0] == 5)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.287F, upperLeft.y + conditionalSizeUnits.y * 0.123F};
		}
		if (currentSprite[0] == 6)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.282F, upperLeft.y + conditionalSizeUnits.y * 0.130F};
		}
		if (currentSprite[0] == 7)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.277F, upperLeft.y + conditionalSizeUnits.y * 0.138F};
		}
	}
	if (DirectionSystem::cutDiagonals(directionSystem.lastDirection) == Direction::RIGHT)
	{
		if (currentSprite[0] == 1)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.6F, upperLeft.y + conditionalSizeUnits.y * 0.17F};
		}
		if (currentSprite[0] == 2)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.59F, upperLeft.y + conditionalSizeUnits.y * 0.19F};
		}
		if (currentSprite[0] == 3)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.56F, upperLeft.y + conditionalSizeUnits.y * 0.2F};
		}
		if (currentSprite[0] == 4)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.59F, upperLeft.y + conditionalSizeUnits.y * 0.17F};
		}
		if (currentSprite[0] == 5)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.62F, upperLeft.y + conditionalSizeUnits.y * 0.16F};
		}
		if (currentSprite[0] == 6)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.63F, upperLeft.y + conditionalSizeUnits.y * 0.16F};
		}
		if (currentSprite[0] == 7)
		{
			return {upperLeft.x + conditionalSizeUnits.x * 0.63F, upperLeft.y + conditionalSizeUnits.y * 0.17F};
		}
	}
	return position;
}

std::vector<SpriteChainElement*> deer::prepareSprites(long long elapsedTime)
{
	auto body = new SpriteChainElement(PackTag::deer, PackPart::stand, Direction::DOWN, 1, position, conditionalSizeUnits, textureBoxOffset, color, mirrored, false);
	animationSpeed = 10;

	auto spriteDirection = DirectionSystem::cutDiagonals(directionSystem.lastDirection);

	if (directionSystem.side == right)
	{
		body->mirrored = true;
	}
	if (directionSystem.lastDirection == Direction::RIGHT || directionSystem.lastDirection == Direction::UPRIGHT || directionSystem.lastDirection == Direction::DOWNRIGHT)
	{
		spriteDirection = DirectionSystem::cutRights(spriteDirection);
		body->mirrored = true;
	}

	body->direction = spriteDirection;

	switch (currentAction)
	{
	case relax:
		{
			animationLength = 1;
			break;
		}
	case commonHit:
		{
			animationLength = 6;
			body->packPart = PackPart::hunt;
			break;
		}
	case dead:
		{
			animationLength = 1;
			body->direction = Direction::DOWN;
			currentSprite[0] = 1;
			break;
		}
	case move:
	case moveSlowly:
		{
			animationLength = 7;
			body->packPart = PackPart::move;
			break;
		}
	default: ;
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

	return {body};
}
