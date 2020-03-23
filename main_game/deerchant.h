#pragma once

#include "dynamic_object.h"
#include "hero_bag.h"

using namespace sf;

class deerchant : public dynamic_object
{
public:	
	deerchant(std::string object_name, Vector2f center_position);
	~deerchant();
	//BuildSystem
	bool is_build_system = false;
	//draw
	std::string name_of_file;
	Vector2f calculate_texture_offset() override;
	std::vector<sprite_chain_element*> prepare_sprites(long long elapsed_time) override;
	//control
	void handle_input(bool used_mouse = false, long long elapsed_time = 0) override;
	void move_end(bool animate = false, bool invert_direction = false);
	[[nodiscard]] int calculate_next_move_end_sprite(int current_sprite = 1) const;
	void behavior_with_dynamic(dynamic_object* target, long long elapsed_time) override;
	void behavior_with_static(world_object* target, long long elapsed_time) override;
	void behavior(long long elapsed_time) override;
	void on_mouse_up(int current_mouse_button, world_object *mouse_selected_object, Vector2f mouse_world_pos, bool is_building = false);
	void set_target(dynamic_object& object) override;
	void ending_previous_action();
	void stopping(bool do_stand = false, bool forget_bound_target = false, bool off_unseal_inventory = false);
	void calculate_speed_line_direction(direction last_direction = direction::STAND, direction direction = direction::STAND);
	void change_action(actions new_action, bool reset_sprite_number, bool remember_last_action) override;	
	[[nodiscard]] Vector2f get_belt_position() const;

	void jerk(float power, float deceleration, Vector2f destination_point = Vector2f(-1, -1)) override;
	void jerk_interact(long long elapsed_time);
	void speed_interact(long long elapsed_time);
	void fight_interact(long long elapsed_time, dynamic_object* target = nullptr) override;
	void animation_smooth_interact(long long elapsed_time);
	void animation_smooth();

	cell* held_item = nullptr;
	Vector2f get_build_position(std::vector<world_object*> visible_items, float scale_factor, Vector2f camera_position) override;
	int get_build_type(Vector2f oun_pos, Vector2f other_pos) override;
	std::vector<hero_bag> bags;
	bool near_the_table = false;
private:
	void set_hit_direction();
	sprite_chain_element* prepare_speed_line();

	float hit_distance_ = 0;
	float move_end_distance_ = 0;
	long long move_time_ = 0;
	long long smooth_move_time_ = 0;
	direction smooth_direction_ = direction::STAND;
	world_object* unseal_inventory_owner_ = nullptr;
	int striking_sprite_ = 0;
	Vector2f last_position_ = { 0, 0 };
	direction speed_line_direction_ = direction::STAND;
	bool was_pushed_after_movement_ = false;
	bool mirrored_speed_line_ = false;
	bool reverse_speed_line_ = false;
	std::vector<direction> smooth_directions_ = {};
};
