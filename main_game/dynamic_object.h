#pragma once
#ifndef DYNAMIC_OBJECT_H
#define DYNAMIC_OBJECT_H

#include "move_system.h"
#include "world_object.h"

class dynamic_object : public world_object
{
public:
	dynamic_object(std::string objectName, Vector2f centerPosition);
	virtual ~dynamic_object();
	int get_sprite_number() override { return current_sprite_[0]; }
	[[nodiscard]] int get_ellipse_size() const { return ellipse_size_; }
	[[nodiscard]] float get_strength() const { return strength_; }
	[[nodiscard]] long long get_time_after_hit_self() const { return time_after_hitself_; }
	[[nodiscard]] Vector2f get_focus1() const { return focus1_; }
	[[nodiscard]] Vector2f get_focus2() const { return focus2_; }
	[[nodiscard]] actions get_current_action() const { return current_action_; }
	[[nodiscard]] world_object *get_bound_target() const { return bound_target_; }

	void set_current_action(actions action) { this->current_action_ = action; }	
	void take_damage(float damage, Vector2f attacker_pos = {-1, -1}) override;
	void set_route(std::vector<std::pair<int, int>> route) { move_system.route = std::move(route); }
	void init_micro_blocks() override;
	virtual void change_action(actions new_action, bool reset_sprite_number = false, bool remember_last_action = false);
	virtual void handle_input(bool used_mouse = false, long long elapsed_time = 0);
	virtual void behavior_with_dynamic(dynamic_object* target, long long elapsed_time) = 0;
	virtual void behavior_with_static(world_object* target, long long elapsed_time) = 0;
	virtual void behavior(long long elapsedTime) = 0;
	virtual void set_target(dynamic_object &object) = 0;
	virtual void jerk(float power, float deceleration, Vector2f destination_point) = 0;
	virtual bool die_check();

	move_system move_system;
	direction_system direction_system;
	
	long long time_for_new_hit_self = long(6e5);
	long long time_after_hit = 0;
	long long time_for_new_hit = long(100000);
	float sight_range = 0;
	float shake_speed = -1;
	auto is_intersect_dynamic(Vector2f new_position, dynamic_object& other_dynamic) const -> bool;
		
	std::string last_intersected = "";

	std::string debug_info;
protected:	
	int ellipse_size_{};
	Vector2f focus1_, focus2_;
	float strength_ = 0;
	long long time_after_hitself_ = 0;

	actions current_action_ = relax, last_action_ = relax;
	world_object* bound_target_ = nullptr;

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
};

#endif