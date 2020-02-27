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
	radius = 50;
	moveEndDistance = 55;
	hitDistance = 50;
	strikingSprite = 6;
	strength = 10;
	maxHealthPointValue = 100;
	healthPoint = maxHealthPointValue;
	currentAction = relax;
	toSaveName = "hero";
	tag = Tag::hero;
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

Vector2f Deerchant::calculateTextureOffset()
{
	conditionalSizeUnits = { 375, 375 };
	textureBox.width = float(textureBox.width)*getScaleRatio().x;
	textureBox.height = float(textureBox.height)*getScaleRatio().y;
	return { textureBox.width / 2, textureBox.height * 4 / 5 };
}

int Deerchant::calculateNextMoveEndSprite(int currentSprite) const
{
	const int endSprite1 = 1, endSprite2 = 6;

	int increment = 1;

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

void Deerchant::moveEnd(bool animate, float distance, float speed, bool invertDirection)
{
	if (wasPushedAfterMovement)
		return;

	Vector2f pushPosition = position;

	if (direction != Direction::STAND)
	{
		setMoveOffset(10000);
		pushPosition = position - moveOffset;
		if (invertDirection)
			pushPosition = position + moveOffset;
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

void Deerchant::handleInput(bool usedMouse)
{
	//high-priority actions
	if (currentAction == absorbs || currentAction == grab || currentAction == dropping || currentAction == builds || currentAction == jerking)
		return;

	if (currentAction == throwNoose && heldItem->content != std::make_pair(Tag::noose, 1))
		changeAction(relax, true, false);

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
	if (direction != Direction::STAND && currentAction != Actions::moveEnd)
	{
 		for (int i = 0; i < bumpedPositions.size(); i++)
			if (bumpedPositions[i].cancelable)
				bumpedPositions.erase(bumpedPositions.begin() + i);
		pushAway(0);

		if (direction != lastDirection)
		{
			calculateSpeedLineDirection(lastDirection, direction);
			currentSprite[2] = 1;
		}

		animationSmooth();
		lastDirection = direction;
		wasPushedAfterMovement = false;
	}
	else
		if (!boundTarget && currentAction == move || currentAction == moveHit)
			moveEnd(true, moveEndDistance);		

	if (currentAction != throwNoose) //second priority actions, interact while moving
	{
		if (!(Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S)))
		{
			bool isIntersect = false;
			if (boundTarget)
				isIntersect = (sqrt(pow(this->position.x - laxMovePosition.x, 2) + pow(this->position.y - laxMovePosition.y, 2)) <= (this->radius + boundTarget->getRadius()));
			if (isIntersect || !boundTarget && currentAction != Actions::moveEnd)
			{				
				direction = Direction::STAND;
				if (currentAction == Actions::move || currentAction == Actions::moveEnd)
				{
					calculateDirection();
					changeAction(relax, true, false);
				}
			}
			else
			{
				if (boundTarget/* && Helper::getDist(position, laxMovePosition) > (this->radius + boundTarget->getRadius())*/)
				{
					calculateDirection();
					setMoveOffset(0);
					changeAction(move, !(currentAction == move || currentAction == Actions::moveEnd), false);
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
		if (direction != Direction::STAND)
		{			
			if (!(currentAction == moveHit || currentAction == commonHit))
				currentSprite[0] = 1;
			if (!(currentAction == move || currentAction == moveHit))
				currentSprite[1] = 1;
			changeAction(moveHit, false, false);
		}
		else
			changeAction(commonHit, !(currentAction == moveHit || currentAction == commonHit), false);
	}
}

void Deerchant::calculateSpeedLineDirection(Direction lastDirection, Direction direction)
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

void Deerchant::changeAction(Actions newAction, bool resetSpriteNumber, bool rememberLastAction)
{
	if (rememberLastAction)
		lastAction = currentAction;

	currentAction = newAction;

	if (resetSpriteNumber)
		for (auto& number : currentSprite)
			number = 1;
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
		this->direction = lastDirection;
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

void Deerchant::behaviorWithDynamic(DynamicObject* target, long long elapsedTime)
{
	if (Helper::getDist(position, target->getPosition()) <= radius + target->getRadius())
		pushByBumping(target);

	const bool isIntersect = Helper::getDist(position, target->getPosition()) <= this->radius + target->getRadius() + hitDistance;

	if (isIntersect && calculateSide(target->getPosition(), elapsedTime) != invertSide(side))
	{
		if ((this->currentAction == commonHit || this->currentAction == moveHit) && (this->getSpriteNumber() == 4 || this->getSpriteNumber() == 5 || this->getSpriteNumber() == 8))	
			target->takeDamage(this->getStrength(), position);		
	}
}

void Deerchant::behaviorWithStatic(WorldObject* target, long long elapsedTime)
{
	if (!target)
		return;

	if (target->tag == Tag::wreathTable && Helper::getDist(position, target->getPosition()) <= radius + target->getRadius())
		nearTheTable = true;	
}

void Deerchant::behavior(long long elapsedTime)
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

	const bool isIntersect = (Helper::getDist(position, laxMovePosition)) <= (this->radius + boundTarget->getRadius());

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
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
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
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			stopping(true);
			break;
		}
		case Tag::dropPoint:
		{		
			changeAction(dropping, true, false);
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			boundTarget->isProcessed = false;
			stopping(true);
			break;
		}
		case Tag::buildObject:
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
		lastDirection = DirectionsSystem::sideToDirection(side);
		changeAction(relax, true, false);
	}
	if (lastAction == moveHit && !Mouse::isButtonPressed(Mouse::Left))
	{
		lastDirection = DirectionsSystem::sideToDirection(side);
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
		if (heldItem->content.first != Tag::emptyCell)
		{
			birthStaticInfo dropObject;
			dropObject.position = { boundTarget->getPosition().x, boundTarget->getPosition().y };
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
					//bags[cnt].~HeroBag();
					bags.erase(bags.begin() + cnt);
					birthStatics.push(dropObject);
					break;
				}	
		}
		stopping(true, true);
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
		lastDirection = DirectionsSystem::sideToDirection(side);
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
			auto pickedItem = dynamic_cast<PickedObject*>(boundTarget);
			if (pickedItem)
			{
				if (pickedItem->getType() == int(Tag::heroBag) || pickedItem->getId() == Tag::hareTrap)
				{					
					bags.resize(bags.size() + 1);
					bags[bags.size() - 1] = *(new HeroBag());
					bags[bags.size()-1].initialize(Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2), true, pickedItem->inventory);
					pickedItem->deletePromiseOn();
				}
				else
					pickedItem->pickUp(&this->bags);
			}
			
			auto nooseItem = dynamic_cast<Noose*>(boundTarget);
			if (nooseItem)
			{
				const auto placedNoose = new std::vector<std::pair<Tag, int>>({ (std::make_pair(Tag::noose, 1)) });
				if (HeroBag::putItemsIn(placedNoose, &bags))
					nooseItem->deletePromiseOn();
				delete placedNoose;
			}
			stopping(true, true);
			changeAction(relax, true, false);
		}
	}
	lastAction = relax;
}

