#include "wolf.h"

using namespace sf;

wolf::wolf(std::string objectName, Vector2f centerPosition) : neutral_mob(std::move(objectName), centerPosition)
{
	conditionalSizeUnits = { 250, 200 };
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	moveSystem.speed = 0.0005f;
	animationSpeed = 0.0006f;
	animationLength = 8;
	radius = 70;
	strength = 10;
	sightRange = Helper::GetScreenSize().y * 1 / 2; 
	morality = 5; // from 1 to 10
	fear = 0;
	healthPoint = 0;
	currentAction = relax;
	timeAfterHitSelf = 0;
	timeForNewHitSelf = long(6e5);
	timeForNewHit = 1000000;

	toSaveName = "wolf";
}

wolf::~wolf()
= default;

void wolf::setTarget(DynamicObject& object)
{
	return;
}

void wolf::behaviorWithStatic(WorldObject* target, long long elapsedTime)
{

}

void wolf::behavior(long long elapsedTime)
{
	fightInteract(elapsedTime);
}

Vector2f wolf::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2, textureBox.height * 7 / 8 };
}

Vector2f wolf::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int wolf::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void wolf::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

std::vector<sprite_chain_element*> wolf::prepareSprites(long long elapsedTime)
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