#include "deerchant.h"
#include "sprite_chain_element.h"
#include "empty_object.h"
#include "world_metrics.h"
#include "math_constants.h"
#include "noose.h"
#include "picked_object.h"

deerchant::deerchant(std::string name, const sf::Vector2f position, const int kind) : dynamic_object(std::move(name), position, kind)
{
	tag = entity_tag::hero;
	size_ = { 375, 375 };
	offset_ = { size_.x * 0.5f, size_.y * 0.8f };
	radius_ = creature_radius;
	current_sprite_ = { 1, 1, 1 };
	move_system.default_speed = 0.0006f;
	move_system.speed = move_system.default_speed;
	animation_speed_ = 0.0010f;
	move_end_distance_ = 55;
	hit_distance_ = creature_radius;
	striking_sprite_ = 6;
	strength_ = creature_strength;
	max_health_point_value_ = creature_max_health;
	health_ = max_health_point_value_;
	current_action_ = action::relax;
	
	move_system.can_crash_into_static = true;

	const auto closed_bag_size = sf::Vector2f(world_metrics::window_size.x / 12, world_metrics::window_size.y / 6);
	
	bags.push_back(hero_bag(sf::Vector2f(world_metrics::window_size.x - closed_bag_size.x, closed_bag_size.y)));
	//bags.emplace_back(sf::Vector2f(world_metrics::window_size.x - closedBagSize.x, 2.0f * closedBagSize.y));
	//bags.emplace_back(sf::Vector2f(world_metrics::window_size.x - closedBagSize.x * 1.5f, 1.5f * closedBagSize.y));
}

deerchant::~deerchant()
= default;

int deerchant::calculate_next_move_end_sprite(const int current_sprite) const
{
	const auto endSprite1 = 1, endSprite2 = 6;

	const auto increment = 1;

	if (abs(current_sprite - endSprite1) <= abs(current_sprite - endSprite2))
	{
		if (current_sprite < endSprite1)
			return current_sprite + increment;
		if (current_sprite > endSprite1)
			return current_sprite - increment;
		return -1;
	}
	if (current_sprite < endSprite2)
		return current_sprite + increment;
	if (current_sprite > endSprite2)
		return current_sprite - increment;
	return -1;
}

void deerchant::move_end(const bool animate, const bool invert_direction)
{
	if (was_pushed_after_movement_)
		return;

	auto pushPosition = position_;

	if (direction_system.direction != direction::STAND)
	{
		move_system.set_move_offset(10000);
		pushPosition = position_ - move_system.move_offset;
		if (invert_direction)
			pushPosition = position_ + move_system.move_offset;
	}
	else
	{
		pushPosition = last_position_;
		if (invert_direction)
		{
			pushPosition.x += (position_.x - last_position_.x) * 2;
			pushPosition.y += (position_.y - last_position_.y) * 2;
		}
	}

	if (animate)
		change_action(action::move_end, false, false);
	else
		change_action(action::relax, true, false);

	//bumpedPositions.emplace_back(pushPosition, true);
	//bumpDistance += distance;
}

void deerchant::handle_input(const bool used_mouse, const long long elapsed_time)
{
	//high-priority action
	if (current_action_ == action::absorbs || current_action_ == action::grab || current_action_ == action::dropping || current_action_ == action::builds || current_action_ == action::jerking)
		return;

	if (current_action_ == action::throw_noose && held_item && held_item->content != std::make_pair(entity_tag::noose, 1))
		change_action(action::relax, true, false);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && current_action_ != action::jerking && direction_system.direction != direction::STAND)
	{
		jerk(3, 2);
		return;
	}
	//---------------------

	set_hit_direction();
	move_system.move_offset = sf::Vector2f(-1, -1);
	direction_system.direction = direction::STAND;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		direction_system.direction = direction::LEFT;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		direction_system.direction = direction::RIGHT;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		direction_system.direction = direction::UP;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		direction_system.direction = direction::DOWN;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		direction_system.direction = direction::UPLEFT;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		direction_system.direction = direction::UPRIGHT;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		direction_system.direction = direction::DOWNLEFT;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		direction_system.direction = direction::DOWNRIGHT;
	if (direction_system.direction != direction::STAND && current_action_ != action::move_end)
	{
 		for (auto i = 0u; i < move_system.bumped_positions.size(); i++)
			if (move_system.bumped_positions[i].cancelable)
				move_system.bumped_positions.erase(move_system.bumped_positions.begin() + i);
		move_system.push_away(0);

		if (direction_system.direction != direction_system.last_direction)
		{
			calculate_speed_line_direction(direction_system.last_direction, direction_system.direction);
			current_sprite_[2] = 1;
		}

		animation_smooth();
		direction_system.last_direction = direction_system.direction;
		was_pushed_after_movement_ = false;
	}
	else
		if (!bound_target_ && current_action_ == action::move || current_action_ == action::move_hit)
			move_end(true);		

	if (current_action_ != action::throw_noose) //second priority action, interact while moving
	{
		if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
		{
			auto isIntersect = false;
			if (bound_target_)
				isIntersect = (sqrt(pow(this->position_.x - move_system.lax_move_position.x, 2) + pow(this->position_.y - move_system.lax_move_position.y, 2)) <= (this->radius_ + bound_target_->get_radius()));
			if (isIntersect || !bound_target_ && current_action_ != action::move_end)
			{				
				direction_system.direction = direction::STAND;
				if (current_action_ == action::move || current_action_ == action::move_end)
				{
					direction_system.calculate_direction(elapsed_time);
					change_action(action::relax, true, false);
				}
			}
			else
			{
				if (bound_target_/* && Helper::getDist(position, laxMovePosition) > (this->radius + boundTarget->getRadius())*/)
				{
					direction_system.calculate_direction(elapsed_time);
					move_system.set_move_offset(0);
					change_action(action::move, !(current_action_ == action::move || current_action_ == action::move_end), false);
				}
			}
		}
	}
	else
		return;

	//define action
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && held_item && held_item->content.first == entity_tag::noose)
	{
		change_action(action::throw_noose, true, false);
		return;
	}

	if (direction_system.direction != direction::STAND && current_action_ != action::move_hit && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
		change_action(action::move, current_action_ == action::relax/*, currentAction != move*/, false);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::F) || sf::Keyboard::isKeyPressed(sf::Keyboard::E) || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (bound_target_ != nullptr)
			stopping(true, true);
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			stopping(false, false, true);
	}
	//--------------

	if (is_build_system)
		return;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !used_mouse)
	{
		if (direction_system.direction != direction::STAND)
		{			
			if (current_action_ != action::move_hit && current_action_ != action::common_hit)
				current_sprite_[0] = 1;
			if (current_action_ != action::move && current_action_ != action::move_hit)
				current_sprite_[1] = 1;
			change_action(action::move_hit, false, false);
		}
		else
			change_action(action::common_hit, !(current_action_ == action::move_hit || current_action_ == action::common_hit || current_action_ == action::move_end), false);
	}
}

