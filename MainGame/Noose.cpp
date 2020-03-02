#include "Noose.h"

#include "deer.h"
#include "Deerchant.h"


using namespace sf;

Noose::Noose(const std::string& objectName, const Vector2f centerPosition, WorldObject* owner) : DynamicObject(objectName, centerPosition)
{
	conditionalSizeUnits = {360, 300};
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	this->owner = owner;
	moveSystem.defaultSpeed = 0.0007F;
	moveSystem.speed = moveSystem.defaultSpeed;
	animationSpeed = float(5e-4);
	animationLength = 8;
	radius = 50;
	currentAction = move;
	routeGenerationAbility = false;
	moveSystem.canCrashIntoDynamic = false;
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
	ropeElongation = textureBox.width / 20.0F;
	//ropeElongation = 0;
	return {0.0F, textureBox.height / 1.8F};
}

void Noose::setTarget(DynamicObject& object)
{
}

void Noose::behaviorWithDynamic(DynamicObject* target, long long /*elapsedTime*/)
{
	if (currentAction == dead)
	{
		return;
	}

	if (target->tag == Tag::deer)
	{
		auto deer = dynamic_cast<::deer*>(target);

		if (Helper::getDist(position, deer->getHeadPosition()) <= radius + target->getRadius())
		{
			boundTarget = target;
			if (deer->getOwner() == nullptr && deer->getCurrentAction() != commonHit)
			{
				deer->changeAction(commonHit, true, false);
			}
			if (owner != nullptr)
			{
				deer->setOwner(owner);
			}
		}
	}
}

void Noose::behaviorWithStatic(WorldObject* /*target*/, long long /*elapsedTime*/)
{
	if (currentAction == dead)
	{
		return;
	}
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
		ownerPos = Vector2f(owner->getPosition().x + owner->getConditionalSizeUnits().x / 10.0F, owner->getPosition().y - owner->getConditionalSizeUnits().y / 13.0F);
		ownerGlobalBounds = owner->getConditionalSizeUnits();
	}

	if (boundTarget != nullptr && owner != nullptr)
	{
		if (Helper::getDist(position, owner->getPosition()) >= maximumLength)
		{
			auto deer = dynamic_cast<::deer*>(boundTarget);
			if (deer != nullptr)
			{
				deer->setOwner(nullptr);
			}
			stopping(true, true);
			changeAction(dead, true, false);
			owner = nullptr;
		}
	}

	if (boundTarget != nullptr)
	{
		auto deer = dynamic_cast<::deer*>(boundTarget);
		position = deer->getHeadPosition();
		moveSystem.speed = 0;
		changeAction(relax, false, true);
	}
}

Vector2f Noose::getBuildPosition(std::vector<WorldObject*> /*visibleItems*/, float /*scaleFactor*/, Vector2f /*cameraPosition*/)
{
	return {-1, -1};
}

int Noose::getBuildType(Vector2f /*ounPos*/, Vector2f /*otherPos*/)
{
	return 1;
}

void Noose::stopping(bool doStand, bool forgetBoundTarget)
{
	if (doStand)
	{
		this->movePosition = {-1, -1};
		directionSystem.direction = Direction::STAND;
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
	{
		currentAction = dead;
	}
	lastAction = relax;
}

void Noose::jerkInteract(const long long elapsedTime)
{
	if (currentAction == jerking)
	{
		if (jerkTime > 0)
		{
			jerkTime -= elapsedTime;
			moveSystem.speed = jerkDistance / float(jerkDuration) * jerkPower * float(pow(jerkTime / jerkDuration, jerkDeceleration));
			moveSystem.speed = std::max(moveSystem.defaultSpeed / jerkDeceleration, moveSystem.speed);
		}
		else
		{
			moveSystem.speed = 0;
		}
	}
}

void Noose::jerk(const float power, const float deceleration, Vector2f /*destinationPoint*/)
{
	stopping(false, false);
	this->jerkPower = power;
	this->jerkDeceleration = deceleration;
	this->jerkDuration = long(40.0F / animationSpeed * 13.0F);
	this->jerkTime = this->jerkDuration;
	currentAction = jerking;
	jerkDistance = 1400;
	currentSprite[0] = 1;

	const auto mousePos = Vector2f(Mouse::getPosition());
	const auto screenCenter = Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2);
	const auto coeff = jerkDistance / Helper::getDist(mousePos, screenCenter);
	laxMovePosition = Vector2f(owner->getPosition().x + (mousePos.x - screenCenter.x) * coeff, owner->getPosition().y + (mousePos.y - screenCenter.y) * coeff);
}

