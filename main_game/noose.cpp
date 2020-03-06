#include "noose.h"

#include "deer.h"
#include "deerchant.h"


using namespace sf;

noose::noose(const std::string& objectName, const Vector2f centerPosition, world_object* owner) : dynamic_object(objectName, centerPosition)
{
	conditional_size_units_ = {360, 300};
	current_sprite_[0] = 1;
	timeForNewSprite = 0;
	this->owner = owner;
	moveSystem.default_speed = 0.0007f;
	moveSystem.speed = moveSystem.default_speed;
	animation_speed_ = 10;
	animationLength = 8;
	radius_ = 50;
	currentAction = move;
	routeGenerationAbility = false;
	moveSystem.can_crash_into_dynamic = false;
	noose::jerk(2, 1);
	to_save_name_ = "noose";
	tag = entity_tag::noose;
}

noose::~noose()
= default;

Vector2f noose::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	ropeElongation = texture_box_.width / 20.0F;
	//ropeElongation = 0;
	return {0.0F, texture_box_.height / 1.8F};
}

void noose::setTarget(dynamic_object& object)
{
}

void noose::behaviorWithDynamic(dynamic_object* target, long long /*elapsedTime*/)
{
	if (currentAction == dead)
	{
		return;
	}

	if (target->tag == entity_tag::deer)
	{
		auto deer = dynamic_cast<::deer*>(target);

		if (helper::getDist(position_, deer->getHeadPosition()) <= radius_ + target->get_radius())
		{
			boundTarget = target;
			if (deer->getOwner() == nullptr && deer->getCurrentAction() != commonHit)
			{
				deer->changeAction(commonHit, true, false);
			}
			if (owner != nullptr)
			{
				deer->setOwner(owner);
			}
		}
	}
}

void noose::behaviorWithStatic(world_object* /*target*/, long long /*elapsedTime*/)
{
	if (currentAction == dead)
	{
		return;
	}
}

void noose::behavior(long long elapsedTime)
{
	if (currentAction == dead)
	{
		z_coordinate_ = 0;
		return;
	}
	z_coordinate_ = 2;

	endingPreviousAction();
	jerkInteract(elapsedTime);

	if (owner != nullptr)
	{
		ownerPos = Vector2f(owner->get_position().x + owner->get_conditional_size_units().x / 10.0F, owner->get_position().y - owner->get_conditional_size_units().y / 13.0F);
		ownerGlobalBounds = owner->get_conditional_size_units();
	}

	if (boundTarget != nullptr && owner != nullptr)
	{
		if (helper::getDist(position_, owner->get_position()) >= maximumLength)
		{
			auto deer = dynamic_cast<::deer*>(boundTarget);
			if (deer != nullptr)
			{
				deer->setOwner(nullptr);
			}
			stopping(true, true);
			changeAction(dead, true, false);
			owner = nullptr;
		}
	}

	if (boundTarget != nullptr)
	{
		auto deer = dynamic_cast<::deer*>(boundTarget);
		position_ = deer->getHeadPosition();
		moveSystem.speed = 0;
		changeAction(relax, false, true);
	}
}

Vector2f noose::get_build_position(std::vector<world_object*> visible_items, float scale_factor, Vector2f camera_position)
{
	return {-1, -1};
}

int noose::get_build_type(Vector2f oun_pos, Vector2f other_pos)
{
	return 1;
}

void noose::stopping(bool doStand, bool forgetBoundTarget)
{
	if (doStand)
	{
		this->movePosition = {-1, -1};
		directionSystem.direction = direction::STAND;
	}

	if (forgetBoundTarget && boundTarget != nullptr)
	{
		boundTarget->is_processed = false;
		boundTarget = nullptr;
	}
}

void noose::endingPreviousAction()
{
	if (lastAction == jerking)
	{
		currentAction = dead;
	}
	lastAction = relax;
}

void noose::jerkInteract(const long long elapsedTime)
{
	if (currentAction == jerking)
	{
		if (jerkTime > 0)
		{
			jerkTime -= elapsedTime;
			moveSystem.speed = jerkDistance / float(jerkDuration) * jerkPower * float(pow(float(jerkTime) / float(jerkDuration), jerkDeceleration));
			moveSystem.speed = std::max(moveSystem.default_speed / jerkDeceleration, moveSystem.speed);
		}
		else
		{
			moveSystem.speed = 0;
		}
	}
}

void noose::jerk(const float power, const float deceleration, Vector2f /*destinationPoint*/)
{
	stopping(false, false);
	this->jerkPower = power;
	this->jerkDeceleration = deceleration;
	this->jerkDuration = 1e6;
	this->jerkTime = this->jerkDuration;
	currentAction = jerking;
	jerkDistance = 1400;
	current_sprite_[0] = 1;

	const auto mousePos = Vector2f(Mouse::getPosition());
	const auto screenCenter = Vector2f(helper::GetScreenSize().x / 2, helper::GetScreenSize().y / 2);
	const auto coeff = jerkDistance / helper::getDist(mousePos, screenCenter);
	laxMovePosition = Vector2f(owner->get_position().x + (mousePos.x - screenCenter.x) * coeff, owner->get_position().y + (mousePos.y - screenCenter.y) * coeff);
}

