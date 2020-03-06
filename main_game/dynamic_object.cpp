#include "dynamic_object.h"

#include "helper.h"

dynamic_object::dynamic_object(std::string objectName, const Vector2f centerPosition) : world_object(std::move(objectName), centerPosition), current_action_()
{
	move_system_.init(&tag, &radius_, &position_, &color, &current_action_, &direction_system_);
	direction_system_.init(&position_, &move_position_);
}

dynamic_object::~dynamic_object()
= default;

void dynamic_object::handle_input(bool used_mouse)
{
}

void dynamic_object::init_micro_blocks()
{
	locked_micro_blocks_ = { Vector2i(int(ceil(position_.x / micro_block_size.x)), int(ceil(position_.y / micro_block_size.y))) };
}

bool dynamic_object::is_intersect_dynamic(Vector2f new_position, dynamic_object& other_dynamic) const
{
	//Vector2f position1 = dynamic1.getPosition();
	const auto position1 = new_position;
	const auto position2 = other_dynamic.get_position();
	//return (abs(position1.x - position2.x) <= (this->getRadius() + otherDynamic.getRadius()) && abs(position1.y - position2.y) <= (this->getRadius() + otherDynamic.getRadius()));
	return sqrt(pow(position1.x - position2.x, 2) + pow(position1.y - position2.y, 2)) <= /*this->radius + */other_dynamic.radius_;
}

void dynamic_object::change_action(const actions new_action, const bool reset_sprite_number, const bool remember_last_action)
{
	if (remember_last_action)
		last_action_ = current_action_;

	current_action_ = new_action;

	if (reset_sprite_number)
		for (auto& number : current_sprite_)
			number = 1;
}

void dynamic_object::take_damage(const float damage, const Vector2f attacker_pos)
{
	if (time_after_hitself_ < time_for_new_hit_self)
		return;
	
	this->time_after_hitself_ = 0;
	this->health_point_ -= damage / this->armor_;

	move_system_.push_damage = damage;
	move_system_.push_duration = move_system_.default_push_duration;
	move_system_.push_rest_duration = move_system_.push_duration;
	move_system_.red_duration = 2 * move_system_.push_duration;
	move_system_.red_rest_duration = move_system_.red_duration;

	move_system_.push_distance = helper::getDist(this->get_position(), attacker_pos);
	if (attacker_pos != Vector2f(-1, -1))
		move_system_.push_direction = Vector2f(this->position_.x - attacker_pos.x, this->position_.y - attacker_pos.y);
}