void Noose::fightInteract(long long elapsedTime, DynamicObject* target)
{
}

void Noose::rotateAndExtend(SpriteChainElement* rope, SpriteChainElement* loop) const
{
	auto localElongation = ropeElongation;
	if (currentAction == relax)
	{
		localElongation = 1;
	}

	if (ownerPos != Vector2f(0, 0))
	{
		const auto beginPoint = Vector2f(ownerPos.x, ownerPos.y - ownerGlobalBounds.y / 13.0F);
		rope->size = Vector2f(Helper::getDist(beginPoint, position) + localElongation, rope->size.y); // a little bit longer rope for sprite joining		
		if (position.y <= beginPoint.y)
		{
			rope->rotation = acos((beginPoint.x - position.x) / sqrt(pow(beginPoint.x - position.x, 2) + pow(beginPoint.y - position.y, 2))) / pi * 180;
		}
		else
		{
			rope->rotation = -acos((beginPoint.x - position.x) / sqrt(pow(beginPoint.x - position.x, 2) + pow(beginPoint.y - position.y, 2))) / pi * 180;
		}
	}

	if (currentAction != relax)
	{
		loop->rotation = rope->rotation + 180;
		loop->offset.x -= sin(loop->rotation / 180 * pi) * textureBoxOffset.y; // rotational position correction
		loop->offset.y -= (1 - cos(loop->rotation / 180 * pi)) * textureBoxOffset.y;
	}

	if (ownerPos != Vector2f(0, 0))
	{
		rope->offset.x += (ownerPos.x - position.x) * localElongation / Helper::getDist(position, ownerPos); // offset of the extended rope
		rope->offset.y += (ownerPos.y - position.y) * localElongation / Helper::getDist(position, ownerPos);

		// change position to hero belt
		const auto dynOwner = dynamic_cast<Deerchant*>(owner);
		if ((dynOwner != nullptr) && currentAction != dead)
		{
			rope->position = dynOwner->getBeltPosition();
			rope->offset = Vector2f(rope->offset.x + rope->position.x - position.x, rope->offset.y + rope->position.y - position.y);
		}
	}
}

std::vector<SpriteChainElement*> Noose::prepareSprites(long long elapsedTime)
{
	std::vector<SpriteChainElement*> result = {};
	auto ropeSprite = new SpriteChainElement(PackTag::craftObjects, PackPart::noose, Direction::DOWN, 3, position, {conditionalSizeUnits.x, 30}, {0, 0}, color);
	auto loopSprite = new SpriteChainElement(PackTag::craftObjects, PackPart::noose, Direction::UP, 1, position, conditionalSizeUnits, textureBoxOffset, color);

	loopSprite->number = currentSprite[0];

	switch (currentAction)
	{
	case relax:
		{
			currentSprite[0] = 1;
			animationLength = 1;
			animationSpeed = 0.0005F;
			ropeSprite->number = 1;
			ropeSprite->size.y = 60;
			if (owner != nullptr)
			{
				if (owner->getPosition().x < position.x)
				{
					ropeSprite->size.y *= -1;
				}
			}
			loopSprite->number = 2;
			loopSprite->direction = Direction::DOWN;
			loopSprite->size = Vector2f(40, 30);
			loopSprite->offset = Vector2f(loopSprite->size.x / 1.8F, loopSprite->size.y / 1.8F);
			break;
		}
	case jerking:
		{
			animationLength = 13;
			animationSpeed = 0.0005F;
			ropeSprite->number = 3;
			break;
		}
	case dead:
		{
			animationLength = 1;
			animationSpeed = 0.0005F;

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
	default: ;
	}

	if (currentAction == move)
	{
		animationLength = 13;
		animationSpeed = 0.0005F;
		ropeSprite->number = 3;
	}

	rotateAndExtend(ropeSprite, loopSprite);

	result.push_back(ropeSprite);
	result.push_back(loopSprite);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= long(40.0F / animationSpeed))
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
