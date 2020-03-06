#include "owl_boss.h"

using namespace sf;

owl_boss::owl_boss(std::string objectName, Vector2f centerPosition) : dynamic_object(std::move(objectName), centerPosition)
{
	conditional_size_units_ = { 600, 600 };
	current_sprite_[0] = 1;
	time_for_new_sprite_ = 0;
	move_system_.default_speed = 0.0005f;
	animation_speed_ = 0.0005f;
	animation_length_ = 1;
	current_sprite_[0] = 1;
	radius_ = 100;
	strength_ = 100;
	health_point_ = 1000;
	current_action_ = rightFlap;
	time_after_hitself_ = 0;
	time_for_new_hit_self = long(6e5);
	time_for_new_hit = 2000000;
	route_generation_ability_ = false;
	move_system_.can_crash_into_dynamic = false;

	to_save_name_ = "monster";
}

owl_boss::~owl_boss()
= default;

void owl_boss::behavior_with_static(world_object* target, long long elapsedTime)
{

}

void owl_boss::behavior(long long elapsedTime)
{
	fight_interact(elapsedTime);
	//jerk interaction
	if (is_jerking_)
	{
		if (jerk_time_ > 0)
		{
			jerk_time_ -= elapsedTime;
			move_system_.speed = jerk_distance_ / float(jerk_duration_) * float(pow(jerk_time_ / jerk_duration_, jerk_deceleration_));
			move_system_.speed = std::max(move_system_.default_speed / jerk_deceleration_, move_system_.speed);
			//speed = defaultSpeed;
		}
		else
		{
			is_jerking_ = false;
			move_system_.speed = move_system_.default_speed;
		}
	}
	//----------------

	//processing ending of previous actions
	if (last_action_ == upFlap || last_action_ == leftFlap || last_action_ == rightFlap || last_action_ == startFlap)
	{
		if (--flaps_before_jerk_count_ > 0)
		{
			if (flaps_before_jerk_count_ == 1)
				change_action(startFlap, true, false);
			else
				change_action(actions(30 + rand() % 3), true, false);
		}
		last_action_ = relax;
		//changeAction(rightFlap, false, true);
		//lastAction = relax;
	}

	//------------------

	//processing current actions
	if (current_action_ == rightFlap && !is_jerking_ && current_sprite_[0] == 7)
		jerk(2, 1, Vector2f(position_.x - 200, position_.y));
	if (current_action_ == leftFlap && !is_jerking_ && current_sprite_[0] == 7)
		jerk(2, 1, Vector2f(position_.x + 200, position_.y));
	//--------------------------
}

Vector2f owl_boss::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return { texture_box_.width / 2, texture_box_.height * 7 / 8 };
}

void owl_boss::set_target(dynamic_object& object)
{
	if (object.tag == entity_tag::hero)
		return; //targetPosition = object.getPosition();
}

void owl_boss::behavior_with_dynamic(dynamic_object* target, const long long elapsedTime)
{
	debug_info = std::to_string(helper::getDist(this->position_, move_position_) / jerk_distance_ * 1000.0f);
	if (health_point_ <= 0)
	{
		change_action(dead, true, false);
		direction_system_.direction = direction::STAND;
		return;
	}

	if (target->tag != entity_tag::hero)
		return;

	direction_system_.side = direction_system::calculate_side(position_, move_position_);

	time_after_hit += elapsedTime;

	if (jerk_distance_ != 0 && current_action_ == move)
	{
		const auto dragCoefficient = 1.2f; // 1..2
		const auto startPower = 1.1f; //1..2
		move_system_.speed = pow(move_system_.default_speed, dragCoefficient / (startPower + float(time_after_hit) / float(time_for_new_hit)));
		if (helper::getDist(this->position_, target->get_position()) / jerk_distance_ <= 0.2f)
			move_system_.speed *= helper::getDist(this->position_, target->get_position()) / jerk_distance_;
	}
	move_system_.speed = std::max(move_system_.default_speed / 10, move_system_.speed);

	if (flaps_before_jerk_count_ > 1)
	{
		time_after_hit = 0;
		if (!is_jerking_)
			move_position_ = this->position_;
		return;
	}
	if (flaps_before_jerk_count_ == 0)
	{
		change_action(move, false, true);
		move_system_.speed = move_system_.default_speed;
		jerk_distance_ = helper::getDist(this->position_, target->get_position());
	}

	if (move_system_.speed <= move_system_.default_speed * 3) //with greater acceleration the owl loses the ability to coordinate the route
	{
		move_position_ = Vector2f(target->get_position().x + (target->get_position().x - this->position_.x) / 1.0f, target->get_position().y + (target->get_position().y - this->position_.y) / 1.0f);
	}

	if (helper::getDist(this->position_, move_position_) / move_system_.speed <= (40 / animation_speed_) * 3 && current_action_ == move)
	{
		change_action(stopFlap, true, false);
	}

	if (helper::getDist(this->position_, move_position_) <= this->radius_) //hit interaction
	{
		if (helper::getDist(this->position_, target->get_position()) <= this->radius_)
			target->take_damage(this->strength_, position_);
		time_after_hit = 0;
		flaps_before_jerk_count_ = rand() % 3 + 3;
		change_action(upFlap, true, true);
		move_system_.speed = move_system_.default_speed;
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
	this->jerk_power_ = power;
	this->jerk_deceleration_ = deceleration;
	this->jerk_duration_ = long(40.0f / animation_speed_ * 10.0f);
	this->jerk_time_ = this->jerk_duration_;
	is_jerking_ = true;
	jerk_distance_ = 400;

	move_position_ = Vector2f(destinationPoint);
}

void owl_boss::fight_interact(const long long elapsedTime, dynamic_object* target)
{
	move_system_.push_away(elapsedTime);
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
		if (move_position_.y >= this->position.y)
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
		if (move_position_.y >= this->position.y)
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
		if (move_position_.y >= this->position.y)
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
		if (move_position_.y >= this->position.y)
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
		if (move_position_.y >= this->position.y)
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
		if (move_position_.y >= this->position.y)
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