void deerchant::calculate_speed_line_direction(const direction last_direction, const direction direction)
{
	mirrored_speed_line_ = false;
	reverse_speed_line_ = false;

	speed_line_direction_ = direction::STAND;

	if (direction == direction::LEFT && last_direction == direction::RIGHT || direction == direction::RIGHT && last_direction == direction::LEFT)
	{
		speed_line_direction_ = direction::UPLEFT;
		return;
	}

	if (direction == direction::DOWNLEFT)
	{
		if (last_direction == direction::UPRIGHT || last_direction == direction::RIGHT || last_direction == direction::DOWNRIGHT || last_direction == direction::DOWN)
			speed_line_direction_ = direction::DOWN;
		else
		{
			speed_line_direction_ = direction::RIGHT;
			mirrored_speed_line_ = true;
		}
	}

	if (direction == direction::DOWNRIGHT)
	{
		if (last_direction == direction::UPLEFT || last_direction == direction::LEFT || last_direction == direction::DOWNLEFT || last_direction == direction::DOWN)
			speed_line_direction_ = direction::DOWN;
		else
		{
			speed_line_direction_ = direction::RIGHT;
			mirrored_speed_line_ = true;
		}
	}

	if (direction == direction::LEFT)
	{
		if (last_direction == direction::RIGHT || last_direction == direction::DOWNRIGHT || last_direction == direction::DOWN || last_direction == direction::DOWNLEFT)
			speed_line_direction_ = direction::LEFT;
		else
		{
			speed_line_direction_ = direction::LEFT;
			mirrored_speed_line_ = true;
		}
	}


	if (direction == direction::UPLEFT)
	{
		if (last_direction == direction::DOWNRIGHT || last_direction == direction::DOWN || last_direction == direction::DOWNLEFT || last_direction == direction::LEFT)
			speed_line_direction_ = direction::RIGHT;
		else
		{
			speed_line_direction_ = direction::UPLEFT;
			mirrored_speed_line_ = true;
		}
	}

	if (direction == direction::UP)
	{
		if (last_direction == direction::DOWN || last_direction == direction::DOWNLEFT || last_direction == direction::LEFT || last_direction == direction::UPLEFT)
			speed_line_direction_ = direction::UP;
		else
		{
			speed_line_direction_ = direction::UP;
			mirrored_speed_line_ = true;
		}
	}

	if (direction == direction::UPRIGHT)
	{
		if (last_direction == direction::DOWNLEFT || last_direction == direction::LEFT || last_direction == direction::UPLEFT || last_direction == direction::UP)
			speed_line_direction_ = direction::UPLEFT;
		else
		{
			speed_line_direction_ = direction::RIGHT;
			mirrored_speed_line_ = true;
		}
	}

	if (direction == direction::RIGHT)
	{
		if (last_direction == direction::LEFT || last_direction == direction::UPLEFT || last_direction == direction::UP || last_direction == direction::UPRIGHT)
			speed_line_direction_ = direction::LEFT;
		else
		{
			speed_line_direction_ = direction::LEFT;
			mirrored_speed_line_ = true;
		}
	}

	if (direction == direction::DOWNRIGHT)
	{
		if (last_direction == direction::UPLEFT || last_direction == direction::UP || last_direction == direction::UPRIGHT || last_direction == direction::RIGHT)
			speed_line_direction_ = direction::RIGHT;
		else
		{
			speed_line_direction_ = direction::DOWN;
			mirrored_speed_line_ = true;
		}
	}
		
	if (direction == direction::DOWN)
	{
		if (last_direction == direction::UP || last_direction == direction::UPRIGHT || last_direction == direction::RIGHT || last_direction == direction::DOWNRIGHT)
		{
			speed_line_direction_ = direction::UP;
			mirrored_speed_line_ = true;
		}
		else
			speed_line_direction_ = direction::UP;
	}
}

