#include "deerchant.h"

#include "brazier.h"
#include "empty_object.h"
#include "helper.h"
#include "noose.h"
#include "picked_object.h"

using namespace sf;

deerchant::deerchant(std::string objectName, const Vector2f centerPosition) : dynamic_object(std::move(objectName), centerPosition)
{
	current_sprite_.resize(3);
	for (auto& number : current_sprite_)
		number = 1;	
	time_for_new_sprite_ = 0;
	moveSystem.default_speed = 0.0006f;
	moveSystem.speed = moveSystem.default_speed;
	animation_speed_ = 0.0010f;
	radius_ = 50;
	moveEndDistance = 55;
	hitDistance = 50;
	strikingSprite = 6;
	strength = 10;
	max_health_point_value_ = 100;
	health_point_ = max_health_point_value_;
	currentAction = relax;
	to_save_name_ = "hero";
	tag = entity_tag::hero;
	moveSystem.can_crash_into_static = true;

	for (auto i = 0; i < 3; i++)
		bags.emplace_back();
	
	const auto closedBagSize = Vector2f(helper::GetScreenSize().x / 12, helper::GetScreenSize().y / 6);
	
	bags[0].initialize(Vector2f(helper::GetScreenSize().x - closedBagSize.x, closedBagSize.y), true);
	bags[1].initialize(Vector2f(helper::GetScreenSize().x - closedBagSize.x, 2.0f * closedBagSize.y), true);
	bags[2].initialize(Vector2f(helper::GetScreenSize().x - closedBagSize.x * 1.5f, 1.5f * closedBagSize.y), true);
}

deerchant::~deerchant()
= default;

