#include "nightmare_third.h"

nightmare_third::nightmare_third(std::string objectName, Vector2f centerPosition) : monster(std::move(objectName), centerPosition)
{
	conditional_size_units_ = { 600, 600 };
	moveSystem.defaultSpeed = 0.0003f;
	moveSystem.speed = 0.0005f;
	radius_ = 120;
	hitDistance = 120;
	strength = 25;
	health_point_ = 300;
	currentAction = relax;
	timeForNewHitSelf = long(6e5);
	timeAfterHitSelf = timeForNewHitSelf;
	timeForNewHit = long(1e6);
	timeAfterHit = timeForNewHit;
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
	if (timeAfterHit >= timeForNewHit)
	{
		if (helper::getDist(position_, boundTarget->get_position()) <= (this->radius_ + boundTarget->get_radius() + hitDistance / 3))
		{
			changeAction(commonHit, true, false);
			timeAfterHit = 0;
		}
		else
			if (helper::getDist(position_, boundTarget->get_position()) > this->radius_ + boundTarget->get_radius() + hitDistance * 2)
			{
				changeAction(directHit, true, false);
				timeAfterHit = 0;
			}
		
	}	
}

void nightmare_third::endingPreviousAction()
{
	if (lastAction == combatState)
		changeAction(relax, true, false);
	if (lastAction == commonHit)
		changeAction(combatState, true, false);	
	if (lastAction == directHit)	
		changeAction(relax, true, false);	
	lastAction = relax;
}

void nightmare_third::on_sprite_change()
{
	if (currentAction == directHit && current_sprite_[0] == 5)
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