void deerchant::change_action(const action new_action, const bool reset_sprite_number, const bool remember_last_action)
{
	if (remember_last_action)
		last_action_ = current_action_;

	current_action_ = new_action;

	if (reset_sprite_number)
		for (auto& number : current_sprite_)
			number = 1;
}

void deerchant::stopping(const bool do_stand, const bool forget_bound_target, const bool off_unseal_inventory)
{
	if (bound_target_ != nullptr && current_action_ != action::dropping)
		if (bound_target_->get_name() == "droppedBag")
			//for (auto& bag : bags)
				//if (bag.current_state == ejected)
					//bag.current_state = bag_closed;

	if (do_stand)
	{
		move_system.lax_move_position = { -1, -1 };
		move_system.move_offset = { -1, -1 };
		direction_system.direction = direction_system.last_direction;
	}

	if (forget_bound_target && bound_target_ != nullptr)
	{
		bound_target_->is_processed = false;
		bound_target_ = nullptr;
	}

	if (off_unseal_inventory)
	{
		if (unseal_inventory_owner_ != nullptr)
			unseal_inventory_owner_->is_visible_inventory = false;
		unseal_inventory_owner_ = nullptr;
	}
}

void deerchant::set_hit_direction()
{
	const auto xPos = world_metrics::window_size.x / 2.0f, yPos = world_metrics::window_size.y / 2.0f;
	
	const auto mouseX = float(sf::Mouse::getPosition().x);
	const auto mouseY = float(sf::Mouse::getPosition().y);
	
	const auto alpha = atan((yPos - mouseY) / (xPos - mouseX)) * 180 / pi;
	
	if (mouseY <= yPos && abs(alpha) >= 45 && abs(alpha) <= 90)
		direction_system.side = side::up;
	else
		if (mouseX >= xPos && abs(alpha) >= 0 && abs(alpha) <= 45)
			direction_system.side = side::right;
		else
			if (mouseY >= yPos && abs(alpha) >= 45 && abs(alpha) <= 90)
				direction_system.side = side::down;
			else
				if (mouseX <= xPos && abs(alpha) >= 0 && abs(alpha) <= 45)
					direction_system.side = side::left;
}

void deerchant::set_target(dynamic_object& object)
{
	near_the_table = false;
}

void deerchant::behavior_with_dynamic(shared_ptr<dynamic_object> target, const long long elapsed_time)
{
	if (world_metrics::get_dist(position_, target->get_position()) <= radius_ + target->get_radius())
		move_system.push_by_bumping(target->get_position(), target->get_radius(), target->move_system.can_crash_into_dynamic);

	const auto isIntersect = world_metrics::get_dist(position_, target->get_position()) <= this->radius_ + target->get_radius() + hit_distance_;

	if (isIntersect && direction_system.calculate_side(position_, target->get_position(), elapsed_time) != direction_system::invert_side(direction_system.side))
	{
		if ((this->current_action_ == action::common_hit || this->current_action_ == action::move_hit) && (current_sprite_[0] == 4 || current_sprite_[0] == 5 || current_sprite_[0] == 8))
			target->take_damage(this->get_strength(), position_);
	}
}

void deerchant::behavior_with_static(shared_ptr<world_object> target, long long elapsed_time)
{
	if (!target)
		return;

	if (target->tag == entity_tag::wreath_table && world_metrics::get_dist(position_, target->get_position()) <= radius_ + target->get_radius())
		near_the_table = true;	
}

