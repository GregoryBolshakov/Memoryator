#include "owl_boss.h"

using namespace sf;

owl_boss::owl_boss(std::string objectName, Vector2f centerPosition) : dynamic_object(std::move(objectName), centerPosition)
{
	conditional_size_units_ = { 600, 600 };
	current_sprite_[0] = 1;
	timeForNewSprite = 0;
	moveSystem.defaultSpeed = 0.0005f;
	animation_speed_ = 0.0005f;
	animationLength = 1;
	current_sprite_[0] = 1;
	radius_ = 100;
	strength = 100;
	health_point_ = 1000;
	currentAction = rightFlap;
	timeAfterHitSelf = 0;
	timeForNewHitSelf = long(6e5);
	timeForNewHit = 2000000;
	routeGenerationAbility = false;
	moveSystem.canCrashIntoDynamic = false;

	to_save_name_ = "monster";
}

owl_boss::~owl_boss()
= default;

void owl_boss::behaviorWithStatic(world_object* target, long long elapsedTime)
{

}

void owl_boss::behavior(long long elapsedTime)
{
	fightInteract(elapsedTime);
	//jerk interaction
	if (isJerking)
	{
		if (jerkTime > 0)
		{
			jerkTime -= elapsedTime;
			moveSystem.speed = jerkDistance / float(jerkDuration) * float(pow(jerkTime / jerkDuration, jerkDeceleration));
			moveSystem.speed = std::max(moveSystem.defaultSpeed / jerkDeceleration, moveSystem.speed);
			//speed = defaultSpeed;
		}
		else
		{
			isJerking = false;
			moveSystem.speed = moveSystem.defaultSpeed;
		}
	}
	//----------------

	//processing ending of previous actions
	if (lastAction == upFlap || lastAction == leftFlap || lastAction == rightFlap || lastAction == startFlap)
	{
		if (--flapsBeforeJerkCount > 0)
		{
			if (flapsBeforeJerkCount == 1)
				changeAction(startFlap, true, false);
			else
				changeAction(actions(30 + rand() % 3), true, false);
		}
		lastAction = relax;
		//changeAction(rightFlap, false, true);
		//lastAction = relax;
	}

	//------------------

	//processing current actions
	if (currentAction == rightFlap && !isJerking && current_sprite_[0] == 7)
		jerk(2, 1, Vector2f(position_.x - 200, position_.y));
	if (currentAction == leftFlap && !isJerking && current_sprite_[0] == 7)
		jerk(2, 1, Vector2f(position_.x + 200, position_.y));
	//--------------------------
}

Vector2f owl_boss::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return { texture_box_.width / 2, texture_box_.height * 7 / 8 };
}

void owl_boss::setTarget(dynamic_object& object)
{
	if (object.tag == entity_tag::hero)
		return; //targetPosition = object.getPosition();
}

void owl_boss::behaviorWithDynamic(dynamic_object* target, const long long elapsedTime)
{
	debugInfo = std::to_string(helper::getDist(this->position_, movePosition) / jerkDistance * 1000.0f);
	if (health_point_ <= 0)
	{
		changeAction(dead, true, false);
		directionSystem.direction = direction::STAND;
		return;
	}

	if (target->tag != entity_tag::hero)
		return;

	directionSystem.side = direction_system::calculate_side(position_, movePosition);

	timeAfterHit += elapsedTime;

	if (jerkDistance != 0 && currentAction == move)
	{
		const auto dragCoefficient = 1.2f; // 1..2
		const auto startPower = 1.1f; //1..2
		moveSystem.speed = pow(moveSystem.defaultSpeed, dragCoefficient / (startPower + float(timeAfterHit) / float(timeForNewHit)));
		if (helper::getDist(this->position_, target->get_position()) / jerkDistance <= 0.2f)
			moveSystem.speed *= helper::getDist(this->position_, target->get_position()) / jerkDistance;
	}
	moveSystem.speed = std::max(moveSystem.defaultSpeed / 10, moveSystem.speed);

	if (flapsBeforeJerkCount > 1)
	{
		timeAfterHit = 0;
		if (!isJerking)
			movePosition = this->position_;
		return;
	}
	if (flapsBeforeJerkCount == 0)
	{
		changeAction(move, false, true);
		moveSystem.speed = moveSystem.defaultSpeed;
		jerkDistance = helper::getDist(this->position_, target->get_position());
	}

	if (moveSystem.speed <= moveSystem.defaultSpeed * 3) //with greater acceleration the owl loses the ability to coordinate the route
	{
		movePosition = Vector2f(target->get_position().x + (target->get_position().x - this->position_.x) / 1.0f, target->get_position().y + (target->get_position().y - this->position_.y) / 1.0f);
	}

	if (helper::getDist(this->position_, movePosition) / moveSystem.speed <= (40 / animation_speed_) * 3 && currentAction == move)
	{
		changeAction(stopFlap, true, false);
	}

	if (helper::getDist(this->position_, movePosition) <= this->radius_) //hit interaction
	{
		if (helper::getDist(this->position_, target->get_position()) <= this->radius_)
			target->take_damage(this->strength, position_);
		timeAfterHit = 0;
		flapsBeforeJerkCount = rand() % 3 + 3;
		changeAction(upFlap, true, true);
		moveSystem.speed = moveSystem.defaultSpeed;
	}
}

Vector2f owl_boss::get_build_position(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

int owl_boss::get_build_type(Vector2f, Vector2f)
{
	return 1;
}

void owl_boss::jerk(const float power, const float deceleration, const Vector2f destinationPoint)
{
	this->jerkPower = power;
	this->jerkDeceleration = deceleration;
	this->jerkDuration = long(40.0f / animation_speed_ * 10.0f);
	this->jerkTime = this->jerkDuration;
	isJerking = true;
	jerkDistance = 400;

	movePosition = Vector2f(destinationPoint);
}

void owl_boss::fightInteract(const long long elapsedTime, dynamic_object* target)
{
	moveSystem.pushAway(elapsedTime);
}

std::vector<sprite_chain_element*> owl_boss::prepare_sprites(long long elapsedTime)
{
    return {};
	/*spriteChainElement legsSprite, bodySprite, fullSprite;
	additionalSprites.clear();
	legsSprite.offset = Vector2f(this->textureBoxOffset);
	legsSprite.size = Vector2f(this->conditionalSizeUnits);
	bodySprite.offset = Vector2f(this->textureBoxOffset);
	bodySprite.size = Vector2f(this->conditionalSizeUnits);
	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);

	switch (currentAction)
	{
	case upFlap:
	{
		animationLength = 10;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/flap/upFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/flap/upFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case leftFlap:
	{
		animationLength = 10;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/flap/leftFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/flap/leftFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case rightFlap:
	{
		animationLength = 10;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/flap/rightFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/flap/rightFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case startFlap:
	{
		animationLength = 3;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/startFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/startFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case stopFlap:
	{
		animationLength = 3;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/stopFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/stopFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case relax:
	{
		animationLength = 1;
		animationSpeed = 0.0005f;
		bodySprite.path = "Game/worldSprites/owlBoss/stand/down/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		bodySprite.path = "Game/worldSprites/owlBoss/stand/down/1.png";
		currentSprite[0] = 1;
	}
	}

	if (currentAction == move)
	{
		animationLength = 12;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/jerk/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/jerk/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
	}

	additionalSprites.push_back(bodySprite);

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