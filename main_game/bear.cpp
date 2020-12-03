//#include "bear.h"
//
//using namespace sf;
//
//bear::bear(const std::string& objectName, sf::Vector2f centerPosition) : neutral_mob(objectName, centerPosition)
//{
//	size_ = { 432, 384 };
//	current_sprite_[0] = 1;
//	timeForNewSprite = 0;
//	move_system.default_speed = 0.0003f;
//	move_system.speed = 0.0003f;
//	animation_speed_ = float(5e-4);
//	animationLength = 8;
//	radius_ = 70;
//	strength_ = 10;
//	sight_range = size_.x * 3;
//	morality_ = 5; // from 1 to 10
//	fear_ = 0;
//	health_ = 50;
//	current_action_ = relax;	
//	time_for_new_hit_self = long(6e5);
//	time_after_hitself_ = time_for_new_hit_self;
//	timeForNewHit = 10e6;
//	tag = entity_tag::bear;
//}
//
//bear::~bear()
//= default;
//
//Vector2f bear::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//	return { texture_box_.width / 2, texture_box_.height * 7 / 8 };
//}
//
//void bear::behavior_with_static(world_object* target, long long elapsedTime)
//{
//
//}
//
//void bear::behavior(long long elapsedTime)
//{
//	endingPreviousAction();
//	fight_interact(elapsedTime);
//	if (health_ <= 0)
//	{
//		change_action(dead, true);
//		direction_system.direction = direction::STAND;
//		return;
//	}
//
//	if (this->owner != nullptr)
//	{
//		move_system.speed = move_system.default_speed;
//		if (current_action_ == common_hit)
//		{
//			move_system.lax_move_position = position_;
//			return;
//		}
//		direction_system.side = direction_system.calculate_side(position_, owner->get_position(), elapsedTime);
//		if (helper::getDist(position_, owner->get_position()) > sight_range / 2)
//		{
//			change_action(grab, false, false);
//			move_system.lax_move_position = owner->get_position();
//		}
//		else
//			if (helper::getDist(position_, owner->get_position()) <= sight_range / 2.5 && current_action_ != relax)
//			{
//				change_action(relax, true, false);
//				move_system.lax_move_position = position_;
//			}
//		return;
//	}
//
//	direction_system.side = direction_system.calculate_side(position_, move_system.lax_move_position, elapsedTime);
//
//	if (bound_target_ == nullptr)
//		return;
//	const float distanceToTarget = helper::getDist(this->position_, bound_target_->get_position());
//	move_system.speed = std::max(move_system.default_speed, (move_system.default_speed * 10) * (1 - (distanceToTarget) / sight_range * 1.5f));
//	//animationSpeed = std::max(0.0003f, 0.0003f * speed / defaultSpeed);
//
//	if (distanceToTarget <= sight_range)
//	{
//		change_action(move, false, true);
//		move_system.lax_move_position = sf::Vector2f(position_.x - (bound_target_->get_position().x - position_.x), position_.y - (bound_target_->get_position().y - position_.y));
//	}
//	else
//	{
//		if (current_action_ == move)
//		{
//			if (distanceToTarget >= sight_range * 1.5)
//			{
//				change_action(relax, true, true);
//				direction_system.direction = direction::STAND;
//				move_system.lax_move_position = { -1, -1 };
//			}
//			else
//				move_system.lax_move_position = sf::Vector2f(position_.x - (bound_target_->get_position().x - position_.x), position_.y - (bound_target_->get_position().y - position_.y));
//		}
//	}
//	distance_to_nearest_ = 10e6;
//	bound_target_ = nullptr;
//}
//
//Vector2f bear::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition)
//{
//	return { -1, -1 };
//}
//
//int bear::get_build_type(sf::Vector2f ounPos, sf::Vector2f otherPos)
//{
//	return 1;
//}
//
//void bear::endingPreviousAction()
//{
//	if (last_action_ == common_hit)
//		current_action_ = relax;
//
//	last_action_ = relax;
//}
//
//void bear::jerk(float power, float deceleration, sf::Vector2f destinationPoint)
//{
//	return;
//}
//
//std::vector<sprite_chain_element*> bear::prepare_sprites(long long elapsedTime)
//{
//    return {};
//	/*spriteChainElement fullSprite;
//
//	fullSprite.offset = sf::Vector2f(this->textureBoxOffset);
//	fullSprite.size = sf::Vector2f(this->conditionalSizeUnits);
//	additionalSprites.clear();
//	std::string sideStr = DynamicObject::sideToString(side), directionStr = DynamicObject::directionToString(direction);
//	if (direction == Direction::RIGHT || direction == Direction::UPRIGHT || direction == Direction::DOWNRIGHT)
//	{
//		directionStr = "left";
//		fullSprite.mirrored = true;
//	}
//
//	switch (currentAction)
//	{
//	case relax:
//	{
//		animationLength = 1;
//		animationSpeed = 10;
//		fullSprite.path = "Game/worldSprites/bear/stand/" + sideStr + '/';
//		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
//		break;
//	}
//	case commonHit:
//	{
//		animationLength = 6;
//		animationSpeed = 10;
//		fullSprite.path = "Game/worldSprites/bear/hunt/" + sideStr + '/';
//		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
//		break;
//	}
//	case dead:
//	{
//		animationLength = 1;
//		animationSpeed = 10;
//		fullSprite.path = "Game/worldSprites/bear/stand/down/1.png";
//		currentSprite[0] = 1;
//	}
//	default:;
//	}
//
//	if (currentAction == move || currentAction == grab)
//	{
//		animationLength = 7;
//		animationSpeed = 10;
//		fullSprite.path = "Game/worldSprites/bear/move/" + directionStr + '/';
//		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
//	}
//
//	additionalSprites.push_back(fullSprite);
//
//	timeForNewSprite += elapsedTime;
//
//	if (timeForNewSprite >= 1000000 / animationSpeed)
//	{
//		timeForNewSprite = 0;
//
//		if (++currentSprite[0] > animationLength)
//		{
//			lastAction = currentAction;
//			currentSprite[0] = 1;
//		}
//	}*/
//}