//#include "noose.h"
//
//#include "deer.h"
//#include "deerchant.h"
//#include "math_constants.h"
//
//using namespace sf;
//
//noose::noose(const std::string& objectName, const sf::Vector2f centerPosition, world_object* owner) : dynamic_object(objectName, centerPosition)
//{
//	conditional_size_units_ = {360, 300};
//	current_sprite_[0] = 1;
//	timeForNewSprite = 0;
//	this->owner = owner;
//	move_system.default_speed = 0.0007f;
//	move_system.speed = move_system.default_speed;
//	animation_speed_ = 10;
//	animationLength = 8;
//	radius_ = 50;
//	current_action_ = move;
//	move_system.route_generation_ability = false;
//	move_system.can_crash_into_dynamic = false;
//	move_system.can_crash_into_static = false;
//	noose::jerk(2, 1);
//	to_save_name_ = "noose";
//	tag = entity_tag::noose;
//}
//
//noose::~noose()
//= default;
//
//Vector2f noose::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//	ropeElongation = texture_box_.width / 20.0F;
//	//ropeElongation = 0;
//	return {0.0F, texture_box_.height / 1.8F};
//}
//
//void noose::set_target(dynamic_object& object)
//{
//}
//
//void noose::behavior_with_dynamic(dynamic_object* target, long long /*elapsedTime*/)
//{
//	if (current_action_ == dead)
//	{
//		return;
//	}
//
//	if (target->tag == entity_tag::deer)
//	{
//		auto deer = dynamic_cast<::deer*>(target);
//
//		if (helper::getDist(position_, deer->getHeadPosition()) <= radius_ + target->get_radius())
//		{
//			bound_target_ = target;
//			if (deer->getOwner() == nullptr && deer->get_current_action() != common_hit)
//			{
//				deer->change_action(common_hit, true, false);
//			}
//			if (owner != nullptr)
//			{
//				deer->setOwner(owner);
//			}
//		}
//	}
//}
//
//void noose::behavior_with_static(world_object* /*target*/, long long /*elapsedTime*/)
//{
//	if (current_action_ == dead)
//	{
//		return;
//	}
//}
//
//void noose::behavior(long long elapsedTime)
//{
//	if (current_action_ == dead)
//	{
//		z_coordinate_ = 0;
//		return;
//	}
//	z_coordinate_ = 2;
//
//	endingPreviousAction();
//	jerkInteract(elapsedTime);
//
//	if (owner != nullptr)
//	{
//		ownerPos = sf::Vector2f(owner->get_position().x + owner->get_conditional_size_units().x / 10.0F, owner->get_position().y - owner->get_conditional_size_units().y / 13.0F);
//		ownerGlobalBounds = owner->get_conditional_size_units();
//	}
//
//	if (bound_target_ != nullptr && owner != nullptr)
//	{
//		if (helper::getDist(position_, owner->get_position()) >= maximumLength)
//		{
//			auto deer = dynamic_cast<::deer*>(bound_target_);
//			if (deer != nullptr)
//			{
//				deer->setOwner(nullptr);
//			}
//			stopping(true, true);
//			change_action(dead, true, false);
//			owner = nullptr;
//		}
//	}
//
//	if (bound_target_ != nullptr)
//	{
//		auto deer = dynamic_cast<::deer*>(bound_target_);
//		position_ = deer->getHeadPosition();
//		move_system.speed = 0;
//		change_action(relax, false, true);
//	}
//}
//
//Vector2f noose::get_build_position(std::vector<world_object*> visible_items, float scale_factor, sf::Vector2f camera_position)
//{
//	return {-1, -1};
//}
//
//int noose::get_build_type(sf::Vector2f oun_pos, sf::Vector2f other_pos)
//{
//	return 1;
//}
//
//void noose::stopping(bool doStand, bool forgetBoundTarget)
//{
//	if (doStand)
//	{
//		move_system.lax_move_position = {-1, -1};
//		direction_system.direction = direction::STAND;
//	}
//
//	if (forgetBoundTarget && bound_target_ != nullptr)
//	{
//		bound_target_->is_processed = false;
//		bound_target_ = nullptr;
//	}
//}
//
//void noose::endingPreviousAction()
//{
//	if (last_action_ == jerking)
//	{
//		current_action_ = dead;
//	}
//	last_action_ = relax;
//}
//
//void noose::jerkInteract(const long long elapsedTime)
//{
//	if (current_action_ == jerking)
//	{
//		if (jerk_time_ > 0)
//		{
//			jerk_time_ -= elapsedTime;
//			move_system.speed = jerk_distance_ / float(jerk_duration_) * jerk_power_ * float(pow(float(jerk_time_) / float(jerk_duration_), jerk_deceleration_));
//			move_system.speed = std::max(move_system.default_speed / jerk_deceleration_, move_system.speed);
//		}
//		else
//		{
//			move_system.speed = 0;
//		}
//	}
//}
//
//void noose::jerk(const float power, const float deceleration, sf::Vector2f /*destinationPoint*/)
//{
//	stopping(false, false);
//	this->jerk_power_ = power;
//	this->jerk_deceleration_ = deceleration;
//	this->jerk_duration_ = long long(1e6);
//	this->jerk_time_ = this->jerk_duration_;
//	current_action_ = jerking;
//	jerk_distance_ = 1400;
//	current_sprite_[0] = 1;
//
//	const auto mousePos = sf::Vector2f(sf::Mouse::getPosition());
//	const auto screenCenter = sf::Vector2f(helper::GetScreenSize().x / 2, helper::GetScreenSize().y / 2);
//	const auto coeff = jerk_distance_ / helper::getDist(mousePos, screenCenter);
//	move_system.lax_move_position = sf::Vector2f(owner->get_position().x + (mousePos.x - screenCenter.x) * coeff, owner->get_position().y + (mousePos.y - screenCenter.y) * coeff);
//}
//
//void noose::fight_interact(long long elapsedTime, dynamic_object* target)
//{
//}
//
//void noose::rotateAndExtend(sprite_chain_element* rope, sprite_chain_element* loop) const
//{
//	auto localElongation = ropeElongation;
//	if (current_action_ == relax)
//	{
//		localElongation = 1;
//	}
//
//	if (ownerPos != sf::Vector2f(0, 0))
//	{
//		const auto beginPoint = sf::Vector2f(ownerPos.x, ownerPos.y - ownerGlobalBounds.y / 13.0F);
//		rope->size = sf::Vector2f(helper::getDist(beginPoint, position_) + localElongation, rope->size.y); // a little bit longer rope for sprite joining		
//		if (position_.y <= beginPoint.y)
//		{
//			rope->rotation = float(acos((beginPoint.x - position_.x) / sqrt(pow(beginPoint.x - position_.x, 2) + pow(beginPoint.y - position_.y, 2))) / pi * 180);
//		}
//		else
//		{
//			rope->rotation = float(-acos((beginPoint.x - position_.x) / sqrt(pow(beginPoint.x - position_.x, 2) + pow(beginPoint.y - position_.y, 2))) / pi * 180);
//		}
//	}
//
//	if (current_action_ != relax)
//	{
//		loop->rotation = rope->rotation + 180;
//		loop->offset.x -= float(sin(loop->rotation / 180 * pi)) * texture_box_offset_.y; // rotational position correction
//		loop->offset.y -= float(1 - cos(loop->rotation / 180 * pi)) * texture_box_offset_.y;
//	}
//
//	if (ownerPos != sf::Vector2f(0, 0))
//	{
//		rope->offset.x += (ownerPos.x - position_.x) * localElongation / helper::getDist(position_, ownerPos); // offset of the extended rope
//		rope->offset.y += (ownerPos.y - position_.y) * localElongation / helper::getDist(position_, ownerPos);
//
//		// change position to hero belt
//		const auto dynOwner = dynamic_cast<deerchant*>(owner);
//		if ((dynOwner != nullptr) && current_action_ != dead)
//		{
//			rope->position = dynOwner->get_belt_position();
//			rope->offset = sf::Vector2f(rope->offset.x + rope->position.x - position_.x, rope->offset.y + rope->position.y - position_.y);
//		}
//	}
//}
//
//std::vector<sprite_chain_element*> noose::prepare_sprites(long long elapsedTime)
//{
//	std::vector<sprite_chain_element*> result = {};
//	auto ropeSprite = new sprite_chain_element(pack_tag::craftObjects, pack_part::noose, direction::DOWN, 3, position_, {conditional_size_units_.x, 30}, {0, 0}, color);
//	auto loopSprite = new sprite_chain_element(pack_tag::craftObjects, pack_part::noose, direction::UP, 1, position_, conditional_size_units_, texture_box_offset_, color);
//
//	loopSprite->number = current_sprite_[0];
//
//	switch (current_action_)
//	{
//	case relax:
//		{
//			current_sprite_[0] = 1;
//			animationLength = 1;
//			animation_speed_ = 10;
//			ropeSprite->number = 1;
//			ropeSprite->size.y = 60;
//			if (owner != nullptr)
//			{
//				if (owner->get_position().x < position_.x)
//				{
//					ropeSprite->size.y *= -1;
//				}
//			}
//			loopSprite->number = 2;
//			loopSprite->direction = direction::DOWN;
//			loopSprite->size = sf::Vector2f(40, 30);
//			loopSprite->offset = sf::Vector2f(loopSprite->size.x / 1.8F, loopSprite->size.y / 1.8F);
//			break;
//		}
//	case jerking:
//		{
//			animationLength = 13;
//			animation_speed_ = 10;
//			ropeSprite->number = 3;
//			break;
//		}
//	case dead:
//		{
//			animationLength = 1;
//			animation_speed_ = 10;
//
//			ropeSprite->number = 3;
//			loopSprite->number = 12;
//
//			rotateAndExtend(ropeSprite, loopSprite);
//
//			ropeSprite->is_background = true;
//			loopSprite->is_background = true;
//			is_background = true;
//
//			result.push_back(ropeSprite);
//			result.push_back(loopSprite);
//			return result;
//		}
//	default: ;
//	}
//
//	if (current_action_ == move)
//	{
//		animationLength = 13;
//		animation_speed_ = 10;
//		ropeSprite->number = 3;
//	}
//
//	rotateAndExtend(ropeSprite, loopSprite);
//
//	result.push_back(ropeSprite);
//	result.push_back(loopSprite);
//
//	timeForNewSprite += elapsedTime;
//
//	if (timeForNewSprite >= long(1e6 / animation_speed_))
//	{
//		timeForNewSprite = 0;
//
//		if (++current_sprite_[0] > animationLength)
//		{
//			last_action_ = current_action_;
//			current_sprite_[0] = 1;
//		}
//	}
//
//	set_unscaled(result);
//	return result;
//}
