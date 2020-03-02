#include "deerchant.h"

#include "brazier.h"
#include "empty_object.h"
#include "helper.h"
#include "noose.h"
#include "picked_object.h"

using namespace sf;

deerchant::deerchant(std::string objectName, const Vector2f centerPosition) : dynamic_object(std::move(objectName), centerPosition)
{
	currentSprite.resize(3);
	for (auto& number : currentSprite)
		number = 1;	
	timeForNewSprite = 0;
	moveSystem.defaultSpeed = 0.0006f;
	moveSystem.speed = moveSystem.defaultSpeed;
	animationSpeed = 0.0010f;
	radius = 50;
	moveEndDistance = 55;
	hitDistance = 50;
	strikingSprite = 6;
	strength = 10;
	maxHealthPointValue = 100;
	healthPoint = maxHealthPointValue;
	currentAction = relax;
	toSaveName = "hero";
	tag = entity_tag::hero;
	moveSystem.canCrashIntoStatic = true;

	for (auto i = 0; i < 3; i++)
		bags.emplace_back();
	
	const auto closedBagSize = Vector2f(helper::GetScreenSize().x / 12, helper::GetScreenSize().y / 6);
	
	bags[0].initialize(Vector2f(helper::GetScreenSize().x - closedBagSize.x, closedBagSize.y), true);
	bags[1].initialize(Vector2f(helper::GetScreenSize().x - closedBagSize.x, 2.0f * closedBagSize.y), true);
	bags[2].initialize(Vector2f(helper::GetScreenSize().x - closedBagSize.x * 1.5f, 1.5f * closedBagSize.y), true);
}

deerchant::~deerchant()
= default;

Vector2f deerchant::calculateTextureOffset()
{
	conditionalSizeUnits = { 375, 375 };
	textureBox.width = float(textureBox.width)*getScaleRatio().x;
	textureBox.height = float(textureBox.height)*getScaleRatio().y;
	return { textureBox.width / 2, textureBox.height * 4 / 5 };
}

int deerchant::calculateNextMoveEndSprite(const int currentSprite) const
{
	const auto endSprite1 = 1, endSprite2 = 6;

	const auto increment = 1;

	if (abs(currentSprite - endSprite1) <= abs(currentSprite - endSprite2))
	{
		if (currentSprite < endSprite1)
			return currentSprite + increment;
		if (currentSprite > endSprite1)
			return currentSprite - increment;
		return -1;
	}
	if (currentSprite < endSprite2)
		return currentSprite + increment;
	if (currentSprite > endSprite2)
		return currentSprite - increment;
	return -1;
}

void deerchant::moveEnd(const bool animate, const bool invertDirection)
{
	if (wasPushedAfterMovement)
		return;

	auto pushPosition = position;

	if (directionSystem.direction != Direction::STAND)
	{
		setMoveOffset(10000);
		pushPosition = position - moveSystem.moveOffset;
		if (invertDirection)
			pushPosition = position + moveSystem.moveOffset;
	}
	else
	{
		pushPosition = lastPosition;
		if (invertDirection)
		{
			pushPosition.x += (position.x - lastPosition.x) * 2;
			pushPosition.y += (position.y - lastPosition.y) * 2;
		}
	}

	if (animate)
		changeAction(Actions::moveEnd, false, false);
	else
		changeAction(relax, true, false);

	//bumpedPositions.emplace_back(pushPosition, true);
	//bumpDistance += distance;
}

