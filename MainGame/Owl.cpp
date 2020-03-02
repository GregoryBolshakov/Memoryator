#include "owl.h"

#include "fern.h"
#include "forest_tree.h"

using namespace sf;

owl::owl(const std::string& objectName, Vector2f centerPosition) : neutral_mob(objectName, centerPosition)
{
	conditionalSizeUnits = { 280, 200 };
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	moveSystem.defaultSpeed = 0.0006f;
	moveSystem.speed = 0.0006f;
	animationSpeed = 0.0008f;
	animationLength = 8;
	radius = 70;
	strength = 10;
	sightRange = helper::GetScreenSize().y * 1 / 2;
	healthPoint = 50;
	currentAction = relax;
	timeAfterHitSelf = 0;
	timeForNewHitSelf = long(6e5);
	timeForNewHit = 1000000;
	toSaveName = "owl";
	tag = entity_tag::owl;
	moveSystem.canCrashIntoStatic = false;
	zCoord = 10;
}

owl::~owl()
= default;

Vector2f owl::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2, textureBox.height * 7 / 8 };
}

void owl::setTarget(dynamic_object& object)
{	
	if (object.tag == entity_tag::noose || currentAction == absorbs)
		return;
	if (helper::getDist(position, object.getPosition()) <= sightRange)
	{
		if (object.tag == entity_tag::hero)
		{
			boundTarget = &object;
			distanceToNearest = helper::getDist(position, object.getPosition());
		}
	}
}

void owl::behaviorWithStatic(world_object* target, long long elapsedTime)
{
	if (currentAction == absorbs)
		return;

	if (target->tag == entity_tag::fern && helper::getDist(position, target->getPosition()) <= sightRange * 2 && timeAfterFear >= fearTime)
	{
		if (!(boundTarget && boundTarget->tag == entity_tag::hero && (helper::getDist(position, boundTarget->getPosition()) <= sightRange ||
			helper::getDist(target->getPosition(), boundTarget->getPosition()) <= sightRange)))
		{
			boundTarget = target;
			distanceToNearest = helper::getDist(position, target->getPosition());
		}
	}
	else
	if (target->tag == entity_tag::tree)
	{
		auto nearestTreeCasted = dynamic_cast<forest_tree*>(target);
		if (nearestTreeCasted && !(boundTarget && boundTarget->tag == entity_tag::fern))
			if (boundTarget != nullptr && boundTarget->tag == entity_tag::hero && helper::getDist(nearestTreeCasted->getOwlBase(), position) < helper::getDist(nearestTreeCasted->getOwlBase(), boundTarget->getPosition()) &&
				helper::getDist(nearestTreeCasted->getOwlBase(), boundTarget->getPosition()) > helper::getDist(position, boundTarget->getPosition()))
				if ((nearestTree != nullptr && helper::getDist(nearestTreeCasted->getOwlBase(), position) < helper::getDist(nearestTree->getPosition(), position)) || nearestTree == nullptr)
					if (helper::getDist(nearestTreeCasted->getOwlBase(), boundTarget->getPosition()) > sightRange * 1.2)
						nearestTree = target;							
	}
}

void owl::behavior(long long elapsedTime)
{
	endingPreviousAction();
	fightInteract(elapsedTime);
	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		directionSystem.direction = Direction::STAND;
		return;
	}

	// first-priority actions
	if (boundTarget && boundTarget->tag == entity_tag::hero && helper::getDist(position, boundTarget->getPosition()) <= sightRange)
			timeAfterFear = 0;	
	else
		timeAfterFear += elapsedTime;

	if (currentAction == absorbs)
	{
		movePosition = position;
		return;
	}
	//-----------------------	

	if (boundTarget == nullptr || boundTarget && boundTarget->tag == entity_tag::hero && helper::getDist(position, boundTarget->getPosition()) > sightRange)
	{
		auto nearestTreeCasted = dynamic_cast<forest_tree*>(nearestTree);
		if (nearestTree && helper::getDist(position, nearestTreeCasted->getOwlBase()) > radius)
			{				
				changeAction(move, false, true);
				movePosition = nearestTreeCasted->getOwlBase();
				directionSystem.side = direction_system::calculateSide(position, movePosition, elapsedTime);
				return;
			}

		changeAction(relax, true, true);
		directionSystem.direction = Direction::STAND;
		movePosition = position;
		return;
	}

	// bouncing to a trap
	if (boundTarget && boundTarget->tag == entity_tag::fern)
	{
		directionSystem.side = direction_system::calculateSide(position, boundTarget->getPosition(), elapsedTime);
		if (helper::getDist(position, boundTarget->getPosition()) <= radius)
		{
			const auto trap = dynamic_cast<fern*>(boundTarget);
			if (directionSystem.side == right)
				mirrored = true;
			else
				mirrored = false;
			position = trap->getEnterPosition();
			changeAction(absorbs, true, false);
			movePosition = position;
		}
		else
		{
			changeAction(move, false, true);
			movePosition = boundTarget->getPosition();
		}
	}
	//-------------------

	// runaway from enemy
	if (boundTarget && boundTarget->tag == entity_tag::hero)
	{
		const float distanceToTarget = helper::getDist(this->position, boundTarget->getPosition());
		directionSystem.side = direction_system::calculateSide(position, movePosition, elapsedTime);
		moveSystem.speed = std::max(moveSystem.defaultSpeed, (moveSystem.defaultSpeed * 10) * (1 - (helper::getDist(position, boundTarget->getPosition()) / sightRange * 1.5f)));
		animationSpeed = std::max(0.0008f, 0.0008f * moveSystem.speed / moveSystem.defaultSpeed);
		if (distanceToTarget <= sightRange)
		{
			changeAction(move, false, true);
			float k = (sightRange * 1.3f / (sqrt(pow(boundTarget->getPosition().x - position.x, 2) + pow(boundTarget->getPosition().y - position.y, 2))));
			movePosition = Vector2f(position.x + (position.x - boundTarget->getPosition().x) * k, position.y + (position.y - boundTarget->getPosition().y) * k);
		}
	}	
	//-------------------

	if (currentAction != absorbs)
	{
		distanceToNearest = 10e6;
		boundTarget = nullptr;
		nearestTree = nullptr;
	}
}

Vector2f owl::getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int owl::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void owl::endingPreviousAction()
{
	if (lastAction == commonHit)
		currentAction = relax;
	if (lastAction == absorbs)
	{
		auto trap = dynamic_cast<fern*>(boundTarget);
		trap->inventory.emplace_back(entity_tag::owl, 1);
		deletePromiseOn();
	}
	lastAction = relax;
}

void owl::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

std::vector<sprite_chain_element*> owl::prepareSprites(long long elapsedTime)
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
