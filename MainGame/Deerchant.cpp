#include "Deerchant.h"
#include "Helper.h"
#include "PickedObject.h"
#include "Noose.h"
#include "Brazier.h"
#include "EmptyObject.h"

using namespace sf;

Deerchant::Deerchant(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	currentSprite.resize(3);
	for (auto& number : currentSprite)
		number = 1;	
	timeForNewSprite = 0;
	defaultSpeed = 0.0006f;
	speed = defaultSpeed;
	animationSpeed = 0.0010f;
	animationLength = 8;	
	radius = 50;
	hitDistance = 50;
	strikingSprite = 6;
	strength = 10;
	maxHealthPointValue = 100;
	healthPoint = maxHealthPointValue;
	energy = 50; maxEnergyValue = 100;
	currentAction = relax;
	toSaveName = "this1";
	tag = Tag::hero1;
	canCrashIntoStatic = true;

	for (int i = 0; i < 3; i++)
		bags.push_back((*new HeroBag));
	const Vector2f openBagSize = Vector2f(Helper::GetScreenSize().x / 6, Helper::GetScreenSize().y / 3),
		closedBagSize = Vector2f(Helper::GetScreenSize().x / 12, Helper::GetScreenSize().y / 6);
	bags[0].initialize(Vector2f(Helper::GetScreenSize().x - closedBagSize.x, closedBagSize.y), true);
	bags[1].initialize(Vector2f(Helper::GetScreenSize().x - closedBagSize.x, 2 * closedBagSize.y), true);
	bags[2].initialize(Vector2f(Helper::GetScreenSize().x - closedBagSize.x * 1.5f, 1.5 * closedBagSize.y), true);
}

Deerchant::~Deerchant()
{

}

Vector2i Deerchant::calculateTextureOffset()
{
	conditionalSizeUnits = Vector2i(375, 375);
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, textureBox.height * 4 / 5);
}

void Deerchant::handleInput(bool usedMouse)
{	
	//high-priority actions
	if (currentAction == absorbs || currentAction == grab || currentAction == dropping || currentAction == builds || currentAction == jerking)
		return;

	//if (currentAction == throwNoose && heldItem->content != std::make_pair(Tag::noose, 1))
		//changeAction(relax, true, false);

	if (Keyboard::isKeyPressed(Keyboard::Space) && currentAction != jerking && direction != Direction::STAND)
	{
		jerk(3, 2);
		return;
	}
	//---------------------

	setHitDirection();
	moveOffset = Vector2f(-1, -1);
	direction = Direction::STAND;
	if (Keyboard::isKeyPressed(Keyboard::A))					
		direction = Direction::LEFT;					
	if (Keyboard::isKeyPressed(Keyboard::D))						
		direction = Direction::RIGHT;						
	if (Keyboard::isKeyPressed(Keyboard::W))							
		direction = Direction::UP;							
	if (Keyboard::isKeyPressed(Keyboard::S))								
		direction = Direction::DOWN;
	if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::W))
		direction = Direction::UPLEFT;
	if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::W))
		direction = Direction::UPRIGHT;
	if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::S))
		direction = Direction::DOWNLEFT;
	if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S))
		direction = Direction::DOWNRIGHT;
	if (direction != Direction::STAND)
	{
		if (direction != lastDirection)
		{
			speedLineDirection = getSpeedLineDirection(lastDirection, direction);
			currentSprite[2] = 1;
		}
		lastDirection = direction;
	}

	if (currentAction != throwNoose) //second priority actions, interact while moving
	{
		if (!(Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S)))
		{
			bool isIntersect = false;
			if (boundTarget)
				isIntersect = (sqrt(pow(this->position.x - laxMovePosition.x, 2) + pow(this->position.y - laxMovePosition.y, 2)) <= (this->radius + boundTarget->getRadius()));
			if (isIntersect || !boundTarget)
			{
				direction = Direction::STAND;
				if (currentAction == move)
					changeAction(relax, true, false);
			}
			else
			{
				if (boundTarget && Helper::getDist(position, laxMovePosition) > (this->radius + boundTarget->getRadius()))
				{
					direction = calculateDirection();
					setMoveOffset(0);
					changeAction(move, currentAction != move, false);
				}
			}
		}
	}
	else
		return;

	//define actions
	if (Mouse::isButtonPressed(Mouse::Left) && heldItem->content.first == Tag::noose)
	{
		changeAction(throwNoose, true, false);
		return;
	}

	if (direction != Direction::STAND && currentAction != moveHit && !Mouse::isButtonPressed(Mouse::Left))
		changeAction(move, currentAction == relax, currentAction != move);

	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S) ||
		Keyboard::isKeyPressed(Keyboard::Z) || Keyboard::isKeyPressed(Keyboard::F) || Keyboard::isKeyPressed(Keyboard::E) || Keyboard::isKeyPressed(Keyboard::LControl) ||
		Keyboard::isKeyPressed(Keyboard::Space) || Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
	{
		if (boundTarget != nullptr)
			stopping(true, true);
		if (!Mouse::isButtonPressed(Mouse::Left))
			stopping(false, false, true);
	}
	//--------------

	if (isBuildSystemMaker)
		return;
	if (Keyboard::isKeyPressed(Keyboard::Z) && (currentAction == relax))
	{
		changeAction(transitionToEnotherWorld, true, false);
		currentSprite[0] = 1;
	}
	else
		if (Mouse::isButtonPressed(Mouse::Left) && !usedMouse)
		{
			if (direction != Direction::STAND)			
				changeAction(moveHit, !(currentAction == moveHit || currentAction == commonHit), false);			
			else
				changeAction(commonHit, !(currentAction == moveHit || currentAction == commonHit), false);			
		}
}