void deerchant::handleInput(const bool usedMouse)
{
	//high-priority actions
	if (currentAction == absorbs || currentAction == grab || currentAction == dropping || currentAction == builds || currentAction == jerking)
		return;

	if (currentAction == throwNoose && heldItem->content != std::make_pair(entity_tag::noose, 1))
		changeAction(relax, true, false);

	if (Keyboard::isKeyPressed(Keyboard::Space) && currentAction != jerking && directionSystem.direction != Direction::STAND)
	{
		jerk(3, 2);
		return;
	}
	//---------------------

	setHitDirection();
	moveSystem.moveOffset = Vector2f(-1, -1);
	directionSystem.direction = Direction::STAND;
	if (Keyboard::isKeyPressed(Keyboard::A))					
		directionSystem.direction = Direction::LEFT;
	if (Keyboard::isKeyPressed(Keyboard::D))						
		directionSystem.direction = Direction::RIGHT;
	if (Keyboard::isKeyPressed(Keyboard::W))							
		directionSystem.direction = Direction::UP;
	if (Keyboard::isKeyPressed(Keyboard::S))								
		directionSystem.direction = Direction::DOWN;
	if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::W))
		directionSystem.direction = Direction::UPLEFT;
	if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::W))
		directionSystem.direction = Direction::UPRIGHT;
	if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::S))
		directionSystem.direction = Direction::DOWNLEFT;
	if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S))
		directionSystem.direction = Direction::DOWNRIGHT;
	if (directionSystem.direction != Direction::STAND && currentAction != Actions::moveEnd)
	{
 		for (auto i = 0u; i < moveSystem.bumpedPositions.size(); i++)
			if (moveSystem.bumpedPositions[i].cancelable)
				moveSystem.bumpedPositions.erase(moveSystem.bumpedPositions.begin() + i);
		moveSystem.pushAway(0);

		if (directionSystem.direction != directionSystem.lastDirection)
		{
			calculateSpeedLineDirection(directionSystem.lastDirection, directionSystem.direction);
			currentSprite[2] = 1;
		}

		animationSmooth();
		directionSystem.lastDirection = directionSystem.direction;
		wasPushedAfterMovement = false;
	}
	else
		if (!boundTarget && currentAction == move || currentAction == moveHit)
			moveEnd(true);		

	if (currentAction != throwNoose) //second priority actions, interact while moving
	{
		if (!(Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S)))
		{
			auto isIntersect = false;
			if (boundTarget)
				isIntersect = (sqrt(pow(this->position.x - laxMovePosition.x, 2) + pow(this->position.y - laxMovePosition.y, 2)) <= (this->radius + boundTarget->getRadius()));
			if (isIntersect || !boundTarget && currentAction != Actions::moveEnd)
			{				
				directionSystem.direction = Direction::STAND;
				if (currentAction == Actions::move || currentAction == Actions::moveEnd)
				{
					directionSystem.calculateDirection();
					changeAction(relax, true, false);
				}
			}
			else
			{
				if (boundTarget/* && Helper::getDist(position, laxMovePosition) > (this->radius + boundTarget->getRadius())*/)
				{
					directionSystem.calculateDirection();
					setMoveOffset(0);
					changeAction(move, !(currentAction == move || currentAction == Actions::moveEnd), false);
				}
			}
		}
	}
	else
		return;

	//define actions
	if (Mouse::isButtonPressed(Mouse::Left) && heldItem->content.first == entity_tag::noose)
	{
		changeAction(throwNoose, true, false);
		return;
	}

	if (directionSystem.direction != Direction::STAND && currentAction != moveHit && !Mouse::isButtonPressed(Mouse::Left))
		changeAction(move, currentAction == relax/*, currentAction != move*/, false);

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

	if (isBuildSystem)
		return;

	if (Mouse::isButtonPressed(Mouse::Left) && !usedMouse)
	{
		if (directionSystem.direction != Direction::STAND)
		{			
			if (currentAction != moveHit && currentAction != commonHit)
				currentSprite[0] = 1;
			if (currentAction != move && currentAction != moveHit)
				currentSprite[1] = 1;
			changeAction(moveHit, false, false);
		}
		else
			changeAction(commonHit, !(currentAction == moveHit || currentAction == commonHit || currentAction == Actions::moveEnd), false);
	}
}

void deerchant::calculateSpeedLineDirection(Direction lastDirection, Direction direction)
{
	mirroredSpeedLine = false;
	reverseSpeedLine = false;

	speedLineDirection = Direction::STAND;

	if (direction == Direction::LEFT && lastDirection == Direction::RIGHT || direction == Direction::RIGHT && lastDirection == Direction::LEFT)
	{
		speedLineDirection = Direction::UPLEFT;
		return;
	}

	if (direction == Direction::DOWNLEFT)
	{
		if (lastDirection == Direction::UPRIGHT || lastDirection == Direction::RIGHT || lastDirection == Direction::DOWNRIGHT || lastDirection == Direction::DOWN)
			speedLineDirection = Direction::DOWN;
		else
		{
			speedLineDirection = Direction::RIGHT;
			mirroredSpeedLine = true;
		}
	}

	if (direction == Direction::DOWNRIGHT)
	{
		if (lastDirection == Direction::UPLEFT || lastDirection == Direction::LEFT || lastDirection == Direction::DOWNLEFT || lastDirection == Direction::DOWN)
			speedLineDirection = Direction::DOWN;
		else
		{
			speedLineDirection = Direction::RIGHT;
			mirroredSpeedLine = true;
		}
	}

	if (direction == Direction::LEFT)
	{
		if (lastDirection == Direction::RIGHT || lastDirection == Direction::DOWNRIGHT || lastDirection == Direction::DOWN || lastDirection == Direction::DOWNLEFT)
			speedLineDirection = Direction::LEFT;
		else
		{
			speedLineDirection = Direction::LEFT;
			mirroredSpeedLine = true;
		}
	}


	if (direction == Direction::UPLEFT)
	{
		if (lastDirection == Direction::DOWNRIGHT || lastDirection == Direction::DOWN || lastDirection == Direction::DOWNLEFT || lastDirection == Direction::LEFT)
			speedLineDirection = Direction::RIGHT;
		else
		{
			speedLineDirection = Direction::UPLEFT;
			mirroredSpeedLine = true;
		}
	}

	if (direction == Direction::UP)
	{
		if (lastDirection == Direction::DOWN || lastDirection == Direction::DOWNLEFT || lastDirection == Direction::LEFT || lastDirection == Direction::UPLEFT)
			speedLineDirection = Direction::UP;
		else
		{
			speedLineDirection = Direction::UP;
			mirroredSpeedLine = true;
		}
	}

	if (direction == Direction::UPRIGHT)
	{
		if (lastDirection == Direction::DOWNLEFT || lastDirection == Direction::LEFT || lastDirection == Direction::UPLEFT || lastDirection == Direction::UP)
			speedLineDirection = Direction::UPLEFT;
		else
		{
			speedLineDirection = Direction::RIGHT;
			mirroredSpeedLine = true;
		}
	}

	if (direction == Direction::RIGHT)
	{
		if (lastDirection == Direction::LEFT || lastDirection == Direction::UPLEFT || lastDirection == Direction::UP || lastDirection == Direction::UPRIGHT)
			speedLineDirection = Direction::LEFT;
		else
		{
			speedLineDirection = Direction::LEFT;
			mirroredSpeedLine = true;
		}
	}

	if (direction == Direction::DOWNRIGHT)
	{
		if (lastDirection == Direction::UPLEFT || lastDirection == Direction::UP || lastDirection == Direction::UPRIGHT || lastDirection == Direction::RIGHT)
			speedLineDirection = Direction::RIGHT;
		else
		{
			speedLineDirection = Direction::DOWN;
			mirroredSpeedLine = true;
		}
	}
		
	if (direction == Direction::DOWN)
	{
		if (lastDirection == Direction::UP || lastDirection == Direction::UPRIGHT || lastDirection == Direction::RIGHT || lastDirection == Direction::DOWNRIGHT)
		{
			speedLineDirection = Direction::UP;
			mirroredSpeedLine = true;
		}
		else
			speedLineDirection = Direction::UP;				
	}
}

