/*
#include "ClapWhirl.h"
#include "Deerchant.h"
#include "nightmare_third.h"

ClapWhirl::ClapWhirl(const std::string objectName, sf::Vector2f centerPosition, WorldObject* owner) : DynamicObject(objectName, centerPosition)
{
	conditionalSizeUnits = {250, 500);
	startPos = owner->getPosition();
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	this->owner = owner;
	defaultSpeed = 0.00055f;	
	strength = 15;
	animationSpeed = 10;
	animationLength = 8;
	radius = 120;
	currentAction = relax;
	routeGenerationAbility = false;
	canCrashIntoDynamic = false;
	jerk(2, 1);
	toSaveName = "clapWhirl";
	tag = Tag::clapWhirl;
}

ClapWhirl::~ClapWhirl()
{
}

Vector2i ClapWhirl::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return Vector2i(0, textureBox.height / 2));
}

void ClapWhirl::setTarget(DynamicObject& object)
{
	return;
}

void ClapWhirl::behaviorWithDynamic(DynamicObject* target, long long elapsedTime)
{
	if (currentAction == dead)
		return;	

	if (target->tag == Tag::hero)
	{		
		if (Helper::getDist(position, target->getPosition()) <= radius + target->getRadius() && color.a >= 40)
		{
			target->takeDamage(strength, position);
			deletePromiseOn();
		}
	}

	return;
}

void ClapWhirl::behaviorWithStatic(WorldObject* target, long long elapsedTime)
{
	if (currentAction == dead)
		return;
}

void ClapWhirl::behavior(long long elapsedTime)
{
	if (currentAction == dead)
	{
		stopping(true, true);
		color.a = 0;
	}

	endingPreviousAction();
	jerkInteract(elapsedTime);

	if (owner != nullptr)
	{
		ownerPos = sf::Vector2f(owner->getPosition().x + 0, owner->getPosition().y - 0);
		ownerGlobalBounds = owner->getConditionalSizeUnits();
	}

	if (currentAction == jerking)
		color.a = 255 * jerkTime / jerkDuration;
	else
		color.a = 0;
}	

Vector2f ClapWhirl::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition)
{
	return { -1, -1 };
}

int ClapWhirl::getBuildType(sf::Vector2f ounPos, sf::Vector2f otherPos)
{
	return 1;
}

void ClapWhirl::stopping(bool doStand, bool forgetBoundTarget)
{
	if (doStand)
	{
		this->move_position_ = { -1, -1 };
		this->direction = Direction::STAND;
	}

	if (forgetBoundTarget && boundTarget != nullptr)
	{
		boundTarget->isProcessed = false;
		boundTarget = nullptr;
	}
}

void ClapWhirl::endingPreviousAction()
{
	if (lastAction == jerking)
	{
		currentAction = dead;
		deletePromiseOn();
	}
	if (lastAction == dead)
		deletePromiseOn();
	lastAction = relax;
}

void ClapWhirl::jerkInteract(long long elapsedTime)
{
	if (currentAction == jerking)
	{
		if (jerkTime > 0)
		{
			jerkTime -= elapsedTime;
			speed = Helper::getDist(startPos, laxMovePosition) / jerkDuration;
		}
		else
		{
			stopping(true, true);
			deletePromiseOn();
		}
	}
}

void ClapWhirl::jerk(float power, float deceleration, sf::Vector2f destinationPoint)
{
	stopping(false, false);
	this->jerkPower = power;
	this->jerkDeceleration = deceleration;
	this->jerkDuration = 7e5;
	this->jerkTime = this->jerkDuration;
	currentAction = jerking;
	jerkDistance = 1400;
	currentSprite[0] = 1;

	const sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition());
	const sf::Vector2f screenCenter = sf::Vector2f(world_metrics::window_size.x / 2, world_metrics::window_size.y / 2);	

	auto nightmare = dynamic_cast<Nightmare3*>(owner);
	if (nightmare && nightmare->getBoundTarget())
	{
		sf::Vector2f flightVector = sf::Vector2f(nightmare->getBoundTarget()->getPosition().x - nightmare->getPosition().x,
			nightmare->getBoundTarget()->getPosition().y - nightmare->getPosition().y);
		float k = jerkDistance / sqrt(pow(flightVector.x, 2) + pow(flightVector.y, 2));
		laxMovePosition = sf::Vector2f(owner->getPosition().x + flightVector.x * k, owner->getPosition().y + flightVector.y * k);
	}
	else
		laxMovePosition = { 0, 0 };
}

void ClapWhirl::fightInteract(long long elapsedTime, DynamicObject* target)
{
	return;
}

void ClapWhirl::rotateAndExtend(spriteChainElement* whirl)
{
	if (ownerPos != sf::Vector2f(0, 0))
	{
		if (position.y <= startPos.y)
			whirl->rotation = acos((startPos.x - position.x) / sqrt(pow(startPos.x - position.x, 2) + pow(startPos.y - position.y, 2))) / pi * 180;
		else
			whirl->rotation = -acos((startPos.x - position.x) / sqrt(pow(startPos.x - position.x, 2) + pow(startPos.y - position.y, 2))) / pi * 180;
	}

	if (currentAction != relax)
	{
		whirl->rotation = whirl->rotation + 180;
		whirl->offset.x -= sin(whirl->rotation / 180 * pi) * (textureBoxOffset.y); // rotational position correction
		whirl->offset.y -= (1 - cos(whirl->rotation / 180 * pi)) * (textureBoxOffset.y);
	}
}

void ClapWhirl::prepare_sprites(long long elapsedTime)
{
	spriteChainElement whirlSprite;

	whirlSprite.offset = sf::Vector2f(this->textureBoxOffset);
	whirlSprite.size = sf::Vector2f(this->conditionalSizeUnits);
	additionalSprites.clear();
	animationSpeed = 10;

	switch (currentAction)
	{
	case relax:
	{
		animationLength = 13;
		whirlSprite.path = "Game/worldSprites/nightmare3/clap/whirl.png";
		//deletePromiseOn();
		break;
	}
	case jerking:
	{
		animationLength = 13;
		//whirlSprite.path = "Game/worldSprites/noose/nooseLoop/" + std::to_string(currentSprite[0]) + ".png";
		whirlSprite.path = "Game/worldSprites/nightmare3/clap/whirl.png";
		break;
	}
	case dead:
	{
		animationLength = 5;		
		whirlSprite.path = "Game/worldSprites/nightmare3/clap/whirlBroken.png";
		break;
	}
	default:;
	}

	if (currentAction == move)
	{
		animationLength = 13;	
		//whirlSprite.path = "Game/worldSprites/noose/nooseLoop/" + std::to_string(currentSprite[0]) + ".png";
		whirlSprite.path = "Game/worldSprites/nightmare3/clap/whirl.png";
	}

	rotateAndExtend(&whirlSprite);
	additionalSprites.push_back(whirlSprite);
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
}
*/