void Deerchant::animationSmooth()
{
	if (direction == Direction::UP && lastDirection == Direction::LEFT || direction == Direction::LEFT && lastDirection == Direction::UP)
		smoothDirections = { Direction::UPLEFT, Direction::STAND };
	else
	if (direction == Direction::DOWN && lastDirection == Direction::LEFT || direction == Direction::LEFT && lastDirection == Direction::DOWN)
		smoothDirections = { Direction::DOWNLEFT, Direction::STAND };
	else
	if (direction == Direction::UP && lastDirection == Direction::RIGHT || direction == Direction::RIGHT && lastDirection == Direction::UP)
		smoothDirections = { Direction::UPRIGHT, Direction::STAND };
	else
	if (direction == Direction::DOWN && lastDirection == Direction::RIGHT || direction == Direction::RIGHT && lastDirection == Direction::DOWN)
		smoothDirections = { Direction::DOWNRIGHT, Direction::STAND };
	else
	if (direction == Direction::UP && lastDirection == Direction::DOWN)
		smoothDirections = { Direction::DOWNLEFT, Direction::LEFT, Direction::UPLEFT, Direction::STAND };
	else
	if (direction == Direction::DOWN && lastDirection == Direction::UP)
		smoothDirections = { Direction::UPRIGHT, Direction::RIGHT, Direction::DOWNRIGHT, Direction::STAND };
	else
	if (direction == Direction::LEFT && lastDirection == Direction::RIGHT)
		smoothDirections = { Direction::DOWNRIGHT, Direction::DOWN, Direction::DOWNLEFT, Direction::STAND };
	else
	if (direction == Direction::RIGHT && lastDirection == Direction::LEFT)
		smoothDirections = { Direction::UPLEFT, Direction::UP, Direction::UPRIGHT, Direction::STAND };
	else
	if (direction == Direction::UPLEFT && lastDirection == Direction::DOWNRIGHT)
		smoothDirections = { Direction::RIGHT, Direction::UPRIGHT, Direction::UPRIGHT, Direction::STAND };
	else
	if (direction == Direction::DOWNRIGHT && lastDirection == Direction::UPLEFT)
		smoothDirections = { Direction::UP, Direction::UPRIGHT, Direction::RIGHT, Direction::STAND };
	else
	if (direction == Direction::UPRIGHT && lastDirection == Direction::DOWNLEFT)
		smoothDirections = { Direction::LEFT, Direction::UPLEFT, Direction::UP, Direction::STAND };
	else
	if (direction == Direction::DOWNLEFT && lastDirection == Direction::UPRIGHT)
		smoothDirections = { Direction::UP, Direction::UPLEFT, Direction::LEFT, Direction::STAND };
	else
	if (direction == Direction::UPLEFT && lastDirection == Direction::UPRIGHT || direction == Direction::UPRIGHT && lastDirection == Direction::UPLEFT)
		smoothDirections = { Direction::UP, Direction::STAND };
	else
	if (direction == Direction::UPLEFT && lastDirection == Direction::DOWNLEFT || direction == Direction::DOWNLEFT && lastDirection == Direction::UPLEFT)
		smoothDirections = { Direction::LEFT, Direction::STAND };
	else
	if (direction == Direction::UPRIGHT && lastDirection == Direction::DOWNRIGHT || direction == Direction::DOWNRIGHT && lastDirection == Direction::UPRIGHT)
		smoothDirections = { Direction::RIGHT, Direction::STAND };
	if (direction == Direction::DOWNLEFT && lastDirection == Direction::DOWNRIGHT || direction == Direction::DOWNRIGHT && lastDirection == Direction::DOWNLEFT)
		smoothDirections = { Direction::DOWN, Direction::STAND };

}

