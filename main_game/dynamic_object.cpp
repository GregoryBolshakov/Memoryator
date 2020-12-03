#include "dynamic_object.h"
#include "world_metrics.h"

dynamic_object::dynamic_object(std::string name, const sf::Vector2f position, const int kind) :
	world_object(std::move(name), position, kind)
	, move_system(tag, radius_, position_, color, current_action_, direction_system)
	, direction_system(position_, move_system.move_position, mirrored_)
	, time_for_new_hit_self(creature_time_for_new_hitself)
	, time_for_new_hit(creature_time_for_new_hit)
	, current_action_(action::relax)
	, last_action_(action::relax)
{
}

dynamic_object::~dynamic_object()
= default;

void dynamic_object::handle_input(bool used_mouse, long long elapsed_time)
{
}

void dynamic_object::init_route_blocks()
{
	locked_route_blocks_ = {sf::Vector2i(int(ceil(position_.x / micro_block_size.x)), int(ceil(position_.y / micro_block_size.y))) };
}

bool dynamic_object::is_intersect_dynamic(sf::Vector2f new_position, dynamic_object& other_dynamic) const
{
	const auto position1 = new_position;
	const auto position2 = other_dynamic.get_position();
	return sqrt(pow(position1.x - position2.x, 2) + pow(position1.y - position2.y, 2)) <= /*this->radius + */other_dynamic.radius_;
}

void dynamic_object::change_action(const action new_action, const bool reset_sprite_number, const bool remember_last_action)
{
	if (new_action == action::move && current_action_ != action::move)
		move_system.reset_timers();
	
	if (remember_last_action)
		last_action_ = current_action_;

	current_action_ = new_action;

	if (reset_sprite_number)
		for (auto& number : current_sprite_)
			number = 1;
}

void dynamic_object::take_damage(const float damage, const sf::Vector2f attacker_pos)
{
	if (time_after_hitself_ < time_for_new_hit_self)
		return;
	
	this->time_after_hitself_ = 0;
	this->health_ -= damage / this->armor_;

	move_system.push_damage = damage;
	move_system.push_duration = move_system.default_push_duration;
	move_system.push_rest_duration = move_system.push_duration;
	move_system.red_duration = 2 * move_system.push_duration;
	move_system.red_rest_duration = move_system.red_duration;

	move_system.push_distance = world_metrics::get_dist(this->get_position(), attacker_pos);
	if (attacker_pos != sf::Vector2f(-1, -1))
		move_system.push_direction = sf::Vector2f(this->position_.x - attacker_pos.x, this->position_.y - attacker_pos.y);
}

bool dynamic_object::die_check()
{
	if (health_ > 0)
		return false;
	
	change_action(action::dead, true);
	direction_system.direction = direction::STAND;

	return true;
}