void Deerchant::changeAction(Actions newAction, bool resetSpriteNumber, bool rememberLastAction)
{
	if (rememberLastAction)
		lastAction = currentAction;

	currentAction = newAction;

	if (resetSpriteNumber)
		for (auto& number : currentSprite)
			number = 1;
}

void Deerchant::setHitDirection()
{
	auto screenSize = Helper::GetScreenSize();
	float xPos = screenSize.x / 2, yPos = screenSize.y / 2, mouseX = Mouse::getPosition().x, mouseY = Mouse::getPosition().y;
	float alpha = atan((yPos - mouseY) / (xPos - mouseX)) * 180 / pi;
	if (mouseY <= yPos && abs(alpha) >= 45 && abs(alpha) <= 90)
		side = up;
	else
		if (mouseX >= xPos && abs(alpha) >= 0 && abs(alpha) <= 45)
			side = right;
		else
			if (mouseY >= yPos && abs(alpha) >= 45 && abs(alpha) <= 90)
				side = down;
			else
				if (mouseX <= xPos && abs(alpha) >= 0 && abs(alpha) <= 45)
					side = left;	
}

void Deerchant::setTarget(DynamicObject& object)
{
	nearTheTable = false;
	return;
}

void Deerchant::behaviorWithDynamic(DynamicObject* target, float elapsedTime)
{
	if (Helper::getDist(position, target->getPosition()) <= radius + target->getRadius())
		pushByBumping(target);

	const bool isIntersect = Helper::getDist(position, target->getPosition()) <= this->radius + target->getRadius() + hitDistance;

	if (isIntersect && calculateSide(target->getPosition(), elapsedTime) != invertSide(side))
	{
		if ((this->currentAction == commonHit || this->currentAction == moveHit) && (this->getSpriteNumber() == 4 || this->getSpriteNumber() == 5 || this->getSpriteNumber() == 8))
		{
			this->addEnergy(5);
			target->takeDamage(this->getStrength(), position);
		}
	}	
}

void Deerchant::behaviorWithStatic(WorldObject* target, float elapsedTime)
{	
	if (target->tag == Tag::wreathTable && Helper::getDist(position, target->getPosition()) <= radius + target->getRadius())
		nearTheTable = true;	
}

