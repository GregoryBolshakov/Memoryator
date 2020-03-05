#include "owl.h"

#include "fern.h"
#include "forest_tree.h"

using namespace sf;

owl::owl(const std::string& objectName, Vector2f centerPosition) : neutral_mob(objectName, centerPosition)
{
	conditional_size_units_ = { 280, 200 };
	current_sprite_[0] = 1;
	timeForNewSprite = 0;
	moveSystem.default_speed = 0.0006f;
	moveSystem.speed = 0.0006f;
	animation_speed_ = 0.0008f;
	animationLength = 8;
	radius_ = 70;
	strength = 10;
	sightRange = helper::GetScreenSize().y * 1 / 2;
	health_point_ = 50;
	currentAction = relax;
	timeAfterHitSelf = 0;
	timeForNewHitSelf = long(6e5);
	timeForNewHit = 1000000;
	to_save_name_ = "owl";
	tag = entity_tag::owl;
	moveSystem.can_crash_into_static = false;
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

void owl::setTarget(dynamic_object& object)
{	
	if (object.tag == entity_tag::noose || currentAction == absorbs)
		return;
	if (helper::getDist(position_, object.get_position()) <= sightRange)
	{
		if (object.tag == entity_tag::hero)
		{
			boundTarget = &object;
			distanceToNearest = helper::getDist(position_, object.get_position());
		}
	}
}

void owl::behaviorWithStatic(world_object* target, long long elapsedTime)
{
	if (currentAction == absorbs)
		return;

	if (target->tag == entity_tag::fern && helper::getDist(position_, target->get_position()) <= sightRange * 2 && timeAfterFear >= fearTime)
	{
		if (!(boundTarget && boundTarget->tag == entity_tag::hero && (helper::getDist(position_, boundTarget->get_position()) <= sightRange ||
			helper::getDist(target->get_position(), boundTarget->get_position()) <= sightRange)))
		{
			boundTarget = target;
			distanceToNearest = helper::getDist(position_, target->get_position());
		}
	}
	else
	if (target->tag == entity_tag::tree)
	{
		auto nearestTreeCasted = dynamic_cast<forest_tree*>(target);
		if (nearestTreeCasted && !(boundTarget && boundTarget->tag == entity_tag::fern))
			if (boundTarget != nullptr && boundTarget->tag == entity_tag::hero && helper::getDist(nearestTreeCasted->getOwlBase(), position_) < helper::getDist(nearestTreeCasted->getOwlBase(), boundTarget->get_position()) &&
				helper::getDist(nearestTreeCasted->getOwlBase(), boundTarget->get_position()) > helper::getDist(position_, boundTarget->get_position()))
				if ((nearestTree != nullptr && helper::getDist(nearestTreeCasted->getOwlBase(), position_) < helper::getDist(nearestTree->get_position(), position_)) || nearestTree == nullptr)
					if (helper::getDist(nearestTreeCasted->getOwlBase(), boundTarget->get_position()) > sightRange * 1.2)
						nearestTree = target;							
	}
}

void owl::behavior(long long elapsedTime)
{
	endingPreviousAction();
	fightInteract(elapsedTime);
	if (health_point_ <= 0)
	{
		changeAction(dead, true);
		directionSystem.direction = direction::STAND;
		return;
	}

	// first-priority actions
	if (boundTarget && boundTarget->tag == entity_tag::hero && helper::getDist(position_, boundTarget->get_position()) <= sightRange)
			timeAfterFear = 0;	
	else
		timeAfterFear += elapsedTime;

	if (currentAction == absorbs)
	{
		movePosition = position_;
		return;
	}
	//-----------------------	

	if (boundTarget == nullptr || boundTarget && boundTarget->tag == entity_tag::hero && helper::getDist(position_, boundTarget->get_position()) > sightRange)
	{
		auto nearestTreeCasted = dynamic_cast<forest_tree*>(nearestTree);
		if (nearestTree && helper::getDist(position_, nearestTreeCasted->getOwlBase()) > radius_)
			{				
				changeAction(move, false, true);
				movePosition = nearestTreeCasted->getOwlBase();
				directionSystem.side = direction_system::calculate_side(position_, movePosition);
				return;
			}

		changeAction(relax, true, true);
		directionSystem.direction = direction::STAND;
		movePosition = position_;
		return;
	}

	// bouncing to a trap
	if (boundTarget && boundTarget->tag == entity_tag::fern)
	{
		directionSystem.side = direction_system::calculate_side(position_, boundTarget->get_position());
		if (helper::getDist(position_, boundTarget->get_position()) <= radius_)
		{
			const auto trap = dynamic_cast<fern*>(boundTarget);
			if (directionSystem.side == right)
				mirrored_ = true;
			else
				mirrored_ = false;
			position_ = trap->getEnterPosition();
			changeAction(absorbs, true, false);
			movePosition = position_;
		}
		else
		{
			changeAction(move, false, true);
			movePosition = boundTarget->get_position();
		}
	}
	//-------------------

	// runaway from enemy
	if (boundTarget && boundTarget->tag == entity_tag::hero)
	{
		const float distanceToTarget = helper::getDist(this->position_, boundTarget->get_position());
		directionSystem.side = direction_system::calculate_side(position_, movePosition);
		moveSystem.speed = std::max(moveSystem.default_speed, (moveSystem.default_speed * 10) * (1 - (helper::getDist(position_, boundTarget->get_position()) / sightRange * 1.5f)));
		animation_speed_ = std::max(0.0008f, 0.0008f * moveSystem.speed / moveSystem.default_speed);
		if (distanceToTarget <= sightRange)
		{
			changeAction(move, false, true);
			float k = (sightRange * 1.3f / (sqrt(pow(boundTarget->get_position().x - position_.x, 2) + pow(boundTarget->get_position().y - position_.y, 2))));
			movePosition = Vector2f(position_.x + (position_.x - boundTarget->get_position().x) * k, position_.y + (position_.y - boundTarget->get_position().y) * k);
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
	if (lastAction == commonHit)
		currentAction = relax;
	if (lastAction == absorbs)
	{
		auto trap = dynamic_cast<fern*>(boundTarget);
		trap->inventory.emplace_back(entity_tag::owl, 1);
		delete_promise_on();
	}
	lastAction = relax;
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
