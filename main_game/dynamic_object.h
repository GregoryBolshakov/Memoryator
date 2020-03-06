#pragma once
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "move_system.h"
#include "world_object.h"

class dynamic_object : public world_object
{
protected:
	move_system move_system_;
	direction_system direction_system_;
	int ellipse_size_{};
	Vector2f move_position_ = { -1, -1 };
	Vector2f focus1_, focus2_;
	float strength_ = 0;
	long long time_after_hitself_ = 0;

	actions current_action_, last_action_ = relax;
	world_object* bound_target_ = nullptr;
	bool route_generation_ability_ = true;

	//fight logic
	virtual void fight_interact(long long elapsed_time, dynamic_object* target = nullptr) = 0;
	//-----------

	const long long default_push_duration_ = long(2e5);
	const float default_push_speed_ = 0.00025f;

	float jerk_power_{};
	float jerk_deceleration_ = 0;
	float jerk_distance_ = 0;
	long long jerk_duration_ = long(1e6);
	long long jerk_time_ = 0;
	bool is_jerking_ = false;
public:
	dynamic_object(std::string objectName, Vector2f centerPosition);
	virtual ~dynamic_object();
	int get_sprite_number() override { return current_sprite_[0]; }
	[[nodiscard]] int get_ellipse_size() const { return ellipse_size_; }
	[[nodiscard]] move_system get_move_system() const { return move_system_; }
	[[nodiscard]] direction_system get_direction_system() const { return direction_system_; }
	[[nodiscard]] float get_strength() const { return strength_; }
	[[nodiscard]] long long get_time_after_hit_self() const { return time_after_hitself_; }
	[[nodiscard]] bool get_route_generation_ability() const { return route_generation_ability_; }
	[[nodiscard]] Vector2f get_focus1() const { return focus1_; }
	[[nodiscard]] Vector2f get_focus2() const { return focus2_; }
	[[nodiscard]] Vector2f get_move_position() const { return move_position_; }
	[[nodiscard]] actions get_current_action() const { return current_action_; }
	[[nodiscard]] world_object *get_bound_target() const { return bound_target_; }

	void set_current_action(actions action) { this->current_action_ = action; }	
	virtual void change_move_position_to_route(Vector2f newPosition) { move_position_ = newPosition; }
	void take_damage(float damage, Vector2f attacker_pos = {-1, -1}) override;	
	void set_route(std::vector<std::pair<int, int>> route) { this->route = std::move(route); }
	void init_micro_blocks() override;
	virtual void change_action(actions new_action, bool reset_sprite_number = false, bool remember_last_action = false);
	virtual void handle_input(bool used_mouse = false);
	virtual void behavior_with_dynamic(dynamic_object* target, long long elapsed_time) = 0;
	virtual void behavior_with_static(world_object* target, long long elapsed_time) = 0;
	virtual void behavior(long long elapsedTime) = 0;
	virtual void set_target(dynamic_object &object) = 0;
	virtual void jerk(float power, float deceleration, Vector2f destination_point) = 0;
	
	long long time_for_new_hit_self = long(6e5);
	long long time_after_hit = 0;
	long long time_for_new_hit = long(100000);
	float sight_range = 0;
	float shake_speed = -1;
	auto is_intersect_dynamic(Vector2f new_position, dynamic_object& other_dynamic) const -> bool;
	
	Vector2f lax_move_position = { -1, -1 };
	std::string last_intersected = "";
	std::vector<std::pair<int, int>> route = { {} };

	std::string debug_info;
};

#endif