void Deerchant::behavior(float elapsedTime)
{		
	jerkInteract(elapsedTime);
	endingPreviousAction();
	fightLogic(elapsedTime);

	if (!boundTarget || boundTarget->isProcessed)
	{
		laxMovePosition = Vector2f(-1, -1);
		return;
	}

	if (currentAction != jerking && boundTarget->isProcessed)
		laxMovePosition = boundTarget->getPosition();

	bool isIntersect = (Helper::getDist(position, laxMovePosition)) <= (this->radius + boundTarget->getRadius());

	//touch selected object 
	if (isIntersect)
	{
		boundTarget->isProcessed = true;
		switch (boundTarget->tag)
		{
		case Tag::tree:
		{
			if (boundTarget->getState() == absorbed)
				break;
			changeAction(absorbs, true, false);
			currentSprite[0] = 1;
			side = calculateSide(boundTarget->getPosition(), elapsedTime);
			//direction = calculateDirection();
			boundTarget->setState(absorbed);
			boundTarget->isProcessed = false;
			stopping(true);
			break;
		}
		case Tag::fern:
		{
			if (!boundTarget->inventory.empty())
			{
				changeAction(open, true, false);
				unsealInventoryOwner = boundTarget;
				boundTarget->isProcessed = false;
				stopping(true, true);
			}
			else
			{
				changeAction(grab, true, false);
				stopping(true);
			}
			
			break;
		}
		case Tag::chamomile:
		case Tag::yarrow:		
		case Tag::mugwort:
		case Tag::noose:
		case Tag::droppedLoot:
		case Tag::hareTrap:
		case Tag::heroBag:
		{
			changeAction(grab, true, false);
			side = calculateSide(boundTarget->getPosition(), elapsedTime);
			stopping(true);
			break;
		}
		case Tag::dropPoint:
		{		
			changeAction(dropping, true, false);
			side = calculateSide(boundTarget->getPosition(), elapsedTime);
			boundTarget->isProcessed = false;
			stopping(true, true);
			break;
		}
		case Tag::buildObject:
		{
			changeAction(builds, true, false);
			side = calculateSide(boundTarget->getPosition(), elapsedTime);
			boundTarget->isProcessed = false;
			stopping(true, true);
			break;
		}
		default:
		{
			changeAction(relax, true, false);
			if (boundTarget)
			{
				boundTarget->isProcessed = false;
				side = calculateSide(boundTarget->getPosition(), elapsedTime);
				stopping(true, true);
			}
			break;
		}
		}
	}
	else
	{
		boundTarget->isProcessed = false;
	}
	//----------------------------	
}

void Deerchant::onMouseUp(int currentMouseButton, WorldObject *mouseSelectedObject, Vector2f mouseWorldPos, bool isBuilding)
{
	if (isBuilding && currentMouseButton == 2)
	{
		if (boundTarget != nullptr)
		{
			boundTarget->isProcessed = false;
			boundTarget = nullptr;			
		}
		boundTarget = new EmptyObject("buildItem", mouseWorldPos);
		boundTarget->tag = Tag::buildObject;
		laxMovePosition = mouseWorldPos;
		return;
	}

	if (mouseSelectedObject && mouseSelectedObject->tag == Tag::brazier)
	{
		auto brazier = dynamic_cast<Brazier*>(mouseSelectedObject);
		if (heldItem->content.first != Tag::emptyCell &&
			Helper::getDist(brazier->getPlatePosition(), position) <= brazier->getPlateRadius() + radius)
		{
			brazier->putItemToCraft(heldItem->content.first);
			heldItem->content = heldItem->content.second > 1
				? std::make_pair(heldItem->content.first, heldItem->content.second - 1)
				: std::make_pair(Tag::emptyCell, 0);
		}
		return;
	}

	if (!mouseSelectedObject)
	{
		if (heldItem->content.first != Tag::emptyCell && currentMouseButton == 2)
		{
			if (boundTarget != nullptr)
			{
				boundTarget->isProcessed = false;
				boundTarget = nullptr;				
			}
			boundTarget = new EmptyObject("droppedItem", mouseWorldPos);
			boundTarget->tag = Tag::dropPoint;
			laxMovePosition = mouseWorldPos;
			return;
		}

		for (auto& bag : bags)
		{
			if (bag.currentState == ejected)
			{
				if (boundTarget != nullptr)
				{
					boundTarget->isProcessed = false;
					boundTarget = nullptr;
				}
				boundTarget = new EmptyObject("droppedBag", mouseWorldPos);
				boundTarget->tag = Tag::dropPoint;
				laxMovePosition = mouseWorldPos;
			}
		}
		return;
	}	

	if (currentMouseButton == 2)
	{
		boundTarget = mouseSelectedObject;
		laxMovePosition = mouseSelectedObject->getPosition();
	}
}