Vector2f deerchant::calculate_texture_offset()
{
	conditional_size_units_ = { 375, 375 };
	texture_box_.width = float(texture_box_.width)*get_scale_ratio().x;
	texture_box_.height = float(texture_box_.height)*get_scale_ratio().y;
	return { texture_box_.width / 2, texture_box_.height * 4 / 5 };
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

	auto pushPosition = position_;

	if (directionSystem.direction != direction::STAND)
	{
		setMoveOffset(10000);
		pushPosition = position_ - moveSystem.move_offset;
		if (invertDirection)
			pushPosition = position_ + moveSystem.move_offset;
	}
	else
	{
		pushPosition = lastPosition;
		if (invertDirection)
		{
			pushPosition.x += (position_.x - lastPosition.x) * 2;
			pushPosition.y += (position_.y - lastPosition.y) * 2;
		}
	}

	if (animate)
		changeAction(actions::moveEnd, false, false);
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

	if (Keyboard::isKeyPressed(Keyboard::Space) && currentAction != jerking && directionSystem.direction != direction::STAND)
	{
		jerk(3, 2);
		return;
	}
	//---------------------

	setHitDirection();
	moveSystem.move_offset = Vector2f(-1, -1);
	directionSystem.direction = direction::STAND;
	if (Keyboard::isKeyPressed(Keyboard::A))					
		directionSystem.direction = direction::LEFT;
	if (Keyboard::isKeyPressed(Keyboard::D))						
		directionSystem.direction = direction::RIGHT;
	if (Keyboard::isKeyPressed(Keyboard::W))							
		directionSystem.direction = direction::UP;
	if (Keyboard::isKeyPressed(Keyboard::S))								
		directionSystem.direction = direction::DOWN;
	if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::W))
		directionSystem.direction = direction::UPLEFT;
	if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::W))
		directionSystem.direction = direction::UPRIGHT;
	if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::S))
		directionSystem.direction = direction::DOWNLEFT;
	if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S))
		directionSystem.direction = direction::DOWNRIGHT;
	if (directionSystem.direction != direction::STAND && currentAction != actions::moveEnd)
	{
 		for (auto i = 0u; i < moveSystem.bumped_positions.size(); i++)
			if (moveSystem.bumped_positions[i].cancelable)
				moveSystem.bumped_positions.erase(moveSystem.bumped_positions.begin() + i);
		moveSystem.push_away(0);

		if (directionSystem.direction != directionSystem.last_direction)
		{
			calculateSpeedLineDirection(directionSystem.last_direction, directionSystem.direction);
			current_sprite_[2] = 1;
		}

		animationSmooth();
		directionSystem.last_direction = directionSystem.direction;
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
				isIntersect = (sqrt(pow(this->position_.x - laxMovePosition.x, 2) + pow(this->position_.y - laxMovePosition.y, 2)) <= (this->radius_ + boundTarget->get_radius()));
			if (isIntersect || !boundTarget && currentAction != actions::moveEnd)
			{				
				directionSystem.direction = direction::STAND;
				if (currentAction == actions::move || currentAction == actions::moveEnd)
				{
					directionSystem.calculate_direction();
					changeAction(relax, true, false);
				}
			}
			else
			{
				if (boundTarget/* && Helper::getDist(position, laxMovePosition) > (this->radius + boundTarget->getRadius())*/)
				{
					directionSystem.calculate_direction();
					setMoveOffset(0);
					changeAction(move, !(currentAction == move || currentAction == actions::moveEnd), false);
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

	if (directionSystem.direction != direction::STAND && currentAction != moveHit && !Mouse::isButtonPressed(Mouse::Left))
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
		if (directionSystem.direction != direction::STAND)
		{			
			if (currentAction != moveHit && currentAction != commonHit)
				current_sprite_[0] = 1;
			if (currentAction != move && currentAction != moveHit)
				current_sprite_[1] = 1;
			changeAction(moveHit, false, false);
		}
		else
			changeAction(commonHit, !(currentAction == moveHit || currentAction == commonHit || currentAction == actions::moveEnd), false);
	}
}

void deerchant::calculateSpeedLineDirection(direction lastDirection, direction direction)
{
	mirroredSpeedLine = false;
	reverseSpeedLine = false;

	speedLineDirection = direction::STAND;

	if (direction == direction::LEFT && lastDirection == direction::RIGHT || direction == direction::RIGHT && lastDirection == direction::LEFT)
	{
		speedLineDirection = direction::UPLEFT;
		return;
	}

	if (direction == direction::DOWNLEFT)
	{
		if (lastDirection == direction::UPRIGHT || lastDirection == direction::RIGHT || lastDirection == direction::DOWNRIGHT || lastDirection == direction::DOWN)
			speedLineDirection = direction::DOWN;
		else
		{
			speedLineDirection = direction::RIGHT;
			mirroredSpeedLine = true;
		}
	}

	if (direction == direction::DOWNRIGHT)
	{
		if (lastDirection == direction::UPLEFT || lastDirection == direction::LEFT || lastDirection == direction::DOWNLEFT || lastDirection == direction::DOWN)
			speedLineDirection = direction::DOWN;
		else
		{
			speedLineDirection = direction::RIGHT;
			mirroredSpeedLine = true;
		}
	}

	if (direction == direction::LEFT)
	{
		if (lastDirection == direction::RIGHT || lastDirection == direction::DOWNRIGHT || lastDirection == direction::DOWN || lastDirection == direction::DOWNLEFT)
			speedLineDirection = direction::LEFT;
		else
		{
			speedLineDirection = direction::LEFT;
			mirroredSpeedLine = true;
		}
	}


	if (direction == direction::UPLEFT)
	{
		if (lastDirection == direction::DOWNRIGHT || lastDirection == direction::DOWN || lastDirection == direction::DOWNLEFT || lastDirection == direction::LEFT)
			speedLineDirection = direction::RIGHT;
		else
		{
			speedLineDirection = direction::UPLEFT;
			mirroredSpeedLine = true;
		}
	}

	if (direction == direction::UP)
	{
		if (lastDirection == direction::DOWN || lastDirection == direction::DOWNLEFT || lastDirection == direction::LEFT || lastDirection == direction::UPLEFT)
			speedLineDirection = direction::UP;
		else
		{
			speedLineDirection = direction::UP;
			mirroredSpeedLine = true;
		}
	}

	if (direction == direction::UPRIGHT)
	{
		if (lastDirection == direction::DOWNLEFT || lastDirection == direction::LEFT || lastDirection == direction::UPLEFT || lastDirection == direction::UP)
			speedLineDirection = direction::UPLEFT;
		else
		{
			speedLineDirection = direction::RIGHT;
			mirroredSpeedLine = true;
		}
	}

	if (direction == direction::RIGHT)
	{
		if (lastDirection == direction::LEFT || lastDirection == direction::UPLEFT || lastDirection == direction::UP || lastDirection == direction::UPRIGHT)
			speedLineDirection = direction::LEFT;
		else
		{
			speedLineDirection = direction::LEFT;
			mirroredSpeedLine = true;
		}
	}

	if (direction == direction::DOWNRIGHT)
	{
		if (lastDirection == direction::UPLEFT || lastDirection == direction::UP || lastDirection == direction::UPRIGHT || lastDirection == direction::RIGHT)
			speedLineDirection = direction::RIGHT;
		else
		{
			speedLineDirection = direction::DOWN;
			mirroredSpeedLine = true;
		}
	}
		
	if (direction == direction::DOWN)
	{
		if (lastDirection == direction::UP || lastDirection == direction::UPRIGHT || lastDirection == direction::RIGHT || lastDirection == direction::DOWNRIGHT)
		{
			speedLineDirection = direction::UP;
			mirroredSpeedLine = true;
		}
		else
			speedLineDirection = direction::UP;				
	}
}

void deerchant::changeAction(const actions newAction, const bool resetSpriteNumber, const bool rememberLastAction)
{
	if (rememberLastAction)
		lastAction = currentAction;

	currentAction = newAction;

	if (resetSpriteNumber)
		for (auto& number : current_sprite_)
			number = 1;
}

void deerchant::stopping(const bool doStand, const bool forgetBoundTarget, const bool offUnsealInventory)
{
	if (boundTarget != nullptr && currentAction != dropping)
		if (boundTarget->get_name() == "droppedBag")
			for (auto& bag : bags)
				if (bag.current_state == ejected)
					bag.current_state = bag_closed;

	if (doStand)
	{
		this->laxMovePosition = { -1, -1 };
		moveSystem.move_offset = { -1, -1 };
		directionSystem.direction = directionSystem.last_direction;
	}

	if (forgetBoundTarget && boundTarget != nullptr)
	{
		boundTarget->is_processed = false;
		boundTarget = nullptr;
	}

	if (offUnsealInventory)
	{
		if (unsealInventoryOwner != nullptr)
			unsealInventoryOwner->is_visible_inventory = false;
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
	if (helper::getDist(position_, target->get_position()) <= radius_ + target->get_radius())
		moveSystem.push_by_bumping(target->get_position(), target->get_radius(), target->getMoveSystem().can_crash_into_dynamic);

	const auto isIntersect = helper::getDist(position_, target->get_position()) <= this->radius_ + target->get_radius() + hitDistance;

	if (isIntersect && direction_system::calculate_side(position_, target->get_position()) != direction_system::invert_side(directionSystem.side))
	{
		if ((this->currentAction == commonHit || this->currentAction == moveHit) && (this->get_sprite_number() == 4 || this->get_sprite_number() == 5 || this->get_sprite_number() == 8))	
			target->take_damage(this->getStrength(), position_);		
	}
}

void deerchant::behaviorWithStatic(world_object* target, long long elapsedTime)
{
	if (!target)
		return;

	if (target->tag == entity_tag::wreathTable && helper::getDist(position_, target->get_position()) <= radius_ + target->get_radius())
		nearTheTable = true;	
}

void deerchant::behavior(const long long elapsedTime)
{
	jerkInteract(elapsedTime);
	endingPreviousAction();
	fightInteract(elapsedTime);
	speedInteract(elapsedTime);
	animationSmoothInteract(elapsedTime);
	lastPosition = position_;	
	
	// interactions with target
	if (!boundTarget || boundTarget->is_processed)
	{
		laxMovePosition = Vector2f(-1, -1);
		return;
	}	

	if (currentAction != jerking && boundTarget->is_processed)
		laxMovePosition = boundTarget->get_position();

	const auto isIntersect = (helper::getDist(position_, laxMovePosition)) <= (this->radius_ + boundTarget->get_radius());

	//touch selected object 
	if (isIntersect)
	{
		boundTarget->is_processed = true;
		switch (boundTarget->tag)
		{
		case entity_tag::tree:
		{
			if (boundTarget->get_state() == absorbed)
				break;
			changeAction(absorbs, true, false);
			current_sprite_[0] = 1;
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			boundTarget->set_state(absorbed);
			boundTarget->is_processed = false;
			stopping(true);
			break;
		}
		case entity_tag::fern:
		{
			if (!boundTarget->inventory.empty())
			{
				changeAction(open, true, false);
				unsealInventoryOwner = boundTarget;
				boundTarget->is_processed = false;
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
			boundTarget->is_processed = false;
			stopping(true);
			break;
		}
		case entity_tag::buildObject:
		{
			changeAction(builds, true, false);
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			boundTarget->is_processed = false;
			stopping(true, true);
			break;
		}
		default:
		{
			changeAction(relax, true, false);
			if (boundTarget)
			{
				boundTarget->is_processed = false;
				//side = calculateSide(boundTarget->getPosition(), elapsedTime);
				stopping(true, false);
			}
			break;
		}
		}
	}
	else
	{
		boundTarget->is_processed = false;
	}
}

void deerchant::onMouseUp(const int currentMouseButton, world_object *mouseSelectedObject, const Vector2f mouseWorldPos, const bool isBuilding)
{
	if (isBuilding && currentMouseButton == 2)
	{
		if (boundTarget != nullptr)
		{
			boundTarget->is_processed = false;
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
			helper::getDist(brazier->getPlatePosition(), position_) <= brazier->getPlateRadius() + radius_)
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
				boundTarget->is_processed = false;
				boundTarget = nullptr;				
			}
			boundTarget = new empty_object("droppedItem", mouseWorldPos);
			boundTarget->tag = entity_tag::dropPoint;
			laxMovePosition = mouseWorldPos;
			return;
		}

		for (auto& bag : bags)
		{
			if (bag.current_state == ejected)
			{
				if (boundTarget != nullptr)
				{
					boundTarget->is_processed = false;
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
		laxMovePosition = mouseSelectedObject->get_position();
	}
}

void deerchant::endingPreviousAction()
{
	if (lastAction == commonHit && !Mouse::isButtonPressed(Mouse::Left))
	{
		directionSystem.last_direction = direction_system::side_to_direction(directionSystem.side);
		changeAction(relax, true, false);
	}
	if (lastAction == moveHit && !Mouse::isButtonPressed(Mouse::Left))
	{
		directionSystem.last_direction = direction_system::side_to_direction(directionSystem.side);
		changeAction(move, true, false);
	}
	if (lastAction == actions::moveEnd)
	{
		changeAction(relax, true, false);
	}
	if (lastAction == open)
		changeAction(relax, true, false);

	if (lastAction == absorbs)
	{
		boundTarget->is_processed = false;
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
			dropObject.position = { boundTarget->get_position().x, boundTarget->get_position().y };
			dropObject.tag = entity_tag::droppedLoot;
			dropObject.type_of_object = int(heldItem->content.first);
			dropObject.count = heldItem->content.second;
			birth_statics_.push(dropObject);
			heldItem->content = std::make_pair(entity_tag::emptyCell, 0);
		}
		else
		{
			for (auto cnt = 0u; cnt != bags.size(); cnt++)			
				if (bags[cnt].current_state == ejected)
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
						dropObject.type_of_object = int(entity_tag::heroBag);	
					}
					dropObject.position = { position_.x, position_.y + radius_ };
					dropObject.inventory = hero_bag::cells_to_inventory(bags[cnt].cells);					
					//bags[cnt].~HeroBag();
					bags.erase(bags.begin() + cnt);
					birth_statics_.push(dropObject);
					break;
				}	
		}
		stopping(true, true);
	}
	if (currentAction == throwNoose && current_sprite_[0] == 12 && heldItem->content == std::make_pair(entity_tag::noose, 1))
	{
		heldItem->content = { entity_tag::emptyCell, 0 };
		birth_dynamic_info nooseObject;
		nooseObject.position = position_;
		nooseObject.tag = entity_tag::noose;
		nooseObject.owner = this;
		birth_dynamics_.push(nooseObject);
	}
    if (lastAction == throwNoose)
    {
		directionSystem.last_direction = direction_system::side_to_direction(directionSystem.side);
		changeAction(relax, true, false);
    }
	if (lastAction == open)
	{
		if (unsealInventoryOwner)
		{
			unsealInventoryOwner->is_visible_inventory = true;
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
				if (pickedItem->get_type() == int(entity_tag::heroBag) || pickedItem->getId() == entity_tag::hareTrap)
				{					
					bags.resize(bags.size() + 1);
					bags[bags.size() - 1] = *(new hero_bag());
					bags[bags.size()-1].initialize(Vector2f(helper::GetScreenSize().x / 2, helper::GetScreenSize().y / 2), true, pickedItem->inventory);
					pickedItem->delete_promise_on();
				}
				else
					pickedItem->pickUp(&this->bags);
			}
			
			auto nooseItem = dynamic_cast<noose*>(boundTarget);
			if (nooseItem)
			{
				const auto placedNoose = new std::vector<std::pair<entity_tag, int>>({ (std::make_pair(entity_tag::noose, 1)) });
				if (hero_bag::put_items_in(placedNoose, &bags))
					nooseItem->delete_promise_on();
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
	if (directionSystem.direction == direction::UP && directionSystem.last_direction == direction::LEFT ||
		directionSystem.direction == direction::LEFT && directionSystem.last_direction == direction::UP)
	{
		smoothDirections = { direction::UPLEFT, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::DOWN && directionSystem.last_direction == direction::LEFT ||
		directionSystem.direction == direction::LEFT && directionSystem.last_direction == direction::DOWN)
	{
		smoothDirections = { direction::DOWNLEFT, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::UP && directionSystem.last_direction == direction::RIGHT ||
		directionSystem.direction == direction::RIGHT && directionSystem.last_direction == direction::UP)
	{
		smoothDirections = { direction::UPRIGHT, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::DOWN && directionSystem.last_direction == direction::RIGHT ||
		directionSystem.direction == direction::RIGHT && directionSystem.last_direction == direction::DOWN)
	{
		smoothDirections = { direction::DOWNRIGHT, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::UP && directionSystem.last_direction == direction::DOWN)
	{
		smoothDirections = { direction::DOWNLEFT, direction::LEFT, direction::UPLEFT, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::DOWN && directionSystem.last_direction == direction::UP){
		smoothDirections = { direction::UPRIGHT, direction::RIGHT, direction::DOWNRIGHT, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::LEFT && directionSystem.last_direction == direction::RIGHT)
	{		
		smoothDirections = { direction::DOWNRIGHT, direction::DOWN, direction::DOWNLEFT, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::RIGHT && directionSystem.last_direction == direction::LEFT)
	{
		smoothDirections = { direction::UPLEFT, direction::UP, direction::UPRIGHT, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::UPLEFT && directionSystem.last_direction == direction::DOWNRIGHT)
	{
		smoothDirections = { direction::RIGHT, direction::UPRIGHT, direction::UPRIGHT, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::DOWNRIGHT && directionSystem.last_direction == direction::UPLEFT)
	{
		smoothDirections = { direction::UP, direction::UPRIGHT, direction::RIGHT, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::UPRIGHT && directionSystem.last_direction == direction::DOWNLEFT)
	{
		smoothDirections = { direction::LEFT, direction::UPLEFT, direction::UP, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::DOWNLEFT && directionSystem.last_direction == direction::UPRIGHT)
	{
		smoothDirections = { direction::UP, direction::UPLEFT, direction::LEFT, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::UPLEFT && directionSystem.last_direction == direction::UPRIGHT ||
		directionSystem.direction == direction::UPRIGHT && directionSystem.last_direction == direction::UPLEFT)
	{
		smoothDirections = { direction::UP, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::UPLEFT && directionSystem.last_direction == direction::DOWNLEFT ||
		directionSystem.direction == direction::DOWNLEFT && directionSystem.last_direction == direction::UPLEFT)
	{
		smoothDirections = { direction::LEFT, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::UPRIGHT && directionSystem.last_direction == direction::DOWNRIGHT ||
		directionSystem.direction == direction::DOWNRIGHT && directionSystem.last_direction == direction::UPRIGHT)
	{
		smoothDirections = { direction::RIGHT, direction::STAND };
		return;
	}
	if (directionSystem.direction == direction::DOWNLEFT && directionSystem.last_direction == direction::DOWNRIGHT ||
		directionSystem.direction == direction::DOWNRIGHT && directionSystem.last_direction == direction::DOWNLEFT)
	{
		smoothDirections = { direction::DOWN, direction::STAND };
	}

}

void deerchant::animationSmoothInteract(long long elapsedTime)
{
	smoothMoveTime += elapsedTime;

	if (smoothDirections.empty())
	{
		smoothDirection = direction::STAND;
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
	if (!(currentAction == move || currentAction == moveHit || currentAction == actions::moveEnd || currentAction == throwNoose))
	{
		moveTime = 0;
		return;
	}

	moveTime += elapsedTime;

	const long long speedIncreaseTime = long(5e5);
	const auto partOfSpeed = 0.3f;
	
	if (moveTime < speedIncreaseTime)
		moveSystem.speed = (partOfSpeed + float(moveTime) * (1 - partOfSpeed) / speedIncreaseTime) * moveSystem.default_speed;
	else
		moveSystem.speed = moveSystem.default_speed;
}

void deerchant::jerkInteract(const long long elapsedTime)
{
	if (currentAction == jerking)
	{
		if (jerkTime > 0)
		{
			jerkTime -= elapsedTime;
			moveSystem.speed = jerkDistance / float(jerkDuration) * jerkPower * pow(float(jerkTime) / float(jerkDuration), jerkDeceleration);
			moveSystem.speed = std::max(moveSystem.default_speed / jerkDeceleration, moveSystem.speed);
		}
		else
		{
			changeAction(relax, true, false);
			moveSystem.speed = moveSystem.default_speed;
		}
	}
}

Vector2f deerchant::get_build_position(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

Vector2f deerchant::getBeltPosition() const
{
	/*if (additionalSprites.size() >= 2) return
		Vector2f((4 * additionalSprites[0].position.x + additionalSprites[1].position.x) / 5.0f + conditionalSizeUnits.x / 3.0f,
		(4 * additionalSprites[0].position.y + additionalSprites[1].position.y) / 5.0f);
	return additionalSprites[0].position;*/
    return position_;
}

int deerchant::get_build_type(Vector2f, Vector2f)
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
	current_sprite_[0] = 1;

	laxMovePosition = Vector2f(position_.x + cos(float(directionSystem.direction) * pi / 180) * jerkDistance, position_.y - sin(float(directionSystem.direction) * pi / 180) * jerkDistance);
}

void deerchant::fightInteract(long long elapsedTime, dynamic_object* target)
{
	timeAfterHitSelf += elapsedTime;
	moveSystem.push_away(elapsedTime);
}

sprite_chain_element* deerchant::prepareSpeedLine()
{
	auto speedLine = new sprite_chain_element(pack_tag::heroMove, pack_part::lines, direction::STAND, 1, position_, conditional_size_units_, Vector2f(texture_box_offset_));
	speedLine->animation_length = 3;
	if (speedLineDirection == direction::STAND || current_sprite_[2] > speedLine->animation_length)
		return speedLine;

	speedLine->direction = speedLineDirection;
	speedLine->mirrored = mirroredSpeedLine;
	speedLine->offset.y += conditional_size_units_.y / 9.0f;
	speedLine->position = Vector2f(position_.x, position_.y + conditional_size_units_.y / 9.0f);

	if (reverseSpeedLine)
		speedLine->number = speedLine->animation_length + 1 - current_sprite_[2];
	else
		speedLine->number = current_sprite_[2];

	return speedLine;
}

std::vector<sprite_chain_element*> deerchant::prepare_sprites(long long elapsedTime)
{
	auto legsSprite = new sprite_chain_element(Vector2f(position_.x, position_.y - 1), conditional_size_units_, { texture_box_offset_.x, texture_box_offset_.y + 1 }, color);
	auto* bodySprite = new sprite_chain_element(position_, conditional_size_units_, texture_box_offset_, color);

	const auto speedLine = prepareSpeedLine();
	std::vector<sprite_chain_element*> result = {};
	auto legsInverse = false, bodyInverse = false;
	legsSprite->animation_length = 8;

    auto spriteSide = directionSystem.side; auto spriteDirection = directionSystem.last_direction;

	animation_speed_ = 12;

	if (directionSystem.direction == direction::RIGHT || directionSystem.direction == direction::UPRIGHT || directionSystem.direction == direction::DOWNRIGHT)
	{
		spriteDirection = direction_system::cut_rights(spriteDirection);
		legsSprite->mirrored = true;
	}

	if (directionSystem.side == right && currentAction != move && currentAction != actions::moveEnd && currentAction != jerking)
	{
		spriteSide = left;
		bodySprite->mirrored = true;
	}

	switch (currentAction)
	{
	case commonHit:
		bodySprite->animation_length = 8;
		if (directionSystem.side == right)
			legsSprite->mirrored = true;
		bodySprite->pack_tag = pack_tag::heroHit; bodySprite->pack_part = pack_part::body; bodySprite->direction = direction_system::side_to_direction(spriteSide);
		legsSprite->pack_tag = pack_tag::heroHit; legsSprite->pack_part = pack_part::legs; legsSprite->direction = direction_system::side_to_direction(spriteSide);
		break;
	case absorbs:
		bodySprite->animation_length = 10;
		bodySprite->pack_tag = pack_tag::heroAbsorb; bodySprite->pack_part = pack_part::full; bodySprite->direction = direction_system::cut_diagonals(spriteDirection);	
		break;
	case builds:
		bodySprite->animation_length = 10;
		bodySprite->pack_tag = pack_tag::heroAbsorb; bodySprite->pack_part = pack_part::full; bodySprite->direction = direction_system::cut_diagonals(spriteDirection);
		break;
	case grab:
		bodySprite->animation_length = 11;
		animation_speed_ = 15;
		bodySprite->pack_tag = pack_tag::heroPick; bodySprite->pack_part = pack_part::full; bodySprite->direction = direction_system::cut_diagonals(spriteDirection);
		break;
	case dropping:
		bodyInverse = true;
		bodySprite->animation_length = 5;
		if (directionSystem.last_direction == direction::RIGHT || directionSystem.last_direction == direction::UPRIGHT || directionSystem.last_direction == direction::DOWNRIGHT)
			bodySprite->mirrored = true;
		bodySprite->pack_tag = pack_tag::heroPick; bodySprite->pack_part = pack_part::full; bodySprite->direction = direction_system::cut_diagonals(spriteDirection);
		break;
	case open:
		bodySprite->animation_length = 12;
		bodySprite->pack_tag = pack_tag::heroPick; bodySprite->pack_part = pack_part::full; bodySprite->direction = direction_system::cut_diagonals(spriteDirection);
		break;
	case transitionToEnotherWorld:
		bodySprite->animation_length = 18;
		break;
	case jerking:
		bodySprite->animation_length = 8;
		animation_speed_ = 11;
		spriteDirection = directionSystem.last_direction;
		if (directionSystem.last_direction == direction::RIGHT || directionSystem.last_direction == direction::UPRIGHT || directionSystem.last_direction == direction::DOWNRIGHT)
			bodySprite->mirrored = true;
		if (direction_system::cut_rights(directionSystem.last_direction) == direction::UPLEFT || direction_system::cut_rights(directionSystem.last_direction) == direction::DOWNLEFT)
			spriteDirection = direction::LEFT;
		bodySprite->pack_tag = pack_tag::heroRoll; bodySprite->pack_part = pack_part::full; bodySprite->direction = direction_system::cut_rights(spriteDirection);
		break;
	case relax:
		bodySprite->mirrored = false;
		bodySprite->animation_length = 16;
		animation_speed_ = 13;
		spriteDirection = directionSystem.last_direction;
		if (directionSystem.last_direction == direction::RIGHT || directionSystem.last_direction == direction::UPRIGHT || directionSystem.last_direction == direction::DOWNRIGHT)
			bodySprite->mirrored = true;
		if (direction_system::cut_rights(directionSystem.last_direction) == direction::UPLEFT || direction_system::cut_rights(directionSystem.last_direction) == direction::DOWNLEFT)
			spriteDirection = direction::LEFT;
		bodySprite->pack_tag = pack_tag::heroStand; bodySprite->pack_part = pack_part::full; bodySprite->direction = direction_system::cut_rights(spriteDirection);
		break;
	case move:
	{
		animation_speed_ = moveSystem.speed / moveSystem.default_speed * 12;
		if (animation_speed_ < 10)
			animation_speed_ = 10;
		bodySprite->animation_length = 8;

		auto finalDirection = directionSystem.last_direction;
		if (smoothDirection != direction::STAND)
			finalDirection = smoothDirection;

		if (finalDirection == direction::RIGHT || finalDirection == direction::UPRIGHT || finalDirection == direction::DOWNRIGHT)
		{
			bodySprite->mirrored = true;
			legsSprite->mirrored = true;
		}

		bodySprite->pack_tag = pack_tag::heroMove; bodySprite->pack_part = pack_part::body; bodySprite->direction = direction_system::cut_rights(finalDirection);
		legsSprite->pack_tag = pack_tag::heroMove; legsSprite->pack_part = pack_part::legs; legsSprite->direction = direction_system::cut_rights(finalDirection);
		break;
	}
	case actions::moveEnd:
		bodySprite->animation_length = 8;
		current_sprite_[1] = 1;
		if (directionSystem.last_direction == direction::RIGHT || directionSystem.last_direction == direction::UPRIGHT || directionSystem.last_direction == direction::DOWNRIGHT)
		{
			bodySprite->mirrored = true;
			legsSprite->mirrored = true;
		}
		bodySprite->pack_tag = pack_tag::heroMove; bodySprite->pack_part = pack_part::body; bodySprite->direction = direction_system::cut_rights(directionSystem.last_direction);
		directionSystem.last_direction = direction_system::cut_diagonals(directionSystem.last_direction);
		legsSprite->pack_tag = pack_tag::heroHit; legsSprite->pack_part = pack_part::legs; legsSprite->direction = direction_system::cut_rights(directionSystem.last_direction);
		
		break;
	}

	if (currentAction == moveHit)
	{
		animation_speed_ = moveSystem.speed / moveSystem.default_speed * 12;
		if (animation_speed_ < 10)
			animation_speed_ = 10;
		bodySprite->animation_length = 8;

		if (directionSystem.direction == direction::UP && directionSystem.side == down || directionSystem.direction == direction::DOWN && directionSystem.side == up)
		{
			legsInverse = true;
			spriteDirection = direction_system::side_to_direction(spriteSide);
		}
		if ((directionSystem.direction == direction::LEFT || directionSystem.direction == direction::UPLEFT || directionSystem.direction == direction::DOWNLEFT) && directionSystem.side == right ||
			(directionSystem.direction == direction::UPLEFT || directionSystem.direction == direction::UPRIGHT) && directionSystem.side == down ||
			(directionSystem.direction == direction::RIGHT || directionSystem.direction == direction::UPRIGHT || directionSystem.direction == direction::DOWNRIGHT) && directionSystem.side == left ||
			(directionSystem.direction == direction::DOWNLEFT || directionSystem.direction == direction::DOWNRIGHT) && directionSystem.side == up)
		{
			legsInverse = true;
			legsSprite->mirrored = !legsSprite->mirrored;
			spriteDirection = direction_system::invert_direction(spriteDirection);
		}
		if (directionSystem.direction == direction::UP && directionSystem.side == down || directionSystem.direction == direction::DOWN && directionSystem.side == up)
			legsInverse = true;

		legsSprite->pack_tag = pack_tag::heroMove; legsSprite->pack_part = pack_part::legs; legsSprite->direction = direction_system::cut_rights(spriteDirection);
		bodySprite->pack_tag = pack_tag::heroHit; bodySprite->pack_part = pack_part::body; bodySprite->direction = direction_system::side_to_direction(spriteSide);

		if (directionSystem.direction == direction::STAND)
		{
			if (directionSystem.side == right)
				legsSprite->mirrored = true;
			legsSprite->pack_tag = pack_tag::heroMove; legsSprite->pack_part = pack_part::legs; legsSprite->direction = direction_system::side_to_direction(spriteSide);
			legsSprite->animation_length = 14;
		}
	}

	if (currentAction == throwNoose)
	{
		bodySprite->animation_length = 14;
		if (directionSystem.direction == direction::UP && directionSystem.side == down || directionSystem.direction == direction::DOWN && directionSystem.side == up)
		{
			legsInverse = true;
			spriteDirection = direction_system::side_to_direction(spriteSide);
		}
		if ((directionSystem.direction == direction::LEFT || directionSystem.direction == direction::UPLEFT || directionSystem.direction == direction::DOWNLEFT) && directionSystem.side == right ||
			(directionSystem.direction == direction::RIGHT || directionSystem.direction == direction::UPRIGHT || directionSystem.direction == direction::DOWNRIGHT) && directionSystem.side == left)
		{
			legsInverse = true;
			legsSprite->mirrored = !legsSprite->mirrored;
		}

		legsSprite->pack_tag = pack_tag::heroMove; legsSprite->pack_part = pack_part::legs, legsSprite->direction = spriteDirection;
		bodySprite->pack_tag = pack_tag::heroThrow; bodySprite->pack_part = pack_part::body; bodySprite->direction = direction_system::side_to_direction(spriteSide);

		if (directionSystem.direction == direction::STAND)
		{
			if (directionSystem.side == right)
				legsSprite->mirrored = true;
			legsSprite->pack_tag = pack_tag::heroThrow; legsSprite->pack_part = pack_part::legs, legsSprite->direction = direction_system::side_to_direction(spriteSide);
			legsSprite->animation_length = 14;
		}
	}

	//if (speedLineDirection != Direction::STAND)
		//result.push_back(speedLine);

	if (legsSprite->pack_tag != pack_tag::empty)
	{
		if (legsInverse)
			legsSprite->number = legsSprite->animation_length + 1 - current_sprite_[1];
		else
			legsSprite->number = current_sprite_[1];
		result.push_back(legsSprite);
	}

	if (bodySprite->pack_tag != pack_tag::empty)
	{
		if (bodyInverse)
			bodySprite->number = bodySprite->animation_length + 1 - current_sprite_[0];
		else
			bodySprite->number = current_sprite_[0];
		result.push_back(bodySprite);
	}

	time_for_new_sprite_ += elapsedTime;

	if (time_for_new_sprite_ >= 1e6 / animation_speed_)
	{
		time_for_new_sprite_ = 0;

		if (currentAction == actions::moveEnd)
		{
			current_sprite_[0] = calculateNextMoveEndSprite(current_sprite_[0]);
			if (current_sprite_[0] == -1)
			{
				lastAction = currentAction;
				current_sprite_[0] = 1;
			}
		}
		else
		{
			if (++current_sprite_[0] > bodySprite->animation_length)
				current_sprite_[0] = 1;
			if (current_sprite_[0] >= bodySprite->animation_length)
				lastAction = currentAction;
			if (++current_sprite_[1] > legsSprite->animation_length)
				current_sprite_[1] = 1;
			if (current_sprite_[2] > speedLine->animation_length)
				speedLineDirection = direction::STAND;
			else
				current_sprite_[2]++;
		}
	}

	set_unscaled(result);

    return result;
}
