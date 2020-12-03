#pragma once

#include "move_system.h"
#include "direction_system.h"
#include "world_object.h"

enum class action;

enum class action {
	direct_hit = 2,
	relax = 3,
	combat_state = 4,
	move = 5,
	dead = 6,
	move_slowly = 7,
	common_hit = 9,
	move_hit = 10,
	dropping = 11,
	open = 13,
	absorbs = 14,
	grab = 15,
	builds = 16,
	jerking = 17,
	move_end = 18,
	listening = 19,
	sniff = 20,
	transition = 21,
	trap = 22,
	jump = 23,
	jump_end = 24,
	look_around = 25,
	startle = 26,
	flap = 27,
	head = 28,
	landing = 29,
	soar = 30,
	takeoff = 31,
	throw_noose = 35
};

static const float creature_radius = 50;
static const float creature_hit_distance = 50;
static const float creature_strength = 10;
static const float creature_max_health = 100;

static const long long creature_time_for_new_hit = 100000;
static const long long creature_time_for_new_hitself = 600000;

class dynamic_object : public world_object
{
public:
	dynamic_object(std::string name, sf::Vector2f position, int kind);
	virtual ~dynamic_object();
	[[nodiscard]] int get_ellipse_size() const { return ellipse_size_; }
	[[nodiscard]] float get_strength() const { return strength_; }
	[[nodiscard]] long long get_time_after_hit_self() const { return time_after_hitself_; }
	[[nodiscard]] sf::Vector2f get_focus1() const { return focus1_; }
	[[nodiscard]] sf::Vector2f get_focus2() const { return focus2_; }
	[[nodiscard]] action get_current_action() const { return current_action_; }
	[[nodiscard]] world_object *get_bound_target() const { return bound_target_; }

	void set_current_action(action action) { this->current_action_ = action; }	
	void take_damage(float damage, sf::Vector2f attacker_pos = {-1, -1}) override;
	void set_route(std::vector<std::pair<int, int>> route) { move_system.route = std::move(route); }
	void init_route_blocks() override;
	virtual void change_action(action new_action, bool reset_sprite_number = false, bool remember_last_action = false);
	virtual void handle_input(bool used_mouse = false, long long elapsed_time = 0);
	virtual void behavior_with_dynamic(shared_ptr<dynamic_object> target, long long elapsed_time) = 0;
	virtual void behavior_with_static(shared_ptr<world_object> target, long long elapsed_time) = 0;
	virtual void behavior(long long elapsed_time) = 0;
	virtual void set_target(dynamic_object &object) = 0;
	virtual void jerk(float power, float deceleration, sf::Vector2f destination_point) = 0;
	virtual bool die_check();

	move_system move_system;
	direction_system direction_system;
	
	long long time_for_new_hit_self;
	long long time_after_hit = 0;
	long long time_for_new_hit;
	float sight_range = 0;
	float shake_speed = -1;
	auto is_intersect_dynamic(sf::Vector2f new_position, dynamic_object& other_dynamic) const -> bool;
		
	std::string last_intersected = "";

	std::string debug_info;
protected:
	int ellipse_size_{};
	sf::Vector2f focus1_, focus2_;
	float strength_ = 0;
	long long time_after_hitself_ = 0;

	action current_action_, last_action_;
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