void noose::fightInteract(long long elapsedTime, dynamic_object* target)
{
}

void noose::rotateAndExtend(sprite_chain_element* rope, sprite_chain_element* loop) const
{
	auto localElongation = ropeElongation;
	if (currentAction == relax)
	{
		localElongation = 1;
	}

	if (ownerPos != Vector2f(0, 0))
	{
		const auto beginPoint = Vector2f(ownerPos.x, ownerPos.y - ownerGlobalBounds.y / 13.0F);
		rope->size = Vector2f(helper::getDist(beginPoint, position_) + localElongation, rope->size.y); // a little bit longer rope for sprite joining		
		if (position_.y <= beginPoint.y)
		{
			rope->rotation = acos((beginPoint.x - position_.x) / sqrt(pow(beginPoint.x - position_.x, 2) + pow(beginPoint.y - position_.y, 2))) / pi * 180;
		}
		else
		{
			rope->rotation = -acos((beginPoint.x - position_.x) / sqrt(pow(beginPoint.x - position_.x, 2) + pow(beginPoint.y - position_.y, 2))) / pi * 180;
		}
	}

	if (currentAction != relax)
	{
		loop->rotation = rope->rotation + 180;
		loop->offset.x -= sin(loop->rotation / 180 * pi) * texture_box_offset_.y; // rotational position correction
		loop->offset.y -= (1 - cos(loop->rotation / 180 * pi)) * texture_box_offset_.y;
	}

	if (ownerPos != Vector2f(0, 0))
	{
		rope->offset.x += (ownerPos.x - position_.x) * localElongation / helper::getDist(position_, ownerPos); // offset of the extended rope
		rope->offset.y += (ownerPos.y - position_.y) * localElongation / helper::getDist(position_, ownerPos);

		// change position to hero belt
		const auto dynOwner = dynamic_cast<deerchant*>(owner);
		if ((dynOwner != nullptr) && currentAction != dead)
		{
			rope->position = dynOwner->getBeltPosition();
			rope->offset = Vector2f(rope->offset.x + rope->position.x - position_.x, rope->offset.y + rope->position.y - position_.y);
		}
	}
}

std::vector<sprite_chain_element*> noose::prepare_sprites(long long elapsedTime)
{
	std::vector<sprite_chain_element*> result = {};
	auto ropeSprite = new sprite_chain_element(pack_tag::craftObjects, pack_part::noose, direction::DOWN, 3, position_, {conditional_size_units_.x, 30}, {0, 0}, color);
	auto loopSprite = new sprite_chain_element(pack_tag::craftObjects, pack_part::noose, direction::UP, 1, position_, conditional_size_units_, texture_box_offset_, color);

	loopSprite->number = current_sprite_[0];

	switch (currentAction)
	{
	case relax:
		{
			current_sprite_[0] = 1;
			animationLength = 1;
			animation_speed_ = 10;
			ropeSprite->number = 1;
			ropeSprite->size.y = 60;
			if (owner != nullptr)
			{
				if (owner->get_position().x < position_.x)
				{
					ropeSprite->size.y *= -1;
				}
			}
			loopSprite->number = 2;
			loopSprite->direction = direction::DOWN;
			loopSprite->size = Vector2f(40, 30);
			loopSprite->offset = Vector2f(loopSprite->size.x / 1.8F, loopSprite->size.y / 1.8F);
			break;
		}
	case jerking:
		{
			animationLength = 13;
			animation_speed_ = 10;
			ropeSprite->number = 3;
			break;
		}
	case dead:
		{
			animationLength = 1;
			animation_speed_ = 10;

			ropeSprite->number = 3;
			loopSprite->number = 12;

			rotateAndExtend(ropeSprite, loopSprite);

			ropeSprite->is_background = true;
			loopSprite->is_background = true;
			is_background = true;

			result.push_back(ropeSprite);
			result.push_back(loopSprite);
			return result;
		}
	default: ;
	}

	if (currentAction == move)
	{
		animationLength = 13;
		animation_speed_ = 10;
		ropeSprite->number = 3;
	}

	rotateAndExtend(ropeSprite, loopSprite);

	result.push_back(ropeSprite);
	result.push_back(loopSprite);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= long(1e6 / animation_speed_))
	{
		timeForNewSprite = 0;

		if (++current_sprite_[0] > animationLength)
		{
			lastAction = currentAction;
			current_sprite_[0] = 1;
		}
	}

	set_unscaled(result);
	return result;
}
