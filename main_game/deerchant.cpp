#include "deerchant.h"

#include "brazier.h"
#include "empty_object.h"
#include "helper.h"
#include "math_constants.h"
#include "noose.h"
#include "picked_object.h"

using namespace sf;

deerchant::deerchant(std::string objectName, const Vector2f centerPosition) : dynamic_object(std::move(objectName), centerPosition)
{
	current_sprite_.resize(3);
	for (auto& number : current_sprite_)
		number = 1;
	time_for_new_sprite_ = 0;
	move_system.default_speed = 0.0006f;
	move_system.speed = move_system.default_speed;
	animation_speed_ = 0.0010f;
	radius_ = 50;
	moveEndDistance = 55;
	hitDistance = 50;
	strikingSprite = 6;
	strength_ = 10;
	max_health_point_value_ = 100;
	health_point_ = max_health_point_value_;
	current_action_ = relax;
	to_save_name_ = "hero";
	tag = entity_tag::hero;
	move_system.can_crash_into_static = true;

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
	conditional_size_units_ = conditional_size;
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

	if (direction_system.direction != direction::STAND)
	{
		move_system.set_move_offset(10000);
		pushPosition = position_ - move_system.move_offset;
		if (invertDirection)
			pushPosition = position_ + move_system.move_offset;
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
		change_action(actions::move_end, false, false);
	else
		change_action(relax, true, false);

	//bumpedPositions.emplace_back(pushPosition, true);
	//bumpDistance += distance;
}

void deerchant::handle_input(const bool usedMouse, long long elapsed_time)
{
	//high-priority actions
	if (current_action_ == absorbs || current_action_ == grab || current_action_ == dropping || current_action_ == builds || current_action_ == jerking)
		return;

	if (current_action_ == throw_noose && heldItem->content != std::make_pair(entity_tag::noose, 1))
		change_action(relax, true, false);

	if (Keyboard::isKeyPressed(Keyboard::Space) && current_action_ != jerking && direction_system.direction != direction::STAND)
	{
		jerk(3, 2);
		return;
	}
	//---------------------

	setHitDirection();
	move_system.move_offset = Vector2f(-1, -1);
	direction_system.direction = direction::STAND;
	if (Keyboard::isKeyPressed(Keyboard::A))					
		direction_system.direction = direction::LEFT;
	if (Keyboard::isKeyPressed(Keyboard::D))						
		direction_system.direction = direction::RIGHT;
	if (Keyboard::isKeyPressed(Keyboard::W))							
		direction_system.direction = direction::UP;
	if (Keyboard::isKeyPressed(Keyboard::S))								
		direction_system.direction = direction::DOWN;
	if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::W))
		direction_system.direction = direction::UPLEFT;
	if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::W))
		direction_system.direction = direction::UPRIGHT;
	if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::S))
		direction_system.direction = direction::DOWNLEFT;
	if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S))
		direction_system.direction = direction::DOWNRIGHT;
	if (direction_system.direction != direction::STAND && current_action_ != actions::move_end)
	{
 		for (auto i = 0u; i < move_system.bumped_positions.size(); i++)
			if (move_system.bumped_positions[i].cancelable)
				move_system.bumped_positions.erase(move_system.bumped_positions.begin() + i);
		move_system.push_away(0);

		if (direction_system.direction != direction_system.last_direction)
		{
			calculateSpeedLineDirection(direction_system.last_direction, direction_system.direction);
			current_sprite_[2] = 1;
		}

		animationSmooth();
		direction_system.last_direction = direction_system.direction;
		wasPushedAfterMovement = false;
	}
	else
		if (!bound_target_ && current_action_ == move || current_action_ == move_hit)
			moveEnd(true);		

	if (current_action_ != throw_noose) //second priority actions, interact while moving
	{
		if (!(Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S)))
		{
			auto isIntersect = false;
			if (bound_target_)
				isIntersect = (sqrt(pow(this->position_.x - move_system.lax_move_position.x, 2) + pow(this->position_.y - move_system.lax_move_position.y, 2)) <= (this->radius_ + bound_target_->get_radius()));
			if (isIntersect || !bound_target_ && current_action_ != actions::move_end)
			{				
				direction_system.direction = direction::STAND;
				if (current_action_ == actions::move || current_action_ == actions::move_end)
				{
					direction_system.calculate_direction(elapsed_time);
					change_action(relax, true, false);
				}
			}
			else
			{
				if (bound_target_/* && Helper::getDist(position, laxMovePosition) > (this->radius + boundTarget->getRadius())*/)
				{
					direction_system.calculate_direction(elapsed_time);
					move_system.set_move_offset(0);
					change_action(move, !(current_action_ == move || current_action_ == actions::move_end), false);
				}
			}
		}
	}
	else
		return;

	//define actions
	if (Mouse::isButtonPressed(Mouse::Left) && heldItem->content.first == entity_tag::noose)
	{
		change_action(throw_noose, true, false);
		return;
	}

	if (direction_system.direction != direction::STAND && current_action_ != move_hit && !Mouse::isButtonPressed(Mouse::Left))
		change_action(move, current_action_ == relax/*, currentAction != move*/, false);

	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S) ||
		Keyboard::isKeyPressed(Keyboard::Z) || Keyboard::isKeyPressed(Keyboard::F) || Keyboard::isKeyPressed(Keyboard::E) || Keyboard::isKeyPressed(Keyboard::LControl) ||
		Keyboard::isKeyPressed(Keyboard::Space) || Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
	{
		if (bound_target_ != nullptr)
			stopping(true, true);
		if (!Mouse::isButtonPressed(Mouse::Left))
			stopping(false, false, true);
	}
	//--------------

	if (isBuildSystem)
		return;

	if (Mouse::isButtonPressed(Mouse::Left) && !usedMouse)
	{
		if (direction_system.direction != direction::STAND)
		{			
			if (current_action_ != move_hit && current_action_ != common_hit)
				current_sprite_[0] = 1;
			if (current_action_ != move && current_action_ != move_hit)
				current_sprite_[1] = 1;
			change_action(move_hit, false, false);
		}
		else
			change_action(common_hit, !(current_action_ == move_hit || current_action_ == common_hit || current_action_ == actions::move_end), false);
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

void deerchant::change_action(const actions newAction, const bool resetSpriteNumber, const bool rememberLastAction)
{
	if (rememberLastAction)
		last_action_ = current_action_;

	current_action_ = newAction;

	if (resetSpriteNumber)
		for (auto& number : current_sprite_)
			number = 1;
}

void deerchant::stopping(const bool doStand, const bool forgetBoundTarget, const bool offUnsealInventory)
{
	if (bound_target_ != nullptr && current_action_ != dropping)
		if (bound_target_->get_name() == "droppedBag")
			for (auto& bag : bags)
				if (bag.current_state == ejected)
					bag.current_state = bag_closed;

	if (doStand)
	{
		move_system.lax_move_position = { -1, -1 };
		move_system.move_offset = { -1, -1 };
		direction_system.direction = direction_system.last_direction;
	}

	if (forgetBoundTarget && bound_target_ != nullptr)
	{
		bound_target_->is_processed = false;
		bound_target_ = nullptr;
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
		direction_system.side = up;
	else
		if (mouseX >= xPos && abs(alpha) >= 0 && abs(alpha) <= 45)
			direction_system.side = right;
		else
			if (mouseY >= yPos && abs(alpha) >= 45 && abs(alpha) <= 90)
				direction_system.side = down;
			else
				if (mouseX <= xPos && abs(alpha) >= 0 && abs(alpha) <= 45)
					direction_system.side = left;
}

void deerchant::set_target(dynamic_object& object)
{
	nearTheTable = false;
}

void deerchant::behavior_with_dynamic(dynamic_object* target, long long elapsedTime)
{
	if (helper::getDist(position_, target->get_position()) <= radius_ + target->get_radius())
		move_system.push_by_bumping(target->get_position(), target->get_radius(), target->move_system.can_crash_into_dynamic);

	const auto isIntersect = helper::getDist(position_, target->get_position()) <= this->radius_ + target->get_radius() + hitDistance;

	if (isIntersect && direction_system.calculate_side(position_, target->get_position(), elapsedTime) != direction_system::invert_side(direction_system.side))
	{
		if ((this->current_action_ == common_hit || this->current_action_ == move_hit) && (this->get_sprite_number() == 4 || this->get_sprite_number() == 5 || this->get_sprite_number() == 8))	
			target->take_damage(this->get_strength(), position_);		
	}
}

void deerchant::behavior_with_static(world_object* target, long long elapsedTime)
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
	fight_interact(elapsedTime);
	speedInteract(elapsedTime);
	animationSmoothInteract(elapsedTime);
	lastPosition = position_;	
	
	// interactions with target
	if (!bound_target_ || bound_target_->is_processed)
	{
		move_system.lax_move_position = Vector2f(-1, -1);
		return;
	}	

	if (current_action_ != jerking && bound_target_->is_processed)
		move_system.lax_move_position = bound_target_->get_position();

	const auto isIntersect = (helper::getDist(position_, move_system.lax_move_position)) <= (this->radius_ + bound_target_->get_radius());

	//touch selected object 
	if (isIntersect)
	{
		bound_target_->is_processed = true;
		switch (bound_target_->tag)
		{
		case entity_tag::tree:
		{
			if (bound_target_->get_state() == absorbed)
				break;
			change_action(absorbs, true, false);
			current_sprite_[0] = 1;
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			bound_target_->set_state(absorbed);
			bound_target_->is_processed = false;
			stopping(true);
			break;
		}
		case entity_tag::fern:
		{
			if (!bound_target_->inventory.empty())
			{
				change_action(open, true, false);
				unsealInventoryOwner = bound_target_;
				bound_target_->is_processed = false;
				stopping(true, true);
			}
			else
			{
				change_action(grab, true, false);
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
			change_action(grab, true, false);
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			stopping(true);
			break;
		}
		case entity_tag::dropPoint:
		{		
			change_action(dropping, true, false);
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			bound_target_->is_processed = false;
			stopping(true);
			break;
		}
		case entity_tag::buildObject:
		{
			change_action(builds, true, false);
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			bound_target_->is_processed = false;
			stopping(true, true);
			break;
		}
		default:
		{
			change_action(relax, true, false);
			if (bound_target_)
			{
				bound_target_->is_processed = false;
				//side = calculateSide(boundTarget->getPosition(), elapsedTime);
				stopping(true, false);
			}
			break;
		}
		}
	}
	else
	{
		bound_target_->is_processed = false;
	}
}

void deerchant::onMouseUp(const int currentMouseButton, world_object *mouseSelectedObject, const Vector2f mouseWorldPos, const bool isBuilding)
{
	if (isBuilding && currentMouseButton == 2)
	{
		if (bound_target_ != nullptr)
		{
			bound_target_->is_processed = false;
			bound_target_ = nullptr;			
		}
		bound_target_ = new empty_object("buildItem", mouseWorldPos);
		bound_target_->tag = entity_tag::buildObject;
		move_system.lax_move_position = mouseWorldPos;
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
			if (bound_target_ != nullptr)
			{
				bound_target_->is_processed = false;
				bound_target_ = nullptr;				
			}
			bound_target_ = new empty_object("droppedItem", mouseWorldPos);
			bound_target_->tag = entity_tag::dropPoint;
			move_system.lax_move_position = mouseWorldPos;
			return;
		}

		for (auto& bag : bags)
		{
			if (bag.current_state == ejected)
			{
				if (bound_target_ != nullptr)
				{
					bound_target_->is_processed = false;
					bound_target_ = nullptr;
				}
				bound_target_ = new empty_object("droppedBag", mouseWorldPos);
				bound_target_->tag = entity_tag::dropPoint;
				move_system.lax_move_position = mouseWorldPos;
			}
		}
		return;
	}	

	if (currentMouseButton == 2)
	{
		bound_target_ = mouseSelectedObject;
		move_system.lax_move_position = mouseSelectedObject->get_position();
	}
}

void deerchant::endingPreviousAction()
{
	if (last_action_ == common_hit && !Mouse::isButtonPressed(Mouse::Left))
	{
		direction_system.last_direction = direction_system::side_to_direction(direction_system.side);
		change_action(relax, true, false);
	}
	if (last_action_ == move_hit && !Mouse::isButtonPressed(Mouse::Left))
	{
		direction_system.last_direction = direction_system::side_to_direction(direction_system.side);
		change_action(move, true, false);
	}
	if (last_action_ == actions::move_end)
	{
		change_action(relax, true, false);
	}
	if (last_action_ == open)
		change_action(relax, true, false);

	if (last_action_ == absorbs)
	{
		bound_target_->is_processed = false;
		bound_target_ = nullptr;
		change_action(relax, true, false);
	}
	if (last_action_ == builds)
	{
		change_action(relax, true, false);
	}
	if (last_action_ == dropping)
	{
		change_action(relax, true, false);
		if (heldItem->content.first != entity_tag::emptyCell)
		{
			birth_static_info dropObject;
			dropObject.position = { bound_target_->get_position().x, bound_target_->get_position().y };
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
	if (current_action_ == throw_noose && current_sprite_[0] == 12 && heldItem->content == std::make_pair(entity_tag::noose, 1))
	{
		heldItem->content = { entity_tag::emptyCell, 0 };
		birth_dynamic_info nooseObject;
		nooseObject.position = position_;
		nooseObject.tag = entity_tag::noose;
		nooseObject.owner = this;
		birth_dynamics_.push(nooseObject);
	}
    if (last_action_ == throw_noose)
    {
		direction_system.last_direction = direction_system::side_to_direction(direction_system.side);
		change_action(relax, true, false);
    }
	if (last_action_ == open)
	{
		if (unsealInventoryOwner)
		{
			unsealInventoryOwner->is_visible_inventory = true;
			change_action(relax, true, false);
		}
	}
	if (last_action_ == grab)
	{
		if (bound_target_)
		{
			auto pickedItem = dynamic_cast<picked_object*>(bound_target_);
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
			
			auto nooseItem = dynamic_cast<noose*>(bound_target_);
			if (nooseItem)
			{
				const auto placedNoose = new std::vector<std::pair<entity_tag, int>>({ (std::make_pair(entity_tag::noose, 1)) });
				if (hero_bag::put_items_in(placedNoose, &bags))
					nooseItem->delete_promise_on();
				delete placedNoose;
			}
			stopping(true, true);
			change_action(relax, true, false);
		}
	}
	last_action_ = relax;
}

void deerchant::animationSmooth()
{
	if (direction_system.direction == direction::UP && direction_system.last_direction == direction::LEFT ||
		direction_system.direction == direction::LEFT && direction_system.last_direction == direction::UP)
	{
		smoothDirections = { direction::UPLEFT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::DOWN && direction_system.last_direction == direction::LEFT ||
		direction_system.direction == direction::LEFT && direction_system.last_direction == direction::DOWN)
	{
		smoothDirections = { direction::DOWNLEFT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UP && direction_system.last_direction == direction::RIGHT ||
		direction_system.direction == direction::RIGHT && direction_system.last_direction == direction::UP)
	{
		smoothDirections = { direction::UPRIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::DOWN && direction_system.last_direction == direction::RIGHT ||
		direction_system.direction == direction::RIGHT && direction_system.last_direction == direction::DOWN)
	{
		smoothDirections = { direction::DOWNRIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UP && direction_system.last_direction == direction::DOWN)
	{
		smoothDirections = { direction::DOWNLEFT, direction::LEFT, direction::UPLEFT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::DOWN && direction_system.last_direction == direction::UP){
		smoothDirections = { direction::UPRIGHT, direction::RIGHT, direction::DOWNRIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::LEFT && direction_system.last_direction == direction::RIGHT)
	{		
		smoothDirections = { direction::DOWNRIGHT, direction::DOWN, direction::DOWNLEFT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::RIGHT && direction_system.last_direction == direction::LEFT)
	{
		smoothDirections = { direction::UPLEFT, direction::UP, direction::UPRIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UPLEFT && direction_system.last_direction == direction::DOWNRIGHT)
	{
		smoothDirections = { direction::RIGHT, direction::UPRIGHT, direction::UPRIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::DOWNRIGHT && direction_system.last_direction == direction::UPLEFT)
	{
		smoothDirections = { direction::UP, direction::UPRIGHT, direction::RIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UPRIGHT && direction_system.last_direction == direction::DOWNLEFT)
	{
		smoothDirections = { direction::LEFT, direction::UPLEFT, direction::UP, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::DOWNLEFT && direction_system.last_direction == direction::UPRIGHT)
	{
		smoothDirections = { direction::UP, direction::UPLEFT, direction::LEFT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UPLEFT && direction_system.last_direction == direction::UPRIGHT ||
		direction_system.direction == direction::UPRIGHT && direction_system.last_direction == direction::UPLEFT)
	{
		smoothDirections = { direction::UP, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UPLEFT && direction_system.last_direction == direction::DOWNLEFT ||
		direction_system.direction == direction::DOWNLEFT && direction_system.last_direction == direction::UPLEFT)
	{
		smoothDirections = { direction::LEFT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UPRIGHT && direction_system.last_direction == direction::DOWNRIGHT ||
		direction_system.direction == direction::DOWNRIGHT && direction_system.last_direction == direction::UPRIGHT)
	{
		smoothDirections = { direction::RIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::DOWNLEFT && direction_system.last_direction == direction::DOWNRIGHT ||
		direction_system.direction == direction::DOWNRIGHT && direction_system.last_direction == direction::DOWNLEFT)
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
	if (!(current_action_ == move || current_action_ == move_hit || current_action_ == actions::move_end || current_action_ == throw_noose))
	{
		moveTime = 0;
		return;
	}

	moveTime += elapsedTime;

	const long long speedIncreaseTime = long(5e5);
	const auto partOfSpeed = 0.3f;
	
	if (moveTime < speedIncreaseTime)
		move_system.speed = (partOfSpeed + float(moveTime) * (1 - partOfSpeed) / speedIncreaseTime) * move_system.default_speed;
	else
		move_system.speed = move_system.default_speed;
}

void deerchant::jerkInteract(const long long elapsedTime)
{
	if (current_action_ == jerking)
	{
		if (jerk_time_ > 0)
		{
			jerk_time_ -= elapsedTime;
			move_system.speed = jerk_distance_ / float(jerk_duration_) * jerk_power_ * pow(float(jerk_time_) / float(jerk_duration_), jerk_deceleration_);
			move_system.speed = std::max(move_system.default_speed / jerk_deceleration_, move_system.speed);
		}
		else
		{
			change_action(relax, true, false);
			move_system.speed = move_system.default_speed;
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
	this->jerk_power_ = power;
	this->jerk_deceleration_ = deceleration;
	this->jerk_duration_ = 700000;
	this->jerk_time_ = this->jerk_duration_;
	change_action(jerking, current_action_ != jerking, false);
	jerk_distance_ = 500;
	current_sprite_[0] = 1;

	const auto angle = double(direction_system.direction) * pi / 180;
	move_system.lax_move_position = Vector2f(position_.x + float(cos(angle)) * jerk_distance_, position_.y - float(sin(angle)) * jerk_distance_);
}

void deerchant::fight_interact(long long elapsedTime, dynamic_object* target)
{
	time_after_hitself_ += elapsedTime;
	move_system.push_away(elapsedTime);
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

    auto spriteSide = direction_system.side; auto spriteDirection = direction_system.last_direction;

	animation_speed_ = 12;

	if (direction_system.direction == direction::RIGHT || direction_system.direction == direction::UPRIGHT || direction_system.direction == direction::DOWNRIGHT)
	{
		spriteDirection = direction_system::cut_rights(spriteDirection);
		legsSprite->mirrored = true;
	}

	if (direction_system.side == right && current_action_ != move && current_action_ != actions::move_end && current_action_ != jerking)
	{
		spriteSide = left;
		bodySprite->mirrored = true;
	}

	switch (current_action_)
	{
	case common_hit:
		bodySprite->animation_length = 8;
		if (direction_system.side == right)
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
		if (direction_system.last_direction == direction::RIGHT || direction_system.last_direction == direction::UPRIGHT || direction_system.last_direction == direction::DOWNRIGHT)
			bodySprite->mirrored = true;
		bodySprite->pack_tag = pack_tag::heroPick; bodySprite->pack_part = pack_part::full; bodySprite->direction = direction_system::cut_diagonals(spriteDirection);
		break;
	case open:
		bodySprite->animation_length = 12;
		bodySprite->pack_tag = pack_tag::heroPick; bodySprite->pack_part = pack_part::full; bodySprite->direction = direction_system::cut_diagonals(spriteDirection);
		break;	
	case jerking:
		bodySprite->animation_length = 8;
		animation_speed_ = 11;
		spriteDirection = direction_system.last_direction;
		if (direction_system.last_direction == direction::RIGHT || direction_system.last_direction == direction::UPRIGHT || direction_system.last_direction == direction::DOWNRIGHT)
			bodySprite->mirrored = true;
		if (direction_system::cut_rights(direction_system.last_direction) == direction::UPLEFT || direction_system::cut_rights(direction_system.last_direction) == direction::DOWNLEFT)
			spriteDirection = direction::LEFT;
		bodySprite->pack_tag = pack_tag::heroRoll; bodySprite->pack_part = pack_part::full; bodySprite->direction = direction_system::cut_rights(spriteDirection);
		break;
	case relax:
		bodySprite->mirrored = false;
		bodySprite->animation_length = 16;
		animation_speed_ = 13;
		spriteDirection = direction_system.last_direction;
		if (direction_system.last_direction == direction::RIGHT || direction_system.last_direction == direction::UPRIGHT || direction_system.last_direction == direction::DOWNRIGHT)
			bodySprite->mirrored = true;
		if (direction_system::cut_rights(direction_system.last_direction) == direction::UPLEFT || direction_system::cut_rights(direction_system.last_direction) == direction::DOWNLEFT)
			spriteDirection = direction::LEFT;
		bodySprite->pack_tag = pack_tag::heroStand; bodySprite->pack_part = pack_part::full; bodySprite->direction = direction_system::cut_rights(spriteDirection);
		break;
	case move:
	{
		animation_speed_ = move_system.speed / move_system.default_speed * 12;
		if (animation_speed_ < 10)
			animation_speed_ = 10;
		bodySprite->animation_length = 8;

		auto finalDirection = direction_system.last_direction;
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
	case actions::move_end:
		bodySprite->animation_length = 8;
		current_sprite_[1] = 1;
		if (direction_system.last_direction == direction::RIGHT || direction_system.last_direction == direction::UPRIGHT || direction_system.last_direction == direction::DOWNRIGHT)
		{
			bodySprite->mirrored = true;
			legsSprite->mirrored = true;
		}
		bodySprite->pack_tag = pack_tag::heroMove; bodySprite->pack_part = pack_part::body; bodySprite->direction = direction_system::cut_rights(direction_system.last_direction);
		direction_system.last_direction = direction_system::cut_diagonals(direction_system.last_direction);
		legsSprite->pack_tag = pack_tag::heroHit; legsSprite->pack_part = pack_part::legs; legsSprite->direction = direction_system::cut_rights(direction_system.last_direction);
		
		break;
	}

	if (current_action_ == move_hit)
	{
		animation_speed_ = move_system.speed / move_system.default_speed * 12;
		if (animation_speed_ < 10)
			animation_speed_ = 10;
		bodySprite->animation_length = 8;

		if (direction_system.direction == direction::UP && direction_system.side == down || direction_system.direction == direction::DOWN && direction_system.side == up)
		{
			legsInverse = true;
			spriteDirection = direction_system::side_to_direction(spriteSide);
		}
		if ((direction_system.direction == direction::LEFT || direction_system.direction == direction::UPLEFT || direction_system.direction == direction::DOWNLEFT) && direction_system.side == right ||
			(direction_system.direction == direction::UPLEFT || direction_system.direction == direction::UPRIGHT) && direction_system.side == down ||
			(direction_system.direction == direction::RIGHT || direction_system.direction == direction::UPRIGHT || direction_system.direction == direction::DOWNRIGHT) && direction_system.side == left ||
			(direction_system.direction == direction::DOWNLEFT || direction_system.direction == direction::DOWNRIGHT) && direction_system.side == up)
		{
			legsInverse = true;
			legsSprite->mirrored = !legsSprite->mirrored;
			spriteDirection = direction_system::invert_direction(spriteDirection);
		}
		if (direction_system.direction == direction::UP && direction_system.side == down || direction_system.direction == direction::DOWN && direction_system.side == up)
			legsInverse = true;

		legsSprite->pack_tag = pack_tag::heroMove; legsSprite->pack_part = pack_part::legs; legsSprite->direction = direction_system::cut_rights(spriteDirection);
		bodySprite->pack_tag = pack_tag::heroHit; bodySprite->pack_part = pack_part::body; bodySprite->direction = direction_system::side_to_direction(spriteSide);

		if (direction_system.direction == direction::STAND)
		{
			if (direction_system.side == right)
				legsSprite->mirrored = true;
			legsSprite->pack_tag = pack_tag::heroMove; legsSprite->pack_part = pack_part::legs; legsSprite->direction = direction_system::side_to_direction(spriteSide);
			legsSprite->animation_length = 14;
		}
	}

	if (current_action_ == throw_noose)
	{
		bodySprite->animation_length = 14;
		if (direction_system.direction == direction::UP && direction_system.side == down || direction_system.direction == direction::DOWN && direction_system.side == up)
		{
			legsInverse = true;
			spriteDirection = direction_system::side_to_direction(spriteSide);
		}
		if ((direction_system.direction == direction::LEFT || direction_system.direction == direction::UPLEFT || direction_system.direction == direction::DOWNLEFT) && direction_system.side == right ||
			(direction_system.direction == direction::RIGHT || direction_system.direction == direction::UPRIGHT || direction_system.direction == direction::DOWNRIGHT) && direction_system.side == left)
		{
			legsInverse = true;
			legsSprite->mirrored = !legsSprite->mirrored;
		}

		legsSprite->pack_tag = pack_tag::heroMove; legsSprite->pack_part = pack_part::legs, legsSprite->direction = spriteDirection;
		bodySprite->pack_tag = pack_tag::heroThrow; bodySprite->pack_part = pack_part::body; bodySprite->direction = direction_system::side_to_direction(spriteSide);

		if (direction_system.direction == direction::STAND)
		{
			if (direction_system.side == right)
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

	if (double(time_for_new_sprite_) >= 1e6 / animation_speed_)
	{
		time_for_new_sprite_ = 0;

		if (current_action_ == actions::move_end)
		{
			current_sprite_[0] = calculateNextMoveEndSprite(current_sprite_[0]);
			if (current_sprite_[0] == -1)
			{
				last_action_ = current_action_;
				current_sprite_[0] = 1;
			}
		}
		else
		{
			if (++current_sprite_[0] > bodySprite->animation_length)
				current_sprite_[0] = 1;
			if (current_sprite_[0] >= bodySprite->animation_length)
				last_action_ = current_action_;
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