void deerchant::changeAction(const Actions newAction, const bool resetSpriteNumber, const bool rememberLastAction)
{
	if (rememberLastAction)
		lastAction = currentAction;

	currentAction = newAction;

	if (resetSpriteNumber)
		for (auto& number : currentSprite)
			number = 1;
}

void deerchant::stopping(const bool doStand, const bool forgetBoundTarget, const bool offUnsealInventory)
{
	if (boundTarget != nullptr && currentAction != dropping)
		if (boundTarget->getName() == "droppedBag")
			for (auto& bag : bags)
				if (bag.currentState == ejected)
					bag.currentState = bagClosed;

	if (doStand)
	{
		this->laxMovePosition = { -1, -1 };
		moveSystem.moveOffset = { -1, -1 };
		directionSystem.direction = directionSystem.lastDirection;
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

void deerchant::setHitDirection()
{
	const auto screenSize = helper::GetScreenSize();

	const auto xPos = screenSize.x / 2.0f, yPos = screenSize.y / 2.0f;
	
	const auto mouseX = float(Mouse::getPosition().x);
	const auto mouseY = float(Mouse::getPosition().y);
	
	const auto alpha = atan((yPos - mouseY) / (xPos - mouseX)) * 180 / pi;
	
	if (mouseY <= yPos && abs(alpha) >= 45 && abs(alpha) <= 90)
		directionSystem.side = up;
	else
		if (mouseX >= xPos && abs(alpha) >= 0 && abs(alpha) <= 45)
			directionSystem.side = right;
		else
			if (mouseY >= yPos && abs(alpha) >= 45 && abs(alpha) <= 90)
				directionSystem.side = down;
			else
				if (mouseX <= xPos && abs(alpha) >= 0 && abs(alpha) <= 45)
					directionSystem.side = left;
}

void deerchant::setTarget(dynamic_object& object)
{
	nearTheTable = false;
}

void deerchant::behaviorWithDynamic(dynamic_object* target, long long elapsedTime)
{
	if (helper::getDist(position, target->getPosition()) <= radius + target->getRadius())
		moveSystem.pushByBumping(target->getPosition(), target->getRadius(), target->getMoveSystem().canCrashIntoDynamic);

	const auto isIntersect = helper::getDist(position, target->getPosition()) <= this->radius + target->getRadius() + hitDistance;

	if (isIntersect && direction_system::calculateSide(position, target->getPosition(), elapsedTime) != direction_system::invertSide(directionSystem.side))
	{
		if ((this->currentAction == commonHit || this->currentAction == moveHit) && (this->getSpriteNumber() == 4 || this->getSpriteNumber() == 5 || this->getSpriteNumber() == 8))	
			target->takeDamage(this->getStrength(), position);		
	}
}

void deerchant::behaviorWithStatic(world_object* target, long long elapsedTime)
{
	if (!target)
		return;

	if (target->tag == entity_tag::wreathTable && helper::getDist(position, target->getPosition()) <= radius + target->getRadius())
		nearTheTable = true;	
}

void deerchant::behavior(const long long elapsedTime)
{
	jerkInteract(elapsedTime);
	endingPreviousAction();
	fightInteract(elapsedTime);
	speedInteract(elapsedTime);
	animationSmoothInteract(elapsedTime);
	lastPosition = position;	
	
	// interactions with target
	if (!boundTarget || boundTarget->isProcessed)
	{
		laxMovePosition = Vector2f(-1, -1);
		return;
	}	

	if (currentAction != jerking && boundTarget->isProcessed)
		laxMovePosition = boundTarget->getPosition();

	const auto isIntersect = (helper::getDist(position, laxMovePosition)) <= (this->radius + boundTarget->getRadius());

	//touch selected object 
	if (isIntersect)
	{
		boundTarget->isProcessed = true;
		switch (boundTarget->tag)
		{
		case entity_tag::tree:
		{
			if (boundTarget->getState() == absorbed)
				break;
			changeAction(absorbs, true, false);
			currentSprite[0] = 1;
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			boundTarget->setState(absorbed);
			boundTarget->isProcessed = false;
			stopping(true);
			break;
		}
		case entity_tag::fern:
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
		case entity_tag::chamomile:
		case entity_tag::yarrow:		
		case entity_tag::mugwort:
		case entity_tag::noose:
		case entity_tag::droppedLoot:
		case entity_tag::hareTrap:
		case entity_tag::heroBag:
		{
			changeAction(grab, true, false);
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			stopping(true);
			break;
		}
		case entity_tag::dropPoint:
		{		
			changeAction(dropping, true, false);
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			boundTarget->isProcessed = false;
			stopping(true);
			break;
		}
		case entity_tag::buildObject:
		{
			changeAction(builds, true, false);
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
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
				//side = calculateSide(boundTarget->getPosition(), elapsedTime);
				stopping(true, false);
			}
			break;
		}
		}
	}
	else
	{
		boundTarget->isProcessed = false;
	}
}

void deerchant::onMouseUp(const int currentMouseButton, world_object *mouseSelectedObject, const Vector2f mouseWorldPos, const bool isBuilding)
{
	if (isBuilding && currentMouseButton == 2)
	{
		if (boundTarget != nullptr)
		{
			boundTarget->isProcessed = false;
			boundTarget = nullptr;			
		}
		boundTarget = new empty_object("buildItem", mouseWorldPos);
		boundTarget->tag = entity_tag::buildObject;
		laxMovePosition = mouseWorldPos;
		return;
	}

	if (mouseSelectedObject && mouseSelectedObject->tag == entity_tag::brazier)
	{
		auto brazier = dynamic_cast<::brazier*>(mouseSelectedObject);
		if (heldItem->content.first != entity_tag::emptyCell &&
			helper::getDist(brazier->getPlatePosition(), position) <= brazier->getPlateRadius() + radius)
		{
			brazier->putItemToCraft(heldItem->content.first);
			heldItem->content = heldItem->content.second > 1
				? std::make_pair(heldItem->content.first, heldItem->content.second - 1)
				: std::make_pair(entity_tag::emptyCell, 0);
		}
		return;
	}

	if (!mouseSelectedObject && heldItem != nullptr)
	{
		if (heldItem->content.first != entity_tag::emptyCell && currentMouseButton == 2)
		{
			if (boundTarget != nullptr)
			{
				boundTarget->isProcessed = false;
				boundTarget = nullptr;				
			}
			boundTarget = new empty_object("droppedItem", mouseWorldPos);
			boundTarget->tag = entity_tag::dropPoint;
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
				boundTarget = new empty_object("droppedBag", mouseWorldPos);
				boundTarget->tag = entity_tag::dropPoint;
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

void deerchant::endingPreviousAction()
{
	if (lastAction == commonHit && !Mouse::isButtonPressed(Mouse::Left))
	{
		directionSystem.lastDirection = direction_system::sideToDirection(directionSystem.side);
		changeAction(relax, true, false);
	}
	if (lastAction == moveHit && !Mouse::isButtonPressed(Mouse::Left))
	{
		directionSystem.lastDirection = direction_system::sideToDirection(directionSystem.side);
		changeAction(move, true, false);
	}
	if (lastAction == Actions::moveEnd)
	{
		changeAction(relax, true, false);
	}
	if (lastAction == open)
		changeAction(relax, true, false);

	if (lastAction == absorbs)
	{
		boundTarget->isProcessed = false;
		boundTarget = nullptr;
		changeAction(relax, true, false);
	}
	if (lastAction == builds)
	{
		changeAction(relax, true, false);
	}
	if (lastAction == dropping)
	{
		changeAction(relax, true, false);
		if (heldItem->content.first != entity_tag::emptyCell)
		{
			birth_static_info dropObject;
			dropObject.position = { boundTarget->getPosition().x, boundTarget->getPosition().y };
			dropObject.tag = entity_tag::droppedLoot;
			dropObject.typeOfObject = int(heldItem->content.first);
			dropObject.count = heldItem->content.second;
			birthStatics.push(dropObject);
			heldItem->content = std::make_pair(entity_tag::emptyCell, 0);
		}
		else
		{
			for (auto cnt = 0u; cnt != bags.size(); cnt++)			
				if (bags[cnt].currentState == ejected)
				{
					auto isHareTrap = true;
					for (auto& item : bags[cnt].cells)					
						if (item.content.first != entity_tag::yarrow && item.content.first != entity_tag::emptyCell)
						{
							isHareTrap = false;
							break;
						}
					birth_static_info dropObject;
					if (isHareTrap)
					{
						dropObject.tag = entity_tag::hareTrap;											
					}
					else
					{
						dropObject.tag = entity_tag::droppedLoot;
						dropObject.typeOfObject = int(entity_tag::heroBag);	
					}
					dropObject.position = { position.x, position.y + radius };
					dropObject.inventory = hero_bag::cellsToInventory(bags[cnt].cells);					
					//bags[cnt].~HeroBag();
					bags.erase(bags.begin() + cnt);
					birthStatics.push(dropObject);
					break;
				}	
		}
		stopping(true, true);
	}
	if (currentAction == throwNoose && currentSprite[0] == 12 && heldItem->content == std::make_pair(entity_tag::noose, 1))
	{
		heldItem->content = { entity_tag::emptyCell, 0 };
		birth_dynamic_info nooseObject;
		nooseObject.position = position;
		nooseObject.tag = entity_tag::noose;
		nooseObject.owner = this;
		birthDynamics.push(nooseObject);
	}
    if (lastAction == throwNoose)
    {
		directionSystem.lastDirection = direction_system::sideToDirection(directionSystem.side);
		changeAction(relax, true, false);
    }
	if (lastAction == open)
	{
		if (unsealInventoryOwner)
		{
			unsealInventoryOwner->isVisibleInventory = true;
			changeAction(relax, true, false);
		}
	}
	if (lastAction == grab)
	{
		if (boundTarget)
		{
			auto pickedItem = dynamic_cast<picked_object*>(boundTarget);
			if (pickedItem)
			{
				if (pickedItem->getType() == int(entity_tag::heroBag) || pickedItem->getId() == entity_tag::hareTrap)
				{					
					bags.resize(bags.size() + 1);
					bags[bags.size() - 1] = *(new hero_bag());
					bags[bags.size()-1].initialize(Vector2f(helper::GetScreenSize().x / 2, helper::GetScreenSize().y / 2), true, pickedItem->inventory);
					pickedItem->deletePromiseOn();
				}
				else
					pickedItem->pickUp(&this->bags);
			}
			
			auto nooseItem = dynamic_cast<noose*>(boundTarget);
			if (nooseItem)
			{
				const auto placedNoose = new std::vector<std::pair<entity_tag, int>>({ (std::make_pair(entity_tag::noose, 1)) });
				if (hero_bag::putItemsIn(placedNoose, &bags))
					nooseItem->deletePromiseOn();
				delete placedNoose;
			}
			stopping(true, true);
			changeAction(relax, true, false);
		}
	}
	lastAction = relax;
}

void deerchant::animationSmooth()
{
	if (directionSystem.direction == Direction::UP && directionSystem.lastDirection == Direction::LEFT ||
		directionSystem.direction == Direction::LEFT && directionSystem.lastDirection == Direction::UP)
	{
		smoothDirections = { Direction::UPLEFT, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::DOWN && directionSystem.lastDirection == Direction::LEFT ||
		directionSystem.direction == Direction::LEFT && directionSystem.lastDirection == Direction::DOWN)
	{
		smoothDirections = { Direction::DOWNLEFT, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::UP && directionSystem.lastDirection == Direction::RIGHT ||
		directionSystem.direction == Direction::RIGHT && directionSystem.lastDirection == Direction::UP)
	{
		smoothDirections = { Direction::UPRIGHT, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::DOWN && directionSystem.lastDirection == Direction::RIGHT ||
		directionSystem.direction == Direction::RIGHT && directionSystem.lastDirection == Direction::DOWN)
	{
		smoothDirections = { Direction::DOWNRIGHT, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::UP && directionSystem.lastDirection == Direction::DOWN)
	{
		smoothDirections = { Direction::DOWNLEFT, Direction::LEFT, Direction::UPLEFT, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::DOWN && directionSystem.lastDirection == Direction::UP){
		smoothDirections = { Direction::UPRIGHT, Direction::RIGHT, Direction::DOWNRIGHT, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::LEFT && directionSystem.lastDirection == Direction::RIGHT)
	{		
		smoothDirections = { Direction::DOWNRIGHT, Direction::DOWN, Direction::DOWNLEFT, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::RIGHT && directionSystem.lastDirection == Direction::LEFT)
	{
		smoothDirections = { Direction::UPLEFT, Direction::UP, Direction::UPRIGHT, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::UPLEFT && directionSystem.lastDirection == Direction::DOWNRIGHT)
	{
		smoothDirections = { Direction::RIGHT, Direction::UPRIGHT, Direction::UPRIGHT, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::DOWNRIGHT && directionSystem.lastDirection == Direction::UPLEFT)
	{
		smoothDirections = { Direction::UP, Direction::UPRIGHT, Direction::RIGHT, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::UPRIGHT && directionSystem.lastDirection == Direction::DOWNLEFT)
	{
		smoothDirections = { Direction::LEFT, Direction::UPLEFT, Direction::UP, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::DOWNLEFT && directionSystem.lastDirection == Direction::UPRIGHT)
	{
		smoothDirections = { Direction::UP, Direction::UPLEFT, Direction::LEFT, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::UPLEFT && directionSystem.lastDirection == Direction::UPRIGHT ||
		directionSystem.direction == Direction::UPRIGHT && directionSystem.lastDirection == Direction::UPLEFT)
	{
		smoothDirections = { Direction::UP, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::UPLEFT && directionSystem.lastDirection == Direction::DOWNLEFT ||
		directionSystem.direction == Direction::DOWNLEFT && directionSystem.lastDirection == Direction::UPLEFT)
	{
		smoothDirections = { Direction::LEFT, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::UPRIGHT && directionSystem.lastDirection == Direction::DOWNRIGHT ||
		directionSystem.direction == Direction::DOWNRIGHT && directionSystem.lastDirection == Direction::UPRIGHT)
	{
		smoothDirections = { Direction::RIGHT, Direction::STAND };
		return;
	}
	if (directionSystem.direction == Direction::DOWNLEFT && directionSystem.lastDirection == Direction::DOWNRIGHT ||
		directionSystem.direction == Direction::DOWNRIGHT && directionSystem.lastDirection == Direction::DOWNLEFT)
	{
		smoothDirections = { Direction::DOWN, Direction::STAND };
	}

}

void deerchant::animationSmoothInteract(long long elapsedTime)
{
	smoothMoveTime += elapsedTime;

	if (smoothDirections.empty())
	{
		smoothDirection = Direction::STAND;
		return;
	}

	const long long time_for_new_smooth_direction = 70000;

	if (smoothMoveTime >= time_for_new_smooth_direction)
	{
		smoothDirection = smoothDirections[0];
		smoothDirections.erase(smoothDirections.begin() + 0);
		smoothMoveTime = 0;
	}
}

void deerchant::speedInteract(const long long elapsedTime)
{
	if (!(currentAction == move || currentAction == moveHit || currentAction == Actions::moveEnd || currentAction == throwNoose))
	{
		moveTime = 0;
		return;
	}

	moveTime += elapsedTime;

	const long long speedIncreaseTime = long(5e5);
	const auto partOfSpeed = 0.3f;
	
	if (moveTime < speedIncreaseTime)
		moveSystem.speed = (partOfSpeed + float(moveTime) * (1 - partOfSpeed) / speedIncreaseTime) * moveSystem.defaultSpeed;
	else
		moveSystem.speed = moveSystem.defaultSpeed;
}

void deerchant::jerkInteract(const long long elapsedTime)
{
	if (currentAction == jerking)
	{
		if (jerkTime > 0)
		{
			jerkTime -= elapsedTime;
			moveSystem.speed = jerkDistance / float(jerkDuration) * jerkPower * pow(float(jerkTime) / float(jerkDuration), jerkDeceleration);
			moveSystem.speed = std::max(moveSystem.defaultSpeed / jerkDeceleration, moveSystem.speed);
		}
		else
		{
			changeAction(relax, true, false);
			moveSystem.speed = moveSystem.defaultSpeed;
		}
	}
}

Vector2f deerchant::getBuildPosition(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

Vector2f deerchant::getBeltPosition() const
{
	/*if (additionalSprites.size() >= 2) return
		Vector2f((4 * additionalSprites[0].position.x + additionalSprites[1].position.x) / 5.0f + conditionalSizeUnits.x / 3.0f,
		(4 * additionalSprites[0].position.y + additionalSprites[1].position.y) / 5.0f);
	return additionalSprites[0].position;*/
    return position;
}

int deerchant::getBuildType(Vector2f, Vector2f)
{
	return 1;
}

void deerchant::jerk(const float power, const float deceleration, Vector2f)
{
	stopping(false, true);
	this->jerkPower = power;
	this->jerkDeceleration = deceleration;
	this->jerkDuration = 700000;
	this->jerkTime = this->jerkDuration;
	changeAction(jerking, currentAction != jerking, false);
	jerkDistance = 500;
	currentSprite[0] = 1;

	laxMovePosition = Vector2f(position.x + cos(float(directionSystem.direction) * pi / 180) * jerkDistance, position.y - sin(float(directionSystem.direction) * pi / 180) * jerkDistance);
}

void deerchant::fightInteract(long long elapsedTime, dynamic_object* target)
{
	timeAfterHitSelf += elapsedTime;
	moveSystem.pushAway(elapsedTime);
}

sprite_chain_element* deerchant::prepareSpeedLine()
{
	auto speedLine = new sprite_chain_element(pack_tag::heroMove, pack_part::lines, Direction::STAND, 1, position, conditionalSizeUnits, Vector2f(textureBoxOffset));
	speedLine->animationLength = 3;
	if (speedLineDirection == Direction::STAND || currentSprite[2] > speedLine->animationLength)
		return speedLine;

	speedLine->direction = speedLineDirection;
	speedLine->mirrored = mirroredSpeedLine;
	speedLine->offset.y += conditionalSizeUnits.y / 9.0f;
	speedLine->position = Vector2f(position.x, position.y + conditionalSizeUnits.y / 9.0f);

	if (reverseSpeedLine)
		speedLine->number = speedLine->animationLength + 1 - currentSprite[2];
	else
		speedLine->number = currentSprite[2];

	return speedLine;
}

std::vector<sprite_chain_element*> deerchant::prepareSprites(long long elapsedTime)
{
	auto legsSprite = new sprite_chain_element(Vector2f(position.x, position.y - 1), conditionalSizeUnits, { textureBoxOffset.x, textureBoxOffset.y + 1 }, color);
	auto* bodySprite = new sprite_chain_element(position, conditionalSizeUnits, textureBoxOffset, color);

	const auto speedLine = prepareSpeedLine();
	std::vector<sprite_chain_element*> result = {};
	auto legsInverse = false, bodyInverse = false;
	legsSprite->animationLength = 8;

    auto spriteSide = directionSystem.side; auto spriteDirection = directionSystem.lastDirection;

	animationSpeed = 12;

	if (directionSystem.direction == Direction::RIGHT || directionSystem.direction == Direction::UPRIGHT || directionSystem.direction == Direction::DOWNRIGHT)
	{
		spriteDirection = direction_system::cutRights(spriteDirection);
		legsSprite->mirrored = true;
	}

	if (directionSystem.side == right && currentAction != move && currentAction != Actions::moveEnd && currentAction != jerking)
	{
		spriteSide = left;
		bodySprite->mirrored = true;
	}

	switch (currentAction)
	{
	case commonHit:
		bodySprite->animationLength = 8;
		if (directionSystem.side == right)
			legsSprite->mirrored = true;
		bodySprite->packTag = pack_tag::heroHit; bodySprite->packPart = pack_part::body; bodySprite->direction = direction_system::sideToDirection(spriteSide);
		legsSprite->packTag = pack_tag::heroHit; legsSprite->packPart = pack_part::legs; legsSprite->direction = direction_system::sideToDirection(spriteSide);
		break;
	case absorbs:
		bodySprite->animationLength = 10;
		bodySprite->packTag = pack_tag::heroAbsorb; bodySprite->packPart = pack_part::full; bodySprite->direction = direction_system::cutDiagonals(spriteDirection);	
		break;
	case builds:
		bodySprite->animationLength = 10;
		bodySprite->packTag = pack_tag::heroAbsorb; bodySprite->packPart = pack_part::full; bodySprite->direction = direction_system::cutDiagonals(spriteDirection);
		break;
	case grab:
		bodySprite->animationLength = 11;
		animationSpeed = 15;
		bodySprite->packTag = pack_tag::heroPick; bodySprite->packPart = pack_part::full; bodySprite->direction = direction_system::cutDiagonals(spriteDirection);
		break;
	case dropping:
		bodyInverse = true;
		bodySprite->animationLength = 5;
		if (directionSystem.lastDirection == Direction::RIGHT || directionSystem.lastDirection == Direction::UPRIGHT || directionSystem.lastDirection == Direction::DOWNRIGHT)
			bodySprite->mirrored = true;
		bodySprite->packTag = pack_tag::heroPick; bodySprite->packPart = pack_part::full; bodySprite->direction = direction_system::cutDiagonals(spriteDirection);
		break;
	case open:
		bodySprite->animationLength = 12;
		bodySprite->packTag = pack_tag::heroPick; bodySprite->packPart = pack_part::full; bodySprite->direction = direction_system::cutDiagonals(spriteDirection);
		break;
	case transitionToEnotherWorld:
		bodySprite->animationLength = 18;
		break;
	case jerking:
		bodySprite->animationLength = 8;
		animationSpeed = 11;
		spriteDirection = directionSystem.lastDirection;
		if (directionSystem.lastDirection == Direction::RIGHT || directionSystem.lastDirection == Direction::UPRIGHT || directionSystem.lastDirection == Direction::DOWNRIGHT)
			bodySprite->mirrored = true;
		if (direction_system::cutRights(directionSystem.lastDirection) == Direction::UPLEFT || direction_system::cutRights(directionSystem.lastDirection) == Direction::DOWNLEFT)
			spriteDirection = Direction::LEFT;
		bodySprite->packTag = pack_tag::heroRoll; bodySprite->packPart = pack_part::full; bodySprite->direction = direction_system::cutRights(spriteDirection);
		break;
	case relax:
		bodySprite->mirrored = false;
		bodySprite->animationLength = 16;
		animationSpeed = 13;
		spriteDirection = directionSystem.lastDirection;
		if (directionSystem.lastDirection == Direction::RIGHT || directionSystem.lastDirection == Direction::UPRIGHT || directionSystem.lastDirection == Direction::DOWNRIGHT)
			bodySprite->mirrored = true;
		if (direction_system::cutRights(directionSystem.lastDirection) == Direction::UPLEFT || direction_system::cutRights(directionSystem.lastDirection) == Direction::DOWNLEFT)
			spriteDirection = Direction::LEFT;
		bodySprite->packTag = pack_tag::heroStand; bodySprite->packPart = pack_part::full; bodySprite->direction = direction_system::cutRights(spriteDirection);
		break;
	case move:
	{
		animationSpeed = moveSystem.speed / moveSystem.defaultSpeed * 12;
		if (animationSpeed < 10)
			animationSpeed = 10;
		bodySprite->animationLength = 8;

		auto finalDirection = directionSystem.lastDirection;
		if (smoothDirection != Direction::STAND)
			finalDirection = smoothDirection;

		if (finalDirection == Direction::RIGHT || finalDirection == Direction::UPRIGHT || finalDirection == Direction::DOWNRIGHT)
		{
			bodySprite->mirrored = true;
			legsSprite->mirrored = true;
		}

		bodySprite->packTag = pack_tag::heroMove; bodySprite->packPart = pack_part::body; bodySprite->direction = direction_system::cutRights(finalDirection);
		legsSprite->packTag = pack_tag::heroMove; legsSprite->packPart = pack_part::legs; legsSprite->direction = direction_system::cutRights(finalDirection);
		break;
	}
	case Actions::moveEnd:
		bodySprite->animationLength = 8;
		currentSprite[1] = 1;
		if (directionSystem.lastDirection == Direction::RIGHT || directionSystem.lastDirection == Direction::UPRIGHT || directionSystem.lastDirection == Direction::DOWNRIGHT)
		{
			bodySprite->mirrored = true;
			legsSprite->mirrored = true;
		}
		bodySprite->packTag = pack_tag::heroMove; bodySprite->packPart = pack_part::body; bodySprite->direction = direction_system::cutRights(directionSystem.lastDirection);
		directionSystem.lastDirection = direction_system::cutDiagonals(directionSystem.lastDirection);
		legsSprite->packTag = pack_tag::heroHit; legsSprite->packPart = pack_part::legs; legsSprite->direction = direction_system::cutRights(directionSystem.lastDirection);
		
		break;
	}

	if (currentAction == moveHit)
	{
		animationSpeed = moveSystem.speed / moveSystem.defaultSpeed * 12;
		if (animationSpeed < 10)
			animationSpeed = 10;
		bodySprite->animationLength = 8;

		if (directionSystem.direction == Direction::UP && directionSystem.side == down || directionSystem.direction == Direction::DOWN && directionSystem.side == up)
		{
			legsInverse = true;
			spriteDirection = direction_system::sideToDirection(spriteSide);
		}
		if ((directionSystem.direction == Direction::LEFT || directionSystem.direction == Direction::UPLEFT || directionSystem.direction == Direction::DOWNLEFT) && directionSystem.side == right ||
			(directionSystem.direction == Direction::UPLEFT || directionSystem.direction == Direction::UPRIGHT) && directionSystem.side == down ||
			(directionSystem.direction == Direction::RIGHT || directionSystem.direction == Direction::UPRIGHT || directionSystem.direction == Direction::DOWNRIGHT) && directionSystem.side == left ||
			(directionSystem.direction == Direction::DOWNLEFT || directionSystem.direction == Direction::DOWNRIGHT) && directionSystem.side == up)
		{
			legsInverse = true;
			legsSprite->mirrored = !legsSprite->mirrored;
			spriteDirection = direction_system::invertDirection(spriteDirection);
		}
		if (directionSystem.direction == Direction::UP && directionSystem.side == down || directionSystem.direction == Direction::DOWN && directionSystem.side == up)
			legsInverse = true;

		legsSprite->packTag = pack_tag::heroMove; legsSprite->packPart = pack_part::legs; legsSprite->direction = direction_system::cutRights(spriteDirection);
		bodySprite->packTag = pack_tag::heroHit; bodySprite->packPart = pack_part::body; bodySprite->direction = direction_system::sideToDirection(spriteSide);

		if (directionSystem.direction == Direction::STAND)
		{
			if (directionSystem.side == right)
				legsSprite->mirrored = true;
			legsSprite->packTag = pack_tag::heroMove; legsSprite->packPart = pack_part::legs; legsSprite->direction = direction_system::sideToDirection(spriteSide);
			legsSprite->animationLength = 14;
		}
	}

	if (currentAction == throwNoose)
	{
		bodySprite->animationLength = 14;
		if (directionSystem.direction == Direction::UP && directionSystem.side == down || directionSystem.direction == Direction::DOWN && directionSystem.side == up)
		{
			legsInverse = true;
			spriteDirection = direction_system::sideToDirection(spriteSide);
		}
		if ((directionSystem.direction == Direction::LEFT || directionSystem.direction == Direction::UPLEFT || directionSystem.direction == Direction::DOWNLEFT) && directionSystem.side == right ||
			(directionSystem.direction == Direction::RIGHT || directionSystem.direction == Direction::UPRIGHT || directionSystem.direction == Direction::DOWNRIGHT) && directionSystem.side == left)
		{
			legsInverse = true;
			legsSprite->mirrored = !legsSprite->mirrored;
		}

		legsSprite->packTag = pack_tag::heroMove; legsSprite->packPart = pack_part::legs, legsSprite->direction = spriteDirection;
		bodySprite->packTag = pack_tag::heroThrow; bodySprite->packPart = pack_part::body; bodySprite->direction = direction_system::sideToDirection(spriteSide);

		if (directionSystem.direction == Direction::STAND)
		{
			if (directionSystem.side == right)
				legsSprite->mirrored = true;
			legsSprite->packTag = pack_tag::heroThrow; legsSprite->packPart = pack_part::legs, legsSprite->direction = direction_system::sideToDirection(spriteSide);
			legsSprite->animationLength = 14;
		}
	}

	//if (speedLineDirection != Direction::STAND)
		//result.push_back(speedLine);

	if (legsSprite->packTag != pack_tag::empty)
	{
		if (legsInverse)
			legsSprite->number = legsSprite->animationLength + 1 - currentSprite[1];
		else
			legsSprite->number = currentSprite[1];
		result.push_back(legsSprite);
	}

	if (bodySprite->packTag != pack_tag::empty)
	{
		if (bodyInverse)
			bodySprite->number = bodySprite->animationLength + 1 - currentSprite[0];
		else
			bodySprite->number = currentSprite[0];
		result.push_back(bodySprite);
	}

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 1e6 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (currentAction == Actions::moveEnd)
		{
			currentSprite[0] = calculateNextMoveEndSprite(currentSprite[0]);
			if (currentSprite[0] == -1)
			{
				lastAction = currentAction;
				currentSprite[0] = 1;
			}
		}
		else
		{
			if (++currentSprite[0] > bodySprite->animationLength)
				currentSprite[0] = 1;
			if (currentSprite[0] >= bodySprite->animationLength)
				lastAction = currentAction;
			if (++currentSprite[1] > legsSprite->animationLength)
				currentSprite[1] = 1;
			if (currentSprite[2] > speedLine->animationLength)
				speedLineDirection = Direction::STAND;
			else
				currentSprite[2]++;
		}
	}

	setUnscaled(result);

    return result;
}
