#include "nightmare_third.h"

nightmare_third::nightmare_third(std::string objectName, Vector2f centerPosition) : monster(std::move(objectName), centerPosition)
{
	conditional_size_units_ = { 600, 600 };
	move_system_.default_speed = 0.0003f;
	move_system_.speed = 0.0005f;
	radius_ = 120;
	hitDistance = 120;
	strength_ = 25;
	health_point_ = 300;
	current_action_ = relax;
	time_for_new_hit_self = long(6e5);
	time_after_hitself_ = time_for_new_hit_self;
	time_for_new_hit = long(1e6);
	time_after_hit = time_for_new_hit;
	to_save_name_ = "nightmare3_";
	tag = entity_tag::nightmare3;
}


nightmare_third::~nightmare_third()
= default;


Vector2f nightmare_third::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return { texture_box_.width / 2, texture_box_.height * 6 / 8 };
}

void nightmare_third::doAttack(world_object* target)
{
	if (time_after_hit >= time_for_new_hit)
	{
		if (helper::getDist(position_, bound_target_->get_position()) <= (this->radius_ + bound_target_->get_radius() + hitDistance / 3))
		{
			change_action(commonHit, true, false);
			time_after_hit = 0;
		}
		else
			if (helper::getDist(position_, bound_target_->get_position()) > this->radius_ + bound_target_->get_radius() + hitDistance * 2)
			{
				change_action(directHit, true, false);
				time_after_hit = 0;
			}
		
	}	
}

void nightmare_third::endingPreviousAction()
{
	if (last_action_ == combatState)
		change_action(relax, true, false);
	if (last_action_ == commonHit)
		change_action(combatState, true, false);	
	if (last_action_ == directHit)	
		change_action(relax, true, false);	
	last_action_ = relax;
}

void nightmare_third::on_sprite_change()
{
	if (current_action_ == directHit && current_sprite_[0] == 5)
	{
		birth_dynamic_info whirl;
		whirl.position = position_;
		whirl.tag = entity_tag::clapWhirl;
		whirl.owner = this;
		birth_dynamics_.push(whirl);
	}
}

std::vector<sprite_chain_element*> nightmare_third::prepare_sprites(long long elapsedTime)
{
    return {};
	/*spriteChainElement fullSprite;

	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);
	additionalSprites.clear();
	animationSpeed = 10;

	std::string sideStr = DynamicObject::sideToString(side), directionStr = DynamicObject::directionToString(direction);
	if (side == right)
	{
		sideStr = "left";
		fullSprite.mirrored = true;
	}
	if (direction == Direction::RIGHT || direction == Direction::UPRIGHT || direction == Direction::DOWNRIGHT)
	{
		directionStr = "left";
		fullSprite.mirrored = true;
	}

	switch (currentAction)
	{
	case commonHit:
	{
		animationLength = 9;
		fullSprite.path = "Game/worldSprites/nightmare3/hit/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		if (currentSprite[0] == 6)
			doShake = true;
		break;
	}
	case directHit:
	{
		animationLength = 9;		
		fullSprite.path = "Game/worldSprites/nightmare3/clap/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		if (currentSprite[0] == 6)
			doShake = true;		
		break;
	}
	case combatState:
	{
		animationLength = 9;
		fullSprite.path = "Game/worldSprites/nightmare3/stand/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case relax:
	{
		animationLength = 9;
		fullSprite.path = "Game/worldSprites/nightmare3/stand/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		fullSprite.path = "Game/worldSprites/nightmare3/stand/down/1.png";
		currentSprite[0] = 1;
		deletePromiseOn();
		break;
	}
	case move:
	{
		animationLength = 6;
		fullSprite.path = "Game/worldSprites/nightmare3/move/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";		
		break;
	}
	default:;
	}

	additionalSprites.push_back(fullSprite);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 1000000 / animationSpeed)
	{
		currentSprite[0]++;
		onSpriteChange();
		timeForNewSprite = 0;

		if (currentSprite[0] > animationLength)
		{
			lastAction = currentAction;
			if (currentAction == move)
				currentSprite[0] = 4;
			else
				currentSprite[0] = 1;
		}
	}*/
}

