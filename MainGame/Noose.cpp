#include "Noose.h"
#include "Deer.h"
#include "Deerchant.h"

using namespace sf;

Noose::Noose(const std::string& objectName, Vector2f centerPosition, WorldObject* owner) : DynamicObject(objectName, centerPosition)
{
	conditionalSizeUnits = { 360, 300 };
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	this->owner = owner;
	defaultSpeed = 0.0007f;
	speed = defaultSpeed;
	animationSpeed = float(5e-4);
	animationLength = 8;
	radius = 50;
	currentAction = move;
	routeGenerationAbility = false;
	canCrashIntoDynamic = false;
	Noose::jerk(2, 1);
	toSaveName = "noose";
	tag = Tag::noose;
}

Noose::~Noose()
= default;

Vector2f Noose::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;	
	ropeElongation = textureBox.width / 20.0f;
	//ropeElongation = 0;
	return { 0.0f, textureBox.height / 1.8f };
}

void Noose::setTarget(DynamicObject& object)
{
	return;
}

void Noose::behaviorWithDynamic(DynamicObject* target, long long elapsedTime)
{
	if (currentAction == dead)
		return;

	if (target->tag == Tag::deer)
	{		
		auto deer = dynamic_cast<Deer*>(target);		

		if (Helper::getDist(position, deer->getHeadPosition()) <= radius + target->getRadius())
		{
			boundTarget = target;
			if (deer->getOwner() == nullptr && deer->getCurrentAction() != commonHit)
				deer->changeAction(commonHit, true, false);
			if (owner != nullptr)
				deer->setOwner(owner);
		}
	}

    return;
}

void Noose::behaviorWithStatic(WorldObject* target, long long elapsedTime)
{
	if (currentAction == dead)
		return;
}

void Noose::behavior(long long elapsedTime)
{
	if (currentAction == dead)
	{
		zCoord = 0;
		return;
	}
	zCoord = 2;

	endingPreviousAction();
	jerkInteract(elapsedTime);

	if (owner != nullptr)
	{
		ownerPos = Vector2f(owner->getPosition().x + owner->getConditionalSizeUnits().x / 10.0f, owner->getPosition().y - owner->getConditionalSizeUnits().y / 13.0f);
		ownerGlobalBounds = owner->getConditionalSizeUnits();
	}

	if (boundTarget != nullptr && owner != nullptr)
		if (Helper::getDist(position, owner->getPosition()) >= maximumLength)
		{
			auto deer = dynamic_cast<Deer*>(boundTarget);
			if (deer)
				deer->setOwner(nullptr);
			stopping(true, true);			
			changeAction(dead, true, false);			
			owner = nullptr;		
		}

	if (boundTarget != nullptr)
	{
		auto deer = dynamic_cast<Deer*>(boundTarget);
		position = deer->getHeadPosition();
		speed = 0;
		changeAction(relax, false, true);
	}
}

Vector2f Noose::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Noose::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Noose::stopping(bool doStand, bool forgetBoundTarget)
{
	if (doStand)
	{
		this->movePosition = { -1, -1 };
		this->direction = Direction::STAND;
	}

	if (forgetBoundTarget && boundTarget != nullptr)
	{
		boundTarget->isProcessed = false;
		boundTarget = nullptr;
	}
}

void Noose::endingPreviousAction()
{
	if (lastAction == jerking)	
		currentAction = dead;
	lastAction = relax;
}

void Noose::jerkInteract(long long elapsedTime)
{
	if (currentAction == jerking)
	{
		if (jerkTime > 0)
		{
			jerkTime -= elapsedTime;
			speed = float(jerkDistance / float(jerkDuration) * jerkPower * pow(jerkTime / jerkDuration, jerkDeceleration));
			speed = std::max(defaultSpeed / jerkDeceleration, speed);
		}
		else
			speed = 0;
	}
}

void Noose::jerk(float power, const float deceleration, Vector2f destinationPoint)
{
	stopping(false, false);
	this->jerkPower = power;
	this->jerkDeceleration = deceleration;
	this->jerkDuration = long(40.0f / animationSpeed * 13.0f);
	this->jerkTime = this->jerkDuration;
	currentAction = jerking;
	jerkDistance = 1400;
	currentSprite[0] = 1;

	const Vector2f mousePos = Vector2f(Mouse::getPosition());
	const Vector2f screenCenter = Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2);
	const float coeff = jerkDistance / Helper::getDist(mousePos, screenCenter);
	laxMovePosition = Vector2f(owner->getPosition().x + (mousePos.x - screenCenter.x) * coeff, owner->getPosition().y + (mousePos.y - screenCenter.y) * coeff);
}