void Deerchant::endingPreviousAction()
{
	if (lastAction == commonHit && !Mouse::isButtonPressed(Mouse::Left))
	{
		lastDirection = sideToDirection(side);
		changeAction(relax, true, false);
	}
	if (lastAction == moveHit && !Mouse::isButtonPressed(Mouse::Left))
	{
		lastDirection = sideToDirection(side);
		changeAction(relax, true, false);
	}
	if (lastAction == open)
		changeAction(relax, true, false);
	if (lastAction == absorbs)
	{
		lastDirection = sideToDirection(side);
		boundTarget->isProcessed = false;
		boundTarget = nullptr;
		changeAction(relax, true, false);		
	}
	if (lastAction == builds)
	{
		lastDirection = sideToDirection(side);
		changeAction(relax, true, false);
	}
	if (lastAction == dropping)
	{
		lastDirection = sideToDirection(side);
		stopping(true, true);
		changeAction(relax, true, false);
		if (heldItem->content.first != Tag::emptyCell)
		{
			birthStaticInfo dropObject;
			dropObject.position = { position.x, position.y + radius };
			dropObject.tag = Tag::droppedLoot;
			dropObject.typeOfObject = int(heldItem->content.first);
			dropObject.count = heldItem->content.second;
			birthStatics.push(dropObject);
			heldItem->content = std::make_pair(Tag::emptyCell, 0);
		}
		else
		{
			for (int cnt = 0; cnt != bags.size(); cnt++)			
				if (bags[cnt].currentState == ejected)
				{
					bool isHareTrap = true;
					for (auto& item : bags[cnt].cells)					
						if (item.content.first != Tag::yarrow && item.content.first != Tag::emptyCell)
						{
							isHareTrap = false;
							break;
						}
					birthStaticInfo dropObject;
					if (isHareTrap)
					{											
						dropObject.tag = Tag::hareTrap;											
					}
					else
					{
						dropObject.tag = Tag::droppedLoot;
						dropObject.typeOfObject = int(Tag::heroBag);	
					}
					dropObject.position = { position.x, position.y + radius };
					dropObject.inventory = HeroBag::cellsToInventory(bags[cnt].cells);
					bags[cnt].cleanTextureReferences();
					//bags[cnt].~HeroBag();
					bags.erase(bags.begin() + cnt);
					birthStatics.push(dropObject);
					break;
				}	
		}
	}
	if (currentAction == throwNoose && currentSprite[0] == 12 && heldItem->content == std::make_pair(Tag::noose, 1))
	{
		heldItem->content = { Tag::emptyCell, 0 };
		birthDynamicInfo nooseObject;
		nooseObject.position = position;
		nooseObject.tag = Tag::noose;
		nooseObject.owner = this;
		birthDynamics.push(nooseObject);
	}
    if (lastAction == throwNoose)
    {
		lastDirection = sideToDirection(side);
		changeAction(relax, true, false);
    }
	if (lastAction == open)
	{
		lastDirection = sideToDirection(side);
		if (unsealInventoryOwner)
		{
			unsealInventoryOwner->isVisibleInventory = true;
			changeAction(relax, true, false);
		}
	}
	if (lastAction == grab)
	{
		lastDirection = sideToDirection(side);
		if (boundTarget)
		{
			auto pickedItem = dynamic_cast<PickedObject*>(boundTarget);
			if (pickedItem)
			{
				if (pickedItem->getType() == int(Tag::heroBag) || pickedItem->getId() == Tag::hareTrap)
				{
					const Vector2f openBagSize = Vector2f(Helper::GetScreenSize().x / 6, Helper::GetScreenSize().y / 3),
						closedBagSize = Vector2f(Helper::GetScreenSize().x / 12, Helper::GetScreenSize().y / 6);
					bags.resize(bags.size() + 1);
					bags[bags.size() - 1] = *(new HeroBag());
					bags[bags.size()-1].initialize(Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2), true, pickedItem->inventory);
					pickedItem->deletePromiseOn();
				}
				else
					pickedItem->pickUp(&this->bags);
			}
			
			/*auto nooseItem = dynamic_cast<Noose*>(boundTarget);
			if (nooseItem)
			{
				auto placedNoose = new std::vector<std::pair<Tag, int>>({ (std::make_pair(Tag::noose, 1)) });
				if (HeroBag::putItemsIn(placedNoose, &bags))
					nooseItem->deletePromiseOn();
				delete placedNoose;
			}*/
			stopping(true, true);
			changeAction(relax, true, false);
		}
	}
	lastAction = relax;
}

void Deerchant::jerkInteract(float elapsedTime)
{
	if (currentAction == jerking)
	{
		if (jerkTime > 0)
		{
			jerkTime -= elapsedTime;
			speed = (jerkDistance / jerkDuration) * jerkPower * pow(jerkTime / jerkDuration, jerkDeceleration);
			speed = std::max(defaultSpeed / jerkDeceleration, speed);
		}
		else
		{
			changeAction(relax, true, false);
			speed = defaultSpeed;
		}
	}
}