void Deerchant::animationSmoothInteract(long long elapsedTime)
{
	smoothMoveTime += elapsedTime;

	if (smoothDirections.empty())
	{
		smoothDirection = Direction::STAND;
		return;
	}

	const float timeForNewSmoothDirection = 70000;

	if (smoothMoveTime >= timeForNewSmoothDirection)
	{
		smoothDirection = smoothDirections[0];
		smoothDirections.erase(smoothDirections.begin() + 0);
		smoothMoveTime = 0;
	}
}

void Deerchant::speedInteract(float elapsedTime)
{
	if (!(currentAction == move || currentAction == moveHit || currentAction == Actions::moveEnd || currentAction == throwNoose))
	{
		moveTime = 0;
		return;
	}

	moveTime += elapsedTime;

	const float speedIncreaseTime = 5e5, partOfSpeed = 0.3f;
	if (moveTime < speedIncreaseTime)
		speed = (partOfSpeed + moveTime * (1 - partOfSpeed) / speedIncreaseTime) * defaultSpeed;
	else
		speed = defaultSpeed;
}

void Deerchant::jerkInteract(long long elapsedTime)
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

Vector2f Deerchant::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

Vector2f Deerchant::getBeltPosition() const
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

void Deerchant::fightInteract(long long elapsedTime, DynamicObject* target)
{
	pushAway(elapsedTime);
}