void deerchant::behavior(const long long elapsed_time)
{
	jerk_interact(elapsed_time);
	ending_previous_action();
	fight_interact(elapsed_time);
	speed_interact(elapsed_time);
	animation_smooth_interact(elapsed_time);
	last_position_ = position_;	
	
	// interactions with target
	if (!bound_target_ || bound_target_->is_processed)
	{
		move_system.lax_move_position = sf::Vector2f(-1, -1);
		return;
	}	

	if (current_action_ != action::jerking && bound_target_->is_processed)
		move_system.lax_move_position = bound_target_->get_position();

	const auto isIntersect = (world_metrics::get_dist(position_, move_system.lax_move_position)) <= (this->radius_ + bound_target_->get_radius());

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
			change_action(action::absorbs, true, false);
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
				change_action(action::open, true, false);
				unseal_inventory_owner_ = bound_target_;
				bound_target_->is_processed = false;
				stopping(true, true);
			}
			else
			{
				change_action(action::grab, true, false);
				stopping(true);
			}
			
			break;
		}
		case entity_tag::chamomile:
		case entity_tag::yarrow:		
		case entity_tag::mugwort:
		case entity_tag::noose:
		case entity_tag::dropped_loot:
		case entity_tag::hare_trap:
		case entity_tag::hero_bag:
		{
			change_action(action::grab, true, false);
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			stopping(true);
			break;
		}
		case entity_tag::drop_point:
		{		
			change_action(action::dropping, true, false);
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			bound_target_->is_processed = false;
			stopping(true);
			break;
		}
		case entity_tag::build_object:
		{
			change_action(action::builds, true, false);
			//side = calculateSide(boundTarget->getPosition(), elapsedTime);
			bound_target_->is_processed = false;
			stopping(true, true);
			break;
		}
		default:
		{
			change_action(action::relax, true, false);
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

void deerchant::on_mouse_up(const int current_mouse_button, world_object *mouse_selected_object, const sf::Vector2f mouse_world_pos, const bool is_building)
{
	/*if (is_building && current_mouse_button == 2)
	{
		if (bound_target_ != nullptr)
		{
			bound_target_->is_processed = false;
			bound_target_ = nullptr;			
		}
		bound_target_ = new empty_object("buildItem", mouse_world_pos);
		bound_target_->tag = entity_tag::buildObject;
		move_system.lax_move_position = mouse_world_pos;
		return;
	}

	if (mouse_selected_object && mouse_selected_object->tag == entity_tag::main_object)
	{
		auto main_object = dynamic_cast<::main_object*>(mouse_selected_object);
		if (held_item->content.first != entity_tag::emptyCell &&
			helper::getDist(main_object->getPlatePosition(), position_) <= main_object->getPlateRadius() + radius_)
		{
			main_object->putItemToCraft(held_item->content.first);
			held_item->content = held_item->content.second > 1
				? std::make_pair(held_item->content.first, held_item->content.second - 1)
				: std::make_pair(entity_tag::emptyCell, 0);
		}
		return;
	}

	if (held_item != nullptr)
	{		
		if (held_item->content.first != entity_tag::emptyCell && current_mouse_button == 2)
		{
			mouse_selected_object = nullptr;
			if (bound_target_ != nullptr)
			{
				bound_target_->is_processed = false;
				bound_target_ = nullptr;				
			}
			bound_target_ = new empty_object("droppedItem", mouse_world_pos);
			bound_target_->tag = entity_tag::dropPoint;
			move_system.lax_move_position = mouse_world_pos;
			return;
		}

		for (auto& bag : bags)
		{
			if (bag.current_state == ejected)
			{
				mouse_selected_object = nullptr;
				if (bound_target_ != nullptr)
				{
					bound_target_->is_processed = false;
					bound_target_ = nullptr;
				}
				bound_target_ = new empty_object("droppedBag", mouse_world_pos);
				bound_target_->tag = entity_tag::dropPoint;
				move_system.lax_move_position = mouse_world_pos;
				return;
			}
		}
	}	

	if (current_mouse_button == 2)
	{
		bound_target_ = mouse_selected_object;
		move_system.lax_move_position = mouse_selected_object->get_position();
	}*/
}

void deerchant::ending_previous_action()
{
	if (last_action_ == action::common_hit && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		direction_system.last_direction = direction_system::side_to_direction(direction_system.side);
		change_action(action::relax, true, false);
	}
	if (last_action_ == action::move_hit && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		direction_system.last_direction = direction_system::side_to_direction(direction_system.side);
		change_action(action::move, true, false);
	}
	if (last_action_ == action::move_end)
	{
		change_action(action::relax, true, false);
	}
	if (last_action_ == action::open)
		change_action(action::relax, true, false);

	if (last_action_ == action::absorbs)
	{
		bound_target_->is_processed = false;
		bound_target_ = nullptr;
		change_action(action::relax, true, false);
	}
	if (last_action_ == action::builds)
	{
		change_action(action::relax, true, false);
	}
	if (last_action_ == action::dropping)
	{
		change_action(action::relax, true, false);
		if (held_item && held_item->content.first != entity_tag::empty_cell)
		{
			birth_static_info dropObject;
			dropObject.position = { bound_target_->get_position().x, bound_target_->get_position().y };
			dropObject.tag = entity_tag::dropped_loot;
			dropObject.type_of_object = int(held_item->content.first);
			dropObject.count = held_item->content.second;
			birth_statics_.push(dropObject);
			held_item->content = std::make_pair(entity_tag::empty_cell, 0);
		}
		else
		{
			/*for (auto cnt = 0u; cnt != bags.size(); cnt++)
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
					bags.erase(bags.begin() + cnt);
					birth_statics_.push(dropObject);
					break;
				}*/	
		}
		stopping(true, true);
	}
	if (current_action_ == action::throw_noose && current_sprite_[0] == 12 && held_item && held_item->content == std::make_pair(entity_tag::noose, 1))
	{
		held_item->content = { entity_tag::empty_cell, 0 };
		birth_dynamic_info nooseObject;
		nooseObject.position = position_;
		nooseObject.tag = entity_tag::noose;
		*nooseObject.owner = *this;
		birth_dynamics_.push(nooseObject);
	}
    if (last_action_ == action::throw_noose)
    {
		direction_system.last_direction = direction_system::side_to_direction(direction_system.side);
		change_action(action::relax, true, false);
    }
	if (last_action_ == action::open)
	{
		if (unseal_inventory_owner_)
		{
			unseal_inventory_owner_->is_visible_inventory = true;
			change_action(action::relax, true, false);
		}
	}
	if (last_action_ == action::grab)
	{
		if (bound_target_)
		{
			auto pickedItem = dynamic_cast<picked_object*>(bound_target_);
			if (pickedItem)
			{
				if (pickedItem->get_kind() == int(entity_tag::hero_bag) || pickedItem->getId() == entity_tag::hare_trap)
				{
					//bags.emplace_back(sf::Vector2f(world_metrics::window_size.x / 2, world_metrics::window_size.y / 2), true, pickedItem->inventory);
					pickedItem->delete_promise_on();
				}
				//else
					//pickedItem->pickUp(this->bags);
			}
			
			/*auto nooseItem = dynamic_cast<noose*>(bound_target_);
			if (nooseItem)
			{
				const auto placedNoose = new std::vector<std::pair<entity_tag, int>>({ (std::make_pair(entity_tag::noose, 1)) });
				if (hero_bag::put_items_in(placedNoose, &bags))
					nooseItem->delete_promise_on();
				delete placedNoose;
			}*/
			stopping(true, true);
			change_action(action::relax, true, false);
		}
	}
	last_action_ = action::relax;
}

void deerchant::animation_smooth()
{
	if (direction_system.direction == direction::UP && direction_system.last_direction == direction::LEFT ||
		direction_system.direction == direction::LEFT && direction_system.last_direction == direction::UP)
	{
		smooth_directions_ = { direction::UPLEFT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::DOWN && direction_system.last_direction == direction::LEFT ||
		direction_system.direction == direction::LEFT && direction_system.last_direction == direction::DOWN)
	{
		smooth_directions_ = { direction::DOWNLEFT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UP && direction_system.last_direction == direction::RIGHT ||
		direction_system.direction == direction::RIGHT && direction_system.last_direction == direction::UP)
	{
		smooth_directions_ = { direction::UPRIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::DOWN && direction_system.last_direction == direction::RIGHT ||
		direction_system.direction == direction::RIGHT && direction_system.last_direction == direction::DOWN)
	{
		smooth_directions_ = { direction::DOWNRIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UP && direction_system.last_direction == direction::DOWN)
	{
		smooth_directions_ = { direction::DOWNLEFT, direction::LEFT, direction::UPLEFT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::DOWN && direction_system.last_direction == direction::UP){
		smooth_directions_ = { direction::UPRIGHT, direction::RIGHT, direction::DOWNRIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::LEFT && direction_system.last_direction == direction::RIGHT)
	{		
		smooth_directions_ = { direction::DOWNRIGHT, direction::DOWN, direction::DOWNLEFT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::RIGHT && direction_system.last_direction == direction::LEFT)
	{
		smooth_directions_ = { direction::UPLEFT, direction::UP, direction::UPRIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UPLEFT && direction_system.last_direction == direction::DOWNRIGHT)
	{
		smooth_directions_ = { direction::RIGHT, direction::UPRIGHT, direction::UPRIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::DOWNRIGHT && direction_system.last_direction == direction::UPLEFT)
	{
		smooth_directions_ = { direction::UP, direction::UPRIGHT, direction::RIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UPRIGHT && direction_system.last_direction == direction::DOWNLEFT)
	{
		smooth_directions_ = { direction::LEFT, direction::UPLEFT, direction::UP, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::DOWNLEFT && direction_system.last_direction == direction::UPRIGHT)
	{
		smooth_directions_ = { direction::UP, direction::UPLEFT, direction::LEFT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UPLEFT && direction_system.last_direction == direction::UPRIGHT ||
		direction_system.direction == direction::UPRIGHT && direction_system.last_direction == direction::UPLEFT)
	{
		smooth_directions_ = { direction::UP, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UPLEFT && direction_system.last_direction == direction::DOWNLEFT ||
		direction_system.direction == direction::DOWNLEFT && direction_system.last_direction == direction::UPLEFT)
	{
		smooth_directions_ = { direction::LEFT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::UPRIGHT && direction_system.last_direction == direction::DOWNRIGHT ||
		direction_system.direction == direction::DOWNRIGHT && direction_system.last_direction == direction::UPRIGHT)
	{
		smooth_directions_ = { direction::RIGHT, direction::STAND };
		return;
	}
	if (direction_system.direction == direction::DOWNLEFT && direction_system.last_direction == direction::DOWNRIGHT ||
		direction_system.direction == direction::DOWNRIGHT && direction_system.last_direction == direction::DOWNLEFT)
	{
		smooth_directions_ = { direction::DOWN, direction::STAND };
	}

}

void deerchant::animation_smooth_interact(long long elapsed_time)
{
	smooth_move_time_ += elapsed_time;

	if (smooth_directions_.empty())
	{
		smooth_direction_ = direction::STAND;
		return;
	}

	const long long time_for_new_smooth_direction = 70000;

	if (smooth_move_time_ >= time_for_new_smooth_direction)
	{
		smooth_direction_ = smooth_directions_[0];
		smooth_directions_.erase(smooth_directions_.begin() + 0);
		smooth_move_time_ = 0;
	}
}

void deerchant::speed_interact(const long long elapsed_time)
{
	if (!(current_action_ == action::move || current_action_ == action::move_hit || current_action_ == action::move_end || current_action_ == action::throw_noose))
	{
		move_time_ = 0;
		return;
	}

	move_time_ += elapsed_time;

	const long long speedIncreaseTime = long(5e5);
	const auto partOfSpeed = 0.3f;
	
	if (move_time_ < speedIncreaseTime)
		move_system.speed = (partOfSpeed + float(move_time_) * (1 - partOfSpeed) / speedIncreaseTime) * move_system.default_speed;
	else
		move_system.speed = move_system.default_speed;
}

void deerchant::jerk_interact(const long long elapsed_time)
{
	if (current_action_ == action::jerking)
	{
		if (jerk_time_ > 0)
		{
			jerk_time_ -= elapsed_time;
			move_system.speed = jerk_distance_ / float(jerk_duration_) * jerk_power_ * pow(float(jerk_time_) / float(jerk_duration_), jerk_deceleration_);
			move_system.speed = std::max(move_system.default_speed / jerk_deceleration_, move_system.speed);
		}
		else
		{
			change_action(action::relax, true, false);
			move_system.speed = move_system.default_speed;
		}
	}
}

sf::Vector2f deerchant::get_belt_position() const
{
	/*if (additionalSprites.size() >= 2) return
		sf::Vector2f((4 * additionalSprites[0].position.x + additionalSprites[1].position.x) / 5.0f + conditionalSizeUnits.x / 3.0f,
		(4 * additionalSprites[0].position.y + additionalSprites[1].position.y) / 5.0f);
	return additionalSprites[0].position;*/
    return position_;
}

void deerchant::jerk(const float power, const float deceleration, sf::Vector2f)
{
	stopping(false, true);
	this->jerk_power_ = power;
	this->jerk_deceleration_ = deceleration;
	this->jerk_duration_ = 700000;
	this->jerk_time_ = this->jerk_duration_;
	change_action(action::jerking, current_action_ != action::jerking, false);
	jerk_distance_ = 500;
	current_sprite_[0] = 1;

	const auto angle = double(direction_system.direction) * pi / 180;
	move_system.lax_move_position = sf::Vector2f(position_.x + float(cos(angle)) * jerk_distance_, position_.y - float(sin(angle)) * jerk_distance_);
}

void deerchant::fight_interact(const long long elapsed_time, dynamic_object* target)
{
	time_after_hitself_ += elapsed_time;
	move_system.push_away(elapsed_time);
}

unique_ptr<sprite_chain_element> deerchant::prepare_speed_line()
{
	auto speed_line = make_unique<sprite_chain_element>(pack_tag::heroMove, pack_part::lines, direction::STAND, 1, position_, size_, sf::Vector2f(offset_));
	speed_line->animation_length = 3;
	if (speed_line_direction_ == direction::STAND || current_sprite_[2] > speed_line->animation_length)
		return std::make_unique<sprite_chain_element>();

	speed_line->direction = speed_line_direction_;
	speed_line->mirrored = mirrored_speed_line_;
	speed_line->offset.y += size_.y / 9.0f;
	speed_line->position = sf::Vector2f(position_.x, position_.y + size_.y / 9.0f);

	if (reverse_speed_line_)
		speed_line->number = speed_line->animation_length + 1 - current_sprite_[2];
	else
		speed_line->number = current_sprite_[2];

	return speed_line;
}

std::vector<unique_ptr<sprite_chain_element>> deerchant::prepare_sprites(const long long elapsed_time)
{
	auto legs_sprite = make_unique<sprite_chain_element>(sf::Vector2f(position_.x, position_.y - 1), size_, sf::Vector2f(offset_.x, offset_.y + 1), color);
	auto body_sprite = make_unique<sprite_chain_element>(position_, size_, offset_, color);

	const auto speed_line = prepare_speed_line();
	std::vector<unique_ptr<sprite_chain_element>> result;
	auto legs_inverse = false, body_inverse = false;
	legs_sprite->animation_length = 8;

	auto sprite_side = direction_system.side; auto spriteDirection = direction_system.last_direction;

	animation_speed_ = 12;

	if (direction_system.direction == direction::RIGHT || direction_system.direction == direction::UPRIGHT || direction_system.direction == direction::DOWNRIGHT)
	{
		spriteDirection = direction_system::cut_rights(spriteDirection);
		legs_sprite->mirrored = true;
	}

	if (direction_system.side == side::right && current_action_ != action::move && current_action_ != action::move_end && current_action_ != action::jerking)
	{
		sprite_side = side::left;
		body_sprite->mirrored = true;
	}

	switch (current_action_)
	{
	case action::common_hit:
		body_sprite->animation_length = 8;
		if (direction_system.side == side::right)
			legs_sprite->mirrored = true;
		body_sprite->pack_tag = pack_tag::heroHit; body_sprite->pack_part = pack_part::body; body_sprite->direction = direction_system::side_to_direction(sprite_side);
		legs_sprite->pack_tag = pack_tag::heroHit; legs_sprite->pack_part = pack_part::legs; legs_sprite->direction = direction_system::side_to_direction(sprite_side);
		break;
	case action::absorbs:
		body_sprite->animation_length = 10;
		body_sprite->pack_tag = pack_tag::heroAbsorb; body_sprite->pack_part = pack_part::full; body_sprite->direction = direction_system::cut_diagonals(spriteDirection);	
		break;
	case action::builds:
		body_sprite->animation_length = 10;
		body_sprite->pack_tag = pack_tag::heroAbsorb; body_sprite->pack_part = pack_part::full; body_sprite->direction = direction_system::cut_diagonals(spriteDirection);
		break;
	case action::grab:
		body_sprite->animation_length = 11;
		animation_speed_ = 15;
		body_sprite->pack_tag = pack_tag::heroPick; body_sprite->pack_part = pack_part::full; body_sprite->direction = direction_system::cut_diagonals(spriteDirection);
		break;
	case action::dropping:
		body_inverse = true;
		body_sprite->animation_length = 5;
		if (direction_system.last_direction == direction::RIGHT || direction_system.last_direction == direction::UPRIGHT || direction_system.last_direction == direction::DOWNRIGHT)
			body_sprite->mirrored = true;
		body_sprite->pack_tag = pack_tag::heroPick; body_sprite->pack_part = pack_part::full; body_sprite->direction = direction_system::cut_diagonals(spriteDirection);
		break;
	case action::open:
		body_sprite->animation_length = 12;
		body_sprite->pack_tag = pack_tag::heroPick; body_sprite->pack_part = pack_part::full; body_sprite->direction = direction_system::cut_diagonals(spriteDirection);
		break;	
	case action::jerking:
		body_sprite->animation_length = 8;
		animation_speed_ = 11;
		spriteDirection = direction_system.last_direction;
		if (direction_system.last_direction == direction::RIGHT || direction_system.last_direction == direction::UPRIGHT || direction_system.last_direction == direction::DOWNRIGHT)
			body_sprite->mirrored = true;
		if (direction_system::cut_rights(direction_system.last_direction) == direction::UPLEFT || direction_system::cut_rights(direction_system.last_direction) == direction::DOWNLEFT)
			spriteDirection = direction::LEFT;
		body_sprite->pack_tag = pack_tag::heroRoll; body_sprite->pack_part = pack_part::full; body_sprite->direction = direction_system::cut_rights(spriteDirection);
		break;
	case action::relax:
		body_sprite->mirrored = false;
		body_sprite->animation_length = 16;
		animation_speed_ = 13;
		spriteDirection = direction_system.last_direction;
		if (direction_system.last_direction == direction::RIGHT || direction_system.last_direction == direction::UPRIGHT || direction_system.last_direction == direction::DOWNRIGHT)
			body_sprite->mirrored = true;
		if (direction_system::cut_rights(direction_system.last_direction) == direction::UPLEFT || direction_system::cut_rights(direction_system.last_direction) == direction::DOWNLEFT)
			spriteDirection = direction::LEFT;
		body_sprite->pack_tag = pack_tag::heroStand; body_sprite->pack_part = pack_part::full; body_sprite->direction = direction_system::cut_rights(spriteDirection);
		break;
	case action::move:
	{
		animation_speed_ = move_system.speed / move_system.default_speed * 12;
		if (animation_speed_ < 10)
			animation_speed_ = 10;
		body_sprite->animation_length = 8;

		auto finalDirection = direction_system.last_direction;
		if (smooth_direction_ != direction::STAND)
			finalDirection = smooth_direction_;

		if (finalDirection == direction::RIGHT || finalDirection == direction::UPRIGHT || finalDirection == direction::DOWNRIGHT)
		{
			body_sprite->mirrored = true;
			legs_sprite->mirrored = true;
		}

		body_sprite->pack_tag = pack_tag::heroMove; body_sprite->pack_part = pack_part::body; body_sprite->direction = direction_system::cut_rights(finalDirection);
		legs_sprite->pack_tag = pack_tag::heroMove; legs_sprite->pack_part = pack_part::legs; legs_sprite->direction = direction_system::cut_rights(finalDirection);
		break;
	}
	case action::move_end:
		body_sprite->animation_length = 8;
		current_sprite_[1] = 1;
		if (direction_system.last_direction == direction::RIGHT || direction_system.last_direction == direction::UPRIGHT || direction_system.last_direction == direction::DOWNRIGHT)
		{
			body_sprite->mirrored = true;
			legs_sprite->mirrored = true;
		}
		body_sprite->pack_tag = pack_tag::heroMove; body_sprite->pack_part = pack_part::body; body_sprite->direction = direction_system::cut_rights(direction_system.last_direction);
		direction_system.last_direction = direction_system::cut_diagonals(direction_system.last_direction);
		legs_sprite->pack_tag = pack_tag::heroHit; legs_sprite->pack_part = pack_part::legs; legs_sprite->direction = direction_system::cut_rights(direction_system.last_direction);
		
		break;
	}

	if (current_action_ == action::move_hit)
	{
		animation_speed_ = move_system.speed / move_system.default_speed * 12;
		if (animation_speed_ < 10)
			animation_speed_ = 10;
		body_sprite->animation_length = 8;

		if (direction_system.direction == direction::UP && direction_system.side == side::down || direction_system.direction == direction::DOWN && direction_system.side == side::up)
		{
			legs_inverse = true;
			spriteDirection = direction_system::side_to_direction(sprite_side);
		}
		if ((direction_system.direction == direction::LEFT || direction_system.direction == direction::UPLEFT || direction_system.direction == direction::DOWNLEFT) && direction_system.side == side::right ||
			(direction_system.direction == direction::UPLEFT || direction_system.direction == direction::UPRIGHT) && direction_system.side == side::down ||
			(direction_system.direction == direction::RIGHT || direction_system.direction == direction::UPRIGHT || direction_system.direction == direction::DOWNRIGHT) && direction_system.side == side::left ||
			(direction_system.direction == direction::DOWNLEFT || direction_system.direction == direction::DOWNRIGHT) && direction_system.side == side::up)
		{
			legs_inverse = true;
			legs_sprite->mirrored = !legs_sprite->mirrored;
			spriteDirection = direction_system::invert_direction(spriteDirection);
		}
		if (direction_system.direction == direction::UP && direction_system.side == side::down || direction_system.direction == direction::DOWN && direction_system.side == side::up)
			legs_inverse = true;

		legs_sprite->pack_tag = pack_tag::heroMove; legs_sprite->pack_part = pack_part::legs; legs_sprite->direction = direction_system::cut_rights(spriteDirection);
		body_sprite->pack_tag = pack_tag::heroHit; body_sprite->pack_part = pack_part::body; body_sprite->direction = direction_system::side_to_direction(sprite_side);

		if (direction_system.direction == direction::STAND)
		{
			if (direction_system.side == side::right)
				legs_sprite->mirrored = true;
			legs_sprite->pack_tag = pack_tag::heroMove; legs_sprite->pack_part = pack_part::legs; legs_sprite->direction = direction_system::side_to_direction(sprite_side);
			legs_sprite->animation_length = 14;
		}
	}

	if (current_action_ == action::throw_noose)
	{
		body_sprite->animation_length = 14;
		if (direction_system.direction == direction::UP && direction_system.side == side::down || direction_system.direction == direction::DOWN && direction_system.side == side::up)
		{
			legs_inverse = true;
			spriteDirection = direction_system::side_to_direction(sprite_side);
		}
		if ((direction_system.direction == direction::LEFT || direction_system.direction == direction::UPLEFT || direction_system.direction == direction::DOWNLEFT) && direction_system.side == side::right ||
			(direction_system.direction == direction::RIGHT || direction_system.direction == direction::UPRIGHT || direction_system.direction == direction::DOWNRIGHT) && direction_system.side == side::left)
		{
			legs_inverse = true;
			legs_sprite->mirrored = !legs_sprite->mirrored;
		}

		legs_sprite->pack_tag = pack_tag::heroMove; legs_sprite->pack_part = pack_part::legs, legs_sprite->direction = spriteDirection;
		body_sprite->pack_tag = pack_tag::heroThrow; body_sprite->pack_part = pack_part::body; body_sprite->direction = direction_system::side_to_direction(sprite_side);

		if (direction_system.direction == direction::STAND)
		{
			if (direction_system.side == side::right)
				legs_sprite->mirrored = true;
			legs_sprite->pack_tag = pack_tag::heroThrow; legs_sprite->pack_part = pack_part::legs, legs_sprite->direction = direction_system::side_to_direction(sprite_side);
			legs_sprite->animation_length = 14;
		}
	}

	//if (speedLineDirection != Direction::STAND)
		//result.push_back(speedLine);

	time_for_new_sprite_ += elapsed_time;

	if (static_cast<double>(time_for_new_sprite_) >= 1e6 / animation_speed_)
	{
		time_for_new_sprite_ = 0;

		if (current_action_ == action::move_end)
		{
			current_sprite_[0] = 1;
			last_action_ = current_action_;
		}
		else
		{
			++current_sprite_[0];
			++current_sprite_[1];
			++current_sprite_[2];
			if (current_sprite_[0] > body_sprite->animation_length)
				current_sprite_[0] = 1;
			if (current_sprite_[0] >= body_sprite->animation_length)
				last_action_ = current_action_;
			if (legs_sprite && current_sprite_[1] > legs_sprite->animation_length)
				current_sprite_[1] = 1;
			if (speed_line && current_sprite_[2] > speed_line->animation_length)
				speed_line_direction_ = direction::STAND;
		}
	}

	if (body_sprite->pack_tag != pack_tag::empty)
	{
		if (body_inverse)
			body_sprite->number = body_sprite->animation_length + 1 - current_sprite_[0];
		else
			body_sprite->number = current_sprite_[0];
		result.push_back(std::move(body_sprite));
	}

	if (legs_sprite->pack_tag != pack_tag::empty)
	{
		if (legs_inverse)
			legs_sprite->number = legs_sprite->animation_length + 1 - current_sprite_[1];
		else
			legs_sprite->number = current_sprite_[1];
		result.push_back(std::move(legs_sprite));
	}

	return result;
}
