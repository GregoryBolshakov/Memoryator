#include "owl.h"

#include "fern.h"
#include "forest_tree.h"

using namespace sf;

owl::owl(const std::string& objectName, Vector2f centerPosition) : neutral_mob(objectName, centerPosition)
{
	conditional_size_units_ = { 280, 200 };
	current_sprite_[0] = 1;
	timeForNewSprite = 0;
	move_system.default_speed = 0.0006f;
	move_system.speed = 0.0006f;
	animation_speed_ = 0.0008f;
	animationLength = 8;
	radius_ = 70;
	strength_ = 10;
	sight_range = helper::GetScreenSize().y * 1 / 2;
	health_point_ = 50;
	current_action_ = relax;
	time_after_hitself_ = 0;
	time_for_new_hit_self = long(6e5);
	timeForNewHit = 1000000;
	to_save_name_ = "owl";
	tag = entity_tag::owl;
	move_system.can_crash_into_static = false;
	z_coordinate_ = 10;
}

owl::~owl()
= default;

Vector2f owl::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return { texture_box_.width / 2, texture_box_.height * 7 / 8 };
}

void owl::set_target(dynamic_object& object)
{	
	if (object.tag == entity_tag::noose || current_action_ == absorbs)
		return;
	if (helper::getDist(position_, object.get_position()) <= sight_range)
	{
		if (object.tag == entity_tag::hero)
		{
			bound_target_ = &object;
			distanceToNearest = helper::getDist(position_, object.get_position());
		}
	}
}

void owl::behavior_with_static(world_object* target, long long elapsedTime)
{
	if (current_action_ == absorbs)
		return;

	if (target->tag == entity_tag::fern && helper::getDist(position_, target->get_position()) <= sight_range * 2 && timeAfterFear >= fearTime)
	{
		if (!(bound_target_ && bound_target_->tag == entity_tag::hero && (helper::getDist(position_, bound_target_->get_position()) <= sight_range ||
			helper::getDist(target->get_position(), bound_target_->get_position()) <= sight_range)))
		{
			bound_target_ = target;
			distanceToNearest = helper::getDist(position_, target->get_position());
		}
	}
	else
	if (target->tag == entity_tag::tree)
	{
		auto nearestTreeCasted = dynamic_cast<forest_tree*>(target);
		if (nearestTreeCasted && !(bound_target_ && bound_target_->tag == entity_tag::fern))
			if (bound_target_ != nullptr && bound_target_->tag == entity_tag::hero && helper::getDist(nearestTreeCasted->getOwlBase(), position_) < helper::getDist(nearestTreeCasted->getOwlBase(), bound_target_->get_position()) &&
				helper::getDist(nearestTreeCasted->getOwlBase(), bound_target_->get_position()) > helper::getDist(position_, bound_target_->get_position()))
				if ((nearestTree != nullptr && helper::getDist(nearestTreeCasted->getOwlBase(), position_) < helper::getDist(nearestTree->get_position(), position_)) || nearestTree == nullptr)
					if (helper::getDist(nearestTreeCasted->getOwlBase(), bound_target_->get_position()) > sight_range * 1.2)
						nearestTree = target;							
	}
}