SpriteChainElement* Deerchant::prepareSpeedLine()
{
	SpriteChainElement* speedLine = new SpriteChainElement(PackTag::heroMove, PackPart::lines, Direction::STAND, 1, position, conditionalSizeUnits, Vector2f(textureBoxOffset));
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

std::vector<SpriteChainElement*> Deerchant::prepareSprites(long long elapsedTime)
{
	auto legsSprite = new SpriteChainElement(Vector2f(position.x, position.y - 1), conditionalSizeUnits, { textureBoxOffset.x, textureBoxOffset.y + 1 }, color);
	auto* bodySprite = new SpriteChainElement(position, conditionalSizeUnits, textureBoxOffset, color);

	SpriteChainElement* speedLine = prepareSpeedLine();
	std::vector<SpriteChainElement*> result = {};
	bool legsInverse = false, bodyInverse = false;
	legsSprite->animationLength = 8;

    auto spriteSide = side; auto spriteDirection = lastDirection;

	animationSpeed = 12;

	if (direction == Direction::RIGHT || direction == Direction::UPRIGHT || direction == Direction::DOWNRIGHT)
	{
		spriteDirection = DirectionsSystem::cutRights(spriteDirection);
		legsSprite->mirrored = true;
	}

	if (side == right && currentAction != move && currentAction != Actions::moveEnd && currentAction != jerking)
	{
		spriteSide = left;
		bodySprite->mirrored = true;
	}

	switch (currentAction)
	{
	case commonHit:
		bodySprite->animationLength = 8;
		if (side == right)
			legsSprite->mirrored = true;
		bodySprite->packTag = PackTag::heroHit; bodySprite->packPart = PackPart::body; bodySprite->direction = DirectionsSystem::sideToDirection(spriteSide);
		legsSprite->packTag = PackTag::heroHit; legsSprite->packPart = PackPart::legs; legsSprite->direction = DirectionsSystem::sideToDirection(spriteSide);
		break;
	case absorbs:
		bodySprite->animationLength = 10;
		bodySprite->packTag = PackTag::heroAbsorb; bodySprite->packPart = PackPart::full; bodySprite->direction = spriteDirection;	
		break;
	case builds:
		bodySprite->animationLength = 10;
		bodySprite->packTag = PackTag::heroAbsorb; bodySprite->packPart = PackPart::full; bodySprite->direction = spriteDirection;
		break;
	case grab:
		bodySprite->animationLength = 11;
		animationSpeed = 15;
		bodySprite->packTag = PackTag::heroPick; bodySprite->packPart = PackPart::full; bodySprite->direction = spriteDirection;
		break;
	case dropping:
		bodyInverse = true;
		bodySprite->animationLength = 5;
		if (lastDirection == Direction::RIGHT || lastDirection == Direction::UPRIGHT || lastDirection == Direction::DOWNRIGHT)
			bodySprite->mirrored = true;
		bodySprite->packTag = PackTag::heroPick; bodySprite->packPart = PackPart::full; bodySprite->direction = spriteDirection;
		break;
	case transitionToEnotherWorld:
		bodySprite->animationLength = 18;
		break;
	case jerking:
		bodySprite->animationLength = 8;
		animationSpeed = 11;
		spriteDirection = lastDirection;
		if (lastDirection == Direction::RIGHT || lastDirection == Direction::UPRIGHT || lastDirection == Direction::DOWNRIGHT)
			bodySprite->mirrored = true;
		if (DirectionsSystem::cutRights(lastDirection) == Direction::UPLEFT || DirectionsSystem::cutRights(lastDirection) == Direction::DOWNLEFT)
			spriteDirection = Direction::LEFT;
		bodySprite->packTag = PackTag::heroRoll; bodySprite->packPart = PackPart::full; bodySprite->direction = DirectionsSystem::cutRights(spriteDirection);
		break;
	case open:
		bodySprite->animationLength = 12;
		bodySprite->packTag = PackTag::heroPick; bodySprite->packPart = PackPart::full; bodySprite->direction = spriteDirection;
		break;
	case relax:
		bodySprite->mirrored = false;
		bodySprite->animationLength = 16;
		animationSpeed = 13;
		spriteDirection = lastDirection;
		if (lastDirection == Direction::RIGHT || lastDirection == Direction::UPRIGHT || lastDirection == Direction::DOWNRIGHT)
			bodySprite->mirrored = true;
		if (DirectionsSystem::cutRights(lastDirection) == Direction::UPLEFT || DirectionsSystem::cutRights(lastDirection) == Direction::DOWNLEFT)
			spriteDirection = Direction::LEFT;
		bodySprite->packTag = PackTag::heroStand; bodySprite->packPart = PackPart::full; bodySprite->direction = DirectionsSystem::cutRights(spriteDirection);
		break;
	case move:
	{
		animationSpeed = speed / defaultSpeed * 12;
		if (animationSpeed < 10)
			animationSpeed = 10;
		bodySprite->animationLength = 8;

		auto finalDirection = lastDirection;
		if (smoothDirection != Direction::STAND)
			finalDirection = smoothDirection;

		if (finalDirection == Direction::RIGHT || finalDirection == Direction::UPRIGHT || finalDirection == Direction::DOWNRIGHT)
		{
			bodySprite->mirrored = true;
			legsSprite->mirrored = true;
		}

		bodySprite->packTag = PackTag::heroMove; bodySprite->packPart = PackPart::body; bodySprite->direction = DirectionsSystem::cutRights(finalDirection);
		legsSprite->packTag = PackTag::heroMove; legsSprite->packPart = PackPart::legs; legsSprite->direction = DirectionsSystem::cutRights(finalDirection);
		break;
	}
	case Actions::moveEnd:
		bodySprite->animationLength = 8;
		currentSprite[1] = 1;
		if (lastDirection == Direction::RIGHT || lastDirection == Direction::UPRIGHT || lastDirection == Direction::DOWNRIGHT)
		{
			bodySprite->mirrored = true;
			legsSprite->mirrored = true;
		}
		bodySprite->packTag = PackTag::heroMove; bodySprite->packPart = PackPart::body; bodySprite->direction = DirectionsSystem::cutRights(lastDirection);
		legsSprite->packTag = PackTag::heroHit; legsSprite->packPart = PackPart::legs; legsSprite->direction = DirectionsSystem::cutRights(lastDirection);
		
		break;
	}

	if (currentAction == moveHit)
	{
		animationSpeed = speed / defaultSpeed * 12;
		if (animationSpeed < 10)
			animationSpeed = 10;
		bodySprite->animationLength = 8;

		if (direction == Direction::UP && side == down || direction == Direction::DOWN && side == up)
		{
			legsInverse = true;
			spriteDirection = DirectionsSystem::sideToDirection(spriteSide);
		}
		if ((direction == Direction::LEFT || direction == Direction::UPLEFT || direction == Direction::DOWNLEFT) && side == right ||
			(direction == Direction::UPLEFT || direction == Direction::UPRIGHT) && side == down ||
			(direction == Direction::RIGHT || direction == Direction::UPRIGHT || direction == Direction::DOWNRIGHT) && side == left ||
			(direction == Direction::DOWNLEFT || direction == Direction::DOWNRIGHT) && side == up)
		{
			legsInverse = true;
			legsSprite->mirrored = !legsSprite->mirrored;
			spriteDirection = DirectionsSystem::invertDirection(spriteDirection);
		}
		if (direction == Direction::UP && side == down || direction == Direction::DOWN && side == up)
			legsInverse = true;

		legsSprite->packTag = PackTag::heroMove; legsSprite->packPart = PackPart::legs; legsSprite->direction = DirectionsSystem::cutRights(spriteDirection);
		bodySprite->packTag = PackTag::heroHit; bodySprite->packPart = PackPart::body; bodySprite->direction = DirectionsSystem::sideToDirection(spriteSide);

		if (direction == Direction::STAND)
		{
			if (side == right)
				legsSprite->mirrored = true;
			legsSprite->packTag = PackTag::heroMove; legsSprite->packPart = PackPart::legs; legsSprite->direction = DirectionsSystem::sideToDirection(spriteSide);
			legsSprite->animationLength = 14;
		}
	}

	if (currentAction == throwNoose)
	{
		bodySprite->animationLength = 14;
		if (direction == Direction::UP && side == down || direction == Direction::DOWN && side == up)
		{
			legsInverse = true;
			spriteDirection = DirectionsSystem::sideToDirection(spriteSide);
		}
		if ((direction == Direction::LEFT || direction == Direction::UPLEFT || direction == Direction::DOWNLEFT) && side == right ||
			(direction == Direction::RIGHT || direction == Direction::UPRIGHT || direction == Direction::DOWNRIGHT) && side == left)
		{
			legsInverse = true;
			legsSprite->mirrored = !legsSprite->mirrored;
		}

		legsSprite->packTag = PackTag::heroMove; legsSprite->packPart = PackPart::legs, legsSprite->direction = spriteDirection;
		bodySprite->packTag = PackTag::heroThrow; bodySprite->packPart = PackPart::body; bodySprite->direction = DirectionsSystem::sideToDirection(spriteSide);

		if (direction == Direction::STAND)
		{
			if (side == right)
				legsSprite->mirrored = true;
			legsSprite->packTag = PackTag::heroThrow; legsSprite->packPart = PackPart::legs, legsSprite->direction = DirectionsSystem::sideToDirection(spriteSide);
			legsSprite->animationLength = 14;
		}
	}

	//if (speedLineDirection != Direction::STAND)
		//result.push_back(speedLine);

	if (legsSprite->packTag != PackTag::empty)
	{
		if (legsInverse)
			legsSprite->number = legsSprite->animationLength + 1 - currentSprite[1];
		else
			legsSprite->number = currentSprite[1];
		result.push_back(legsSprite);
	}

	if (bodySprite->packTag != PackTag::empty)
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
			if (currentSprite[0] >= bodySprite->animationLength)
				lastAction = currentAction;
			if (++currentSprite[0] > bodySprite->animationLength)
				currentSprite[0] = 1;
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