void Deerchant::stopping(bool doStand, bool forgetBoundTarget, bool offUnsealInventory)
{
	if (boundTarget != nullptr && currentAction != dropping)
		if (boundTarget->getName() == "droppedBag")
			for (auto& bag : bags)
				if (bag.currentState == ejected)
					bag.currentState = bagClosed;

	if (doStand)
	{
		this->laxMovePosition = { -1, -1 };
		moveOffset = { -1, -1 };
		this->direction = Direction::STAND;
	}

	if (forgetBoundTarget && boundTarget != nullptr)
	{
		boundTarget->isProcessed = false;
		boundTarget = nullptr;
	}

	if (offUnsealInventory)
	{
		if (unsealInventoryOwner != nullptr)
			unsealInventoryOwner->isVisibleInventory = false;
		unsealInventoryOwner = nullptr;
	}
}

Vector2f Deerchant::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

Vector2f Deerchant::getBeltPosition()
{
	/*if (additionalSprites.size() >= 2) return
		Vector2f((4 * additionalSprites[0].position.x + additionalSprites[1].position.x) / 5.0f + conditionalSizeUnits.x / 3.0f,
		(4 * additionalSprites[0].position.y + additionalSprites[1].position.y) / 5.0f);
	return additionalSprites[0].position;*/
    return position;
}

int Deerchant::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Deerchant::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	stopping(false, true);
	this->jerkPower = power;
	this->jerkDeceleration = deceleration;
	this->jerkDuration = 700000;
	this->jerkTime = this->jerkDuration;
	changeAction(jerking, currentAction != jerking, false);
	jerkDistance = 500;
	currentSprite[0] = 1;

	laxMovePosition = Vector2f(position.x + cos(int(direction) * pi / 180) * jerkDistance, position.y - sin(int(direction) * pi / 180) * jerkDistance);
}

void Deerchant::fightLogic(float elapsedTime, DynamicObject* target)
{
	pushAway(elapsedTime);
}

SpriteChainElement Deerchant::prepareSpeedLine(SpeedLineDirection direction)
{
	SpriteChainElement speedLine;
    return speedLine;
	/*bool isInverse = false;
	speedLine.offset = Vector2f(this->textureBoxOffset);
	speedLine.size = Vector2f(this->conditionalSizeUnits);
	speedLine.animationLength = 3;
	speedLine.path = "";
	if (direction == SpeedLineDirection::stand || currentSprite[2] > speedLine.animationLength)
		return speedLine;

	switch (direction)
	{
        case SpeedLineDirection::down_downleft:
		    speedLine.path = "Game/worldSprites/hero/move/lines/1/";
		    break;
         case SpeedLineDirection::down_downright:
            speedLine.mirrored = true;
		    speedLine.path = "Game/worldSprites/hero/move/lines/1/";
		    break;
        case SpeedLineDirection::downleft_left:       
        case SpeedLineDirection::upright_right:
	        speedLine.path = "Game/worldSprites/hero/move/lines/2/";
	        break;
        case SpeedLineDirection::upleft_left:
        case SpeedLineDirection::downright_right:
            speedLine.mirrored = true;
	        speedLine.path = "Game/worldSprites/hero/move/lines/2/";
	        break;
        case SpeedLineDirection::left_upleft:       
        case SpeedLineDirection::right_downright:       
	        speedLine.path = "Game/worldSprites/hero/move/lines/3/";
	    break;
        case SpeedLineDirection::left_downleft:
        case SpeedLineDirection::right_upright:
            speedLine.mirrored = true;
	        speedLine.path = "Game/worldSprites/hero/move/lines/3/";
	    break;        
        case SpeedLineDirection::downright_down:
        case SpeedLineDirection::upright_up:
            speedLine.mirrored = true;
	        speedLine.path = "Game/worldSprites/hero/move/lines/4/";
	        break;
        case SpeedLineDirection::upleft_up:
        case SpeedLineDirection::downleft_down:        
            speedLine.path = "Game/worldSprites/hero/move/lines/4/";
	        break;
        case SpeedLineDirection::up_upright:       
	        speedLine.path = "Game/worldSprites/hero/move/lines/5/";
	        break;
         case SpeedLineDirection::up_upleft:
            speedLine.mirrored = true;
            speedLine.path = "Game/worldSprites/hero/move/lines/5/";
	        break;
	    default:
		    speedLine.path = "";
		    return speedLine;		
	}

	if (!speedLine.path.empty())
	{
		speedLine.offset.y += conditionalSizeUnits.y / 9.0f;
		speedLine.position = Vector2f(position.x, position.y + conditionalSizeUnits.y / 9.0f);
		if (isInverse)
			speedLine.path += std::to_string(speedLine.animationLength + 1 - currentSprite[2]) + ".png";
		else
			speedLine.path += std::to_string(currentSprite[2]) + ".png";
		additionalSprites.push_back(speedLine);
	}
	return speedLine;*/
}