void owl::behavior(long long elapsedTime)
{
	endingPreviousAction();
	fight_interact(elapsedTime);
	if (health_point_ <= 0)
	{
		change_action(dead, true);
		direction_system.direction = direction::STAND;
		return;
	}

	// first-priority actions
	if (bound_target_ && bound_target_->tag == entity_tag::hero && helper::getDist(position_, bound_target_->get_position()) <= sight_range)
			timeAfterFear = 0;	
	else
		timeAfterFear += elapsedTime;

	if (current_action_ == absorbs)
	{
		move_system.lax_move_position = position_;
		return;
	}
	//-----------------------	

	if (bound_target_ == nullptr || bound_target_ && bound_target_->tag == entity_tag::hero && helper::getDist(position_, bound_target_->get_position()) > sight_range)
	{
		auto nearestTreeCasted = dynamic_cast<forest_tree*>(nearestTree);
		if (nearestTree && helper::getDist(position_, nearestTreeCasted->getOwlBase()) > radius_)
			{				
				change_action(move, false, true);
				move_system.lax_move_position = nearestTreeCasted->getOwlBase();
				direction_system.side = direction_system.calculate_side(position_, move_system.lax_move_position, elapsedTime);
				return;
			}

		change_action(relax, true, true);
		direction_system.direction = direction::STAND;
		move_system.lax_move_position = position_;
		return;
	}

	// bouncing to a trap
	if (bound_target_ && bound_target_->tag == entity_tag::fern)
	{
		direction_system.side = direction_system.calculate_side(position_, bound_target_->get_position(), elapsedTime);
		if (helper::getDist(position_, bound_target_->get_position()) <= radius_)
		{
			const auto trap = dynamic_cast<fern*>(bound_target_);
			if (direction_system.side == right)
				mirrored_ = true;
			else
				mirrored_ = false;
			position_ = trap->getEnterPosition();
			change_action(absorbs, true, false);
			move_system.lax_move_position = position_;
		}
		else
		{
			change_action(move, false, true);
			move_system.lax_move_position = bound_target_->get_position();
		}
	}
	//-------------------

	// runaway from enemy
	if (bound_target_ && bound_target_->tag == entity_tag::hero)
	{
		const float distanceToTarget = helper::getDist(this->position_, bound_target_->get_position());
		direction_system.side = direction_system.calculate_side(position_, move_system.lax_move_position, elapsedTime);
		move_system.speed = std::max(move_system.default_speed, (move_system.default_speed * 10) * (1 - (helper::getDist(position_, bound_target_->get_position()) / sight_range * 1.5f)));
		animation_speed_ = std::max(0.0008f, 0.0008f * move_system.speed / move_system.default_speed);
		if (distanceToTarget <= sight_range)
		{
			change_action(move, false, true);
			float k = (sight_range * 1.3f / (sqrt(pow(bound_target_->get_position().x - position_.x, 2) + pow(bound_target_->get_position().y - position_.y, 2))));
			move_system.lax_move_position = Vector2f(position_.x + (position_.x - bound_target_->get_position().x) * k, position_.y + (position_.y - bound_target_->get_position().y) * k);
		}
	}	
	//-------------------

	if (current_action_ != absorbs)
	{
		distanceToNearest = 10e6;
		bound_target_ = nullptr;
		nearestTree = nullptr;
	}
}

Vector2f owl::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int owl::get_build_type(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void owl::endingPreviousAction()
{
	if (last_action_ == common_hit)
		current_action_ = relax;
	if (last_action_ == absorbs)
	{
		auto trap = dynamic_cast<fern*>(bound_target_);
		trap->inventory.emplace_back(entity_tag::owl, 1);
		delete_promise_on();
	}
	last_action_ = relax;
}

void owl::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

std::vector<sprite_chain_element*> owl::prepare_sprites(long long elapsedTime)
{
    return {};
	/*spriteChainElement fullSprite;

	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);
	additionalSprites.clear();

	switch (currentAction)
	{
	case relax:
	{
		animationLength = 1;
		fullSprite.path = "Game/worldSprites/owl/stand/" + DynamicObject::sideToString(side) + '/' + std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case absorbs:
	{		
		animationLength = 7;
		fullSprite.path = "Game/worldSprites/owl/fern/" + std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		fullSprite.path = "Game/worldSprites/owl/stand/down/1.png";
		currentSprite[0] = 1;
		break;
	}
	default:;
	}

	if (currentAction == move)
	{
		animationLength = 7;
		fullSprite.path = "Game/worldSprites/owl/move/" + DynamicObject::sideToString(side) + '/' + std::to_string(currentSprite[0]) + ".png";
	}

	additionalSprites.push_back(fullSprite);

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
