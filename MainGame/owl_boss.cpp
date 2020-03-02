#include "owl_boss.h"

using namespace sf;

owl_boss::owl_boss(std::string objectName, Vector2f centerPosition) : dynamic_object(std::move(objectName), centerPosition)
{
	conditionalSizeUnits = { 600, 600 };
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	moveSystem.defaultSpeed = 0.0005f;
	animationSpeed = 0.0005f;
	animationLength = 1;
	currentSprite[0] = 1;
	radius = 100;
	strength = 100;
	healthPoint = 1000;
	currentAction = rightFlap;
	timeAfterHitSelf = 0;
	timeForNewHitSelf = long(6e5);
	timeForNewHit = 2000000;
	routeGenerationAbility = false;
	moveSystem.canCrashIntoDynamic = false;

	toSaveName = "monster";
}

owl_boss::~owl_boss()
= default;

void owl_boss::behaviorWithStatic(world_object* target, long long elapsedTime)
{

}

void owl_boss::behavior(long long elapsedTime)
{
	fightInteract(elapsedTime);
	//jerk interaction
	if (isJerking)
	{
		if (jerkTime > 0)
		{
			jerkTime -= elapsedTime;
			moveSystem.speed = jerkDistance / float(jerkDuration) * float(pow(jerkTime / jerkDuration, jerkDeceleration));
			moveSystem.speed = std::max(moveSystem.defaultSpeed / jerkDeceleration, moveSystem.speed);
			//speed = defaultSpeed;
		}
		else
		{
			isJerking = false;
			moveSystem.speed = moveSystem.defaultSpeed;
		}
	}
	//----------------

	//processing ending of previous actions
	if (lastAction == upFlap || lastAction == leftFlap || lastAction == rightFlap || lastAction == startFlap)
	{
		if (--flapsBeforeJerkCount > 0)
		{
			if (flapsBeforeJerkCount == 1)
				changeAction(startFlap, true, false);
			else
				changeAction(Actions(30 + rand() % 3), true, false);
		}
		lastAction = relax;
		//changeAction(rightFlap, false, true);
		//lastAction = relax;
	}

	//------------------

	//processing current actions
	if (currentAction == rightFlap && !isJerking && currentSprite[0] == 7)
		jerk(2, 1, Vector2f(position.x - 200, position.y));
	if (currentAction == leftFlap && !isJerking && currentSprite[0] == 7)
		jerk(2, 1, Vector2f(position.x + 200, position.y));
	//--------------------------
}

Vector2f owl_boss::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2, textureBox.height * 7 / 8 };
}

void owl_boss::setTarget(dynamic_object& object)
{
	if (object.tag == Tag::hero)
		return; //targetPosition = object.getPosition();
}

void owl_boss::behaviorWithDynamic(dynamic_object* target, const long long elapsedTime)
{
	debugInfo = std::to_string(Helper::getDist(this->position, movePosition) / jerkDistance * 1000.0f);
	if (healthPoint <= 0)
	{
		changeAction(dead, true, false);
		directionSystem.direction = Direction::STAND;
		return;
	}

	if (target->tag != Tag::hero)
		return;

	directionSystem.side = direction_system::calculateSide(position, movePosition, elapsedTime);

	timeAfterHit += elapsedTime;

	if (jerkDistance != 0 && currentAction == move)
	{
		const auto dragCoefficient = 1.2f; // 1..2
		const auto startPower = 1.1f; //1..2
		moveSystem.speed = pow(moveSystem.defaultSpeed, dragCoefficient / (startPower + float(timeAfterHit) / float(timeForNewHit)));
		if (Helper::getDist(this->position, target->getPosition()) / jerkDistance <= 0.2f)
			moveSystem.speed *= Helper::getDist(this->position, target->getPosition()) / jerkDistance;
	}
	moveSystem.speed = std::max(moveSystem.defaultSpeed / 10, moveSystem.speed);

	if (flapsBeforeJerkCount > 1)
	{
		timeAfterHit = 0;
		if (!isJerking)
			movePosition = this->position;
		return;
	}
	if (flapsBeforeJerkCount == 0)
	{
		changeAction(move, false, true);
		moveSystem.speed = moveSystem.defaultSpeed;
		jerkDistance = Helper::getDist(this->position, target->getPosition());
	}

	if (moveSystem.speed <= moveSystem.defaultSpeed * 3) //with greater acceleration the owl loses the ability to coordinate the route
	{
		movePosition = Vector2f(target->getPosition().x + (target->getPosition().x - this->position.x) / 1.0f, target->getPosition().y + (target->getPosition().y - this->position.y) / 1.0f);
	}

	if (Helper::getDist(this->position, movePosition) / moveSystem.speed <= (40 / animationSpeed) * 3 && currentAction == move)
	{
		changeAction(stopFlap, true, false);
	}

	if (Helper::getDist(this->position, movePosition) <= this->radius) //hit interaction
	{
		if (Helper::getDist(this->position, target->getPosition()) <= this->radius)
			target->takeDamage(this->strength, position);
		timeAfterHit = 0;
		flapsBeforeJerkCount = rand() % 3 + 3;
		changeAction(upFlap, true, true);
		moveSystem.speed = moveSystem.defaultSpeed;
	}
}

Vector2f owl_boss::getBuildPosition(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

int owl_boss::getBuildType(Vector2f, Vector2f)
{
	return 1;
}

void owl_boss::jerk(const float power, const float deceleration, const Vector2f destinationPoint)
{
	this->jerkPower = power;
	this->jerkDeceleration = deceleration;
	this->jerkDuration = long(40.0f / animationSpeed * 10.0f);
	this->jerkTime = this->jerkDuration;
	isJerking = true;
	jerkDistance = 400;

	movePosition = Vector2f(destinationPoint);
}

void owl_boss::fightInteract(const long long elapsedTime, dynamic_object* target)
{
	moveSystem.pushAway(elapsedTime);
}

std::vector<sprite_chain_element*> owl_boss::prepareSprites(long long elapsedTime)
{
    return {};
	/*spriteChainElement legsSprite, bodySprite, fullSprite;
	additionalSprites.clear();
	legsSprite.offset = Vector2f(this->textureBoxOffset);
	legsSprite.size = Vector2f(this->conditionalSizeUnits);
	bodySprite.offset = Vector2f(this->textureBoxOffset);
	bodySprite.size = Vector2f(this->conditionalSizeUnits);
	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);

	switch (currentAction)
	{
	case upFlap:
	{
		animationLength = 10;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/flap/upFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/flap/upFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case leftFlap:
	{
		animationLength = 10;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/flap/leftFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/flap/leftFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case rightFlap:
	{
		animationLength = 10;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/flap/rightFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/flap/rightFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case startFlap:
	{
		animationLength = 3;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/startFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/startFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case stopFlap:
	{
		animationLength = 3;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/stopFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/stopFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case relax:
	{
		animationLength = 1;
		animationSpeed = 0.0005f;
		bodySprite.path = "Game/worldSprites/owlBoss/stand/down/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		bodySprite.path = "Game/worldSprites/owlBoss/stand/down/1.png";
		currentSprite[0] = 1;
	}
	}

	if (currentAction == move)
	{
		animationLength = 12;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/jerk/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/jerk/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
	}

	additionalSprites.push_back(bodySprite);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 40 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			lastAction = currentAction;
			currentSprite[0] = 1;
		}
	}*/
}