std::vector<SpriteChainElement> Deerchant::prepareSprites(long long elapsedTime)
{
	SpriteChainElement legsSprite, bodySprite, speedLine = prepareSpeedLine(speedLineDirection);
    std::vector<SpriteChainElement> result = {};
	bool isInverse = false;
	legsSprite.offset = Vector2f(this->textureBoxOffset);
	legsSprite.size = Vector2i(this->conditionalSizeUnits);
	legsSprite.animationLength = 8;
	bodySprite.offset = Vector2f(this->textureBoxOffset);
	bodySprite.size = Vector2i(this->conditionalSizeUnits);

    auto spriteSide = side; auto spriteDirection = direction;

	animationSpeed = 12;

	if (direction == Direction::RIGHT)
	{
		spriteDirection = Direction::LEFT;
		legsSprite.mirrored = true;
	}
	if (direction == Direction::UPRIGHT)
	{
		spriteDirection = Direction::UPLEFT;
		legsSprite.mirrored = true;
	}
	if (direction == Direction::DOWNRIGHT)
	{
		spriteDirection = Direction::DOWNLEFT;
		legsSprite.mirrored = true;
	}

	if (side == right)
	{
		spriteSide = left;
		bodySprite.mirrored = true;
	}

	switch (currentAction)
	{
	case commonHit:
		animationLength = 8;
		if (side == right)
			legsSprite.mirrored = true;
		bodySprite.packTag = PackTag::heroHit; bodySprite.packPart = PackPart::body; bodySprite.direction = sideToDirection(spriteSide);
		legsSprite.packTag = PackTag::heroHit; legsSprite.packPart = PackPart::legs; legsSprite.direction = sideToDirection(spriteSide);
		break;
	case absorbs:
		animationLength = 10;
		bodySprite.packTag = PackTag::heroAbsorb; bodySprite.packPart = PackPart::full; bodySprite.direction = sideToDirection(spriteSide);	
		break;
	case builds:
		animationLength = 10;
		bodySprite.packTag = PackTag::heroAbsorb; bodySprite.packPart = PackPart::full; bodySprite.direction = sideToDirection(spriteSide);	
		break;
	case grab:
		animationLength = 11;
		animationSpeed = 15;
		bodySprite.packTag = PackTag::heroPick; bodySprite.packPart = PackPart::full; bodySprite.direction = sideToDirection(spriteSide);
		break;
	case dropping:
		isInverse = true;
		animationLength = 8;
		bodySprite.packTag = PackTag::heroPick; bodySprite.packPart = PackPart::full; bodySprite.direction = sideToDirection(spriteSide);
		break;
	case transitionToEnotherWorld:
		animationLength = 18;
		break;
	case jerking:
		animationLength = 8;
		animationSpeed = 11;
		if (direction == Direction::RIGHT || direction == Direction::UPRIGHT || direction == Direction::DOWNRIGHT)
			bodySprite.mirrored = true;
		if (spriteDirection == Direction::UPLEFT || spriteDirection == Direction::DOWNLEFT)
			spriteDirection = Direction::LEFT;
		bodySprite.packTag = PackTag::heroRoll; bodySprite.packPart = PackPart::full; bodySprite.direction = cutRights(lastDirection);
		break;
	case open:
		animationLength = 12;
		bodySprite.packTag = PackTag::heroPick; bodySprite.packPart = PackPart::full; bodySprite.direction = sideToDirection(spriteSide);
		break;
	case relax:
		bodySprite.mirrored = false;
		animationLength = 16;
		animationSpeed = 13;
		if (lastDirection == Direction::RIGHT || lastDirection == Direction::UPRIGHT || lastDirection == Direction::DOWNRIGHT)		
			bodySprite.mirrored = true;
		bodySprite.packTag = PackTag::heroStand; bodySprite.packPart = PackPart::full; bodySprite.direction = cutRights(lastDirection);
		break;
	}

	if (currentAction == move)
	{
		animationLength = 8;
		if (direction == Direction::RIGHT || direction == Direction::UPRIGHT || direction == Direction::DOWNRIGHT)
			bodySprite.mirrored = true;
		bodySprite.packTag = PackTag::heroMove; bodySprite.packPart = PackPart::body; bodySprite.direction = spriteDirection;
		legsSprite.packTag = PackTag::heroMove; legsSprite.packPart = PackPart::legs; legsSprite.direction = spriteDirection;
	}

	if (currentAction == moveHit)
	{
		animationLength = 8;		

		if (direction == Direction::UP && side == down || direction == Direction::DOWN && side == up )
		{
			isInverse = true;
			spriteDirection = sideToDirection(spriteSide);		
		}
		if ((direction == Direction::LEFT || direction == Direction::UPLEFT || direction == Direction::DOWNLEFT) && side == right ||
			(direction == Direction::UP || direction == Direction::UPLEFT || direction == Direction::UPRIGHT) && side == down ||
			(direction == Direction::RIGHT || direction == Direction::UPRIGHT || direction == Direction::DOWNRIGHT) && side == left ||
			(direction == Direction::DOWN || direction == Direction::DOWNLEFT || direction == Direction::DOWNRIGHT) && side == up)
		{
			isInverse = true;
			legsSprite.mirrored = !legsSprite.mirrored;
			spriteDirection = invertDirection(spriteDirection);
		}

		legsSprite.packTag = PackTag::heroMove; legsSprite.packPart = PackPart::legs; legsSprite.direction = cutRights(spriteDirection);
		bodySprite.packTag = PackTag::heroHit; bodySprite.packPart = PackPart::body; bodySprite.direction = sideToDirection(spriteSide);

		if (direction == Direction::STAND)
		{
			if (side == right)
				legsSprite.mirrored = true;
			legsSprite.packTag = PackTag::heroMove; legsSprite.packPart = PackPart::legs; legsSprite.direction = sideToDirection(spriteSide);
			legsSprite.animationLength = 14;
		}
	}

	/*if (currentAction == throwNoose)
	{
		animationLength = 14;
		if (direction == Direction::UP && side == down || direction == Direction::DOWN && side == up)
		{
			isInverse = true;
			directionStr = sideStr;
		}
		if ((direction == Direction::LEFT || direction == Direction::UPLEFT || direction == Direction::DOWNLEFT) && side == right ||
			(direction == Direction::RIGHT || direction == Direction::UPRIGHT || direction == Direction::DOWNRIGHT) && side == left)
		{
			isInverse = true;
			legsSprite.mirrored = !legsSprite.mirrored;
		}
		legsSprite.path = "Game/worldSprites/hero/move/legs/" + directionStr + '/';
		bodySprite.path = "Game/worldSprites/hero/throw/body/" + sideStr + '/';

		if (direction == Direction::STAND)
		{
			if (side == right)
				legsSprite.mirrored = true;
			legsSprite.path = "Game/worldSprites/hero/throw/legs/" + sideStr + "/";
			legsSprite.animationLength = 14;
		}
	}

	if (!speedLine.path.empty())
		additionalSprites.push_back(speedLine);*/
	if (legsSprite.packTag != PackTag::empty)
	{
		if (isInverse)
			legsSprite.number = legsSprite.animationLength + 1 - currentSprite[1];
		else
			legsSprite.number = currentSprite[1];
		result.push_back(legsSprite);
	}
	if (bodySprite.packTag != PackTag::empty)
	{
		bodySprite.number = currentSprite[0];
		bodySprite.offset.y += conditionalSizeUnits.y / 10.0f;
		bodySprite.position = Vector2f(position.x, position.y + conditionalSizeUnits.y / 10.0f);
		result.push_back(bodySprite);
	}

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 1000000 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			lastAction = currentAction;
			currentSprite[0] = 1;
		}
		if (++currentSprite[1] > legsSprite.animationLength)
			currentSprite[1] = 1;
		if (currentSprite[2] > legsSprite.animationLength)
			speedLineDirection = SpeedLineDirection::stand;
		else
			currentSprite[2]++;
	}
    for (auto& sprite : result)
        sprite.tag = tag;
    return result;
}