void Noose::fightInteract(long long elapsedTime, DynamicObject* target)
{
}

void Noose::rotateAndExtend(SpriteChainElement* rope, SpriteChainElement* loop) const
{
	if (ownerPos != Vector2f(0, 0))
	{
		const auto beginPoint = Vector2f(ownerPos.x, ownerPos.y - ownerGlobalBounds.y / 13.0f);
		rope->size = Vector2f(Helper::getDist(beginPoint, position) + ropeElongation, rope->size.y); // a little bit longer rope for sprite joining		
		if (position.y <= beginPoint.y)
			rope->rotation = acos((beginPoint.x - position.x) / sqrt(pow(beginPoint.x - position.x, 2) + pow(beginPoint.y - position.y, 2))) / pi * 180;
		else
			rope->rotation = -acos((beginPoint.x - position.x) / sqrt(pow(beginPoint.x - position.x, 2) + pow(beginPoint.y - position.y, 2))) / pi * 180;
	}

	if (currentAction != relax)
	{
		loop->rotation = rope->rotation + 180;
		loop->offset.x -= sin(loop->rotation / 180 * pi) * (textureBoxOffset.y); // rotational position correction
		loop->offset.y -= (1 - cos(loop->rotation / 180 * pi)) * (textureBoxOffset.y);
	}

	if (ownerPos != Vector2f(0, 0))
	{
		rope->offset.x += (ownerPos.x - position.x) * ropeElongation / Helper::getDist(position, ownerPos); // offset of the extended rope
		rope->offset.y += (ownerPos.y - position.y) * ropeElongation / Helper::getDist(position, ownerPos);

		// change position to hero belt
		const auto dynOwner = dynamic_cast<Deerchant*>(owner);
		if (dynOwner && currentAction != dead)
		{
			rope->position = dynOwner->getBeltPosition();
			rope->offset = Vector2f(rope->offset.x + rope->position.x - position.x, rope->offset.y + rope->position.y - position.y);
		}
	}
}

std::vector<SpriteChainElement*> Noose::prepareSprites(long long elapsedTime)
{
	std::vector<SpriteChainElement*> result = {};
	auto ropeSprite = new SpriteChainElement(PackTag::craftObjects, PackPart::noose, Direction::DOWN, 3, position, { conditionalSizeUnits.x, 30 }, { 0, 0 }, color);
	auto loopSprite = new SpriteChainElement(PackTag::craftObjects, PackPart::noose, Direction::UP, 1, position, conditionalSizeUnits, textureBoxOffset, color);

	switch (currentAction)
	{
		case relax:
		{
			animationLength = 13;
			animationSpeed = 0.0005f;
			ropeSprite->number = 2;
			ropeSprite->size.y = 60;
			if (owner != nullptr)
				if (owner->getPosition().x < position.x)
					ropeSprite->size.y *= -1;
			loopSprite->number = 1;
			loopSprite->size = Vector2f(40, 30);
			loopSprite->offset = Vector2f(float(loopSprite->size.x)*getScaleRatio().x / 1.8f, float(loopSprite->size.y)*getScaleRatio().y / 1.8f);
			break;
		}
		case jerking:
		{
			animationLength = 13;
			animationSpeed = 0.0005f;
			ropeSprite->number = 3;			
			break;
		}
		case dead:
		{
			animationLength = 1;
			animationSpeed = 0.0005f;

			ropeSprite->number = 3;
			loopSprite->number = 12;			

			rotateAndExtend(ropeSprite, loopSprite);

			ropeSprite->isBackground = true;
			loopSprite->isBackground = true;
			isBackground = true;

			result.push_back(ropeSprite);
			result.push_back(loopSprite);
			return result;
		}
	default:;
	}

	if (currentAction == move)
	{
		animationLength = 13;
		animationSpeed = 0.0005f;
		ropeSprite->number = 3;
	}

	loopSprite->number = currentSprite[0];

	rotateAndExtend(ropeSprite, loopSprite);

	result.push_back(ropeSprite);
    result.push_back(loopSprite);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= long(40.0f / animationSpeed))
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			lastAction = currentAction;	
			currentSprite[0] = 1;
		}
	}

	setUnscaled(result);
	return result;
}