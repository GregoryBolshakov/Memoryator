#include "wolf.h"

using namespace sf;

wolf::wolf(std::string objectName, Vector2f centerPosition) : neutral_mob(std::move(objectName), centerPosition)
{
	conditional_size_units_ = { 250, 200 };
	current_sprite_[0] = 1;
	timeForNewSprite = 0;
	moveSystem.speed = 0.0005f;
	animation_speed_ = 0.0006f;
	animationLength = 8;
	radius_ = 70;
	strength = 10;
	sightRange = helper::GetScreenSize().y * 1 / 2; 
	morality = 5; // from 1 to 10
	fear = 0;
	health_point_ = 0;
	currentAction = relax;
	timeAfterHitSelf = 0;
	timeForNewHitSelf = long(6e5);
	timeForNewHit = 1000000;

	to_save_name_ = "wolf";
}

wolf::~wolf()
= default;

void wolf::setTarget(dynamic_object& object)
{
	return;
}

void wolf::behaviorWithStatic(world_object* target, long long elapsedTime)
{

}

void wolf::behavior(long long elapsedTime)
{
	fightInteract(elapsedTime);
}

Vector2f wolf::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return { texture_box_.width / 2, texture_box_.height * 7 / 8 };
}

Vector2f wolf::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int wolf::get_build_type(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void wolf::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

std::vector<sprite_chain_element*> wolf::prepare_sprites(long long elapsedTime)
{
    return {};
	/*std::string spriteName;

	switch (currentAction)
	{
	case bottomHit:
	{
		animationLength = 8;
		switch (side)
		{
			case up:
			{
				spriteName = "Game/worldSprites/wolf/hit/up/";
				break;
			}
			case right:
			{
				spriteName = "Game/worldSprites/wolf/hit/right/";
				break;
			}
			case down:
			{
				spriteName = "Game/worldSprites/wolf/hit/down/";
				break;
			}
			case left:
			{
				spriteName = "Game/worldSprites/wolf/hit/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite[0]);
		spriteName += ".png";
		break;
	}
	case upperHit:
	{
		animationLength = 8;
		switch (side)
		{
			case up:
			{
				spriteName = "Game/worldSprites/wolf/hit/up/";
				break;
			}
			case right:
			{
				spriteName = "Game/worldSprites/wolf/hit/right/";
				break;
			}
			case down:
			{
				spriteName = "Game/worldSprites/wolf/hit/down/";
				break;
			}
			case left:
			{
				spriteName = "Game/worldSprites/wolf/hit/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite[0]);
		spriteName += ".png";
		break;
	}
	case directHit:
	{
		animationLength = 8;
		switch (side)
		{
			case up:
			{
				spriteName = "Game/worldSprites/wolf/hit/up/";
				break;
			}
			case right:
			{
				spriteName = "Game/worldSprites/wolf/hit/right/";
				break;
			}
			case down:
			{
				spriteName = "Game/worldSprites/wolf/hit/down/";
				break;
			}
			case left:
			{
				spriteName = "Game/worldSprites/wolf/hit/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite[0]);
		spriteName += ".png";
		break;
	}
	case combatState:
	{
		animationLength = 8;
		switch (side)
		{
			case up:
			{
				spriteName = "Game/worldSprites/wolf/move/up/";
				break;
			}
			case right:
			{
				spriteName = "Game/worldSprites/wolf/move/right/";
				break;
			}
			case down:
			{
				spriteName = "Game/worldSprites/wolf/move/down/";
				break;
			}
			case left:
			{
				spriteName = "Game/worldSprites/wolf/move/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite[0]);
		spriteName += ".png";
		break;
	}
	case relax:
	{
		animationLength = 1;
	    spriteName = "Game/worldSprites/wolf/stand/down/";				
		spriteName += std::to_string(currentSprite[0]);
		spriteName += ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		spriteName = "Game/worldSprites/wolf/stand/down/1.png";
		currentSprite[0] = 1;
	}
	}

	if (currentAction == move)
	{
		animationLength = 8;
		switch (direction)
		{
		case Direction::LEFT:
			spriteName = "Game/worldSprites/wolf/move/left/";
			spriteName += std::to_string(currentSprite[0]);
			spriteName += ".png";
			break;
		case Direction::RIGHT:
			spriteName = "Game/worldSprites/wolf/move/right/";
			spriteName += std::to_string(currentSprite[0]);
			spriteName += ".png";
			break;
		case Direction::UP:
			spriteName = "Game/worldSprites/wolf/move/up/";
			spriteName += std::to_string(currentSprite[0]);
			spriteName += ".png";
			break;
		case Direction::DOWN:
			spriteName = "Game/worldSprites/wolf/move/down/";
			spriteName += std::to_string(currentSprite[0]);
			spriteName += ".png";
			break;
		case Direction::UPLEFT:
			spriteName = "Game/worldSprites/wolf/move/left/";
			spriteName += std::to_string(currentSprite[0]);
			spriteName += ".png";
			break;
		case Direction::UPRIGHT:
			spriteName = "Game/worldSprites/wolf/move/right/";
			spriteName += std::to_string(currentSprite[0]);
			spriteName += ".png";
			break;
		case Direction::DOWNLEFT:
			spriteName = "Game/worldSprites/wolf/move/left/";
			spriteName += std::to_string(currentSprite[0]);
			spriteName += ".png";
			break;
		case Direction::DOWNRIGHT:
			spriteName = "Game/worldSprites/wolf/move/right/";
			spriteName += std::to_string(currentSprite[0]);
			spriteName += ".png";
			break;
		default:;
		}
	}

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 40 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			if (currentAction >= (Actions)(0) && currentAction < (Actions)3)
			{
				lastAction = currentAction;
				currentAction = combatState;
			}
			currentSprite[0] = 1;
		}
	}
	*/
}