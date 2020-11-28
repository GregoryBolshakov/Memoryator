#pragma once

#include <SFML/Graphics.hpp>
#include <stack>

class sprite_chain_element;
enum class entity_tag;

// mobs 101 - 199; craft objects 201 - 299; world objects 301 - 399; flowers 401 - 499; auxiliary objects 501 - 599;
enum class entity_tag {
	hero = 101, hare = 102, owl = 103, deer = 104, fox = 105, bear = 106, wolf = 107, monster = 108, owl_boss = 109, nightmare1 = 110, nightmare2 = 111, nightmare3 = 112,
	hero_bag = 201, noose = 202, totem = 211, hare_trap = 216, cage_bear = 218, fence = 219, inky_black_pen = 220,
	unknown_wreath = 251, hare_wreath = 252, owl_wreath = 253,
	tree = 301, grass = 302, spawn = 303, ground = 311, ground_connection = 312, brazier = 314, wreath_table = 315, rock = 317,
	stump = 319, dropped_loot = 320, mushroom = 321, log = 322, bush = 323, roof = 324, lake = 325, root = 326,
	chamomile = 401, yarrow = 402, fern = 403, mugwort = 404, poppy = 405,
	build_object = 501, drop_point = 502, empty_draft = 503, empty_page = 504, empty_cell = 505, selected_cell = 506, clap_whirl = 507, empty_object = 508
};

class world_object;
using std::unique_ptr, std::make_unique;
using std::shared_ptr, std::make_shared, std::dynamic_pointer_cast;

enum state { common = 1, absorbed = 2, constructed = 3 };

struct birth_static_info
{
	entity_tag tag;
	sf::Vector2f position;
	int type_of_object = 1;
	int count = 1;
	std::vector<std::pair<entity_tag, int>> inventory;
};

struct  birth_dynamic_info
{
	entity_tag tag;
	sf::Vector2f position;
	shared_ptr<world_object> owner;
};

class world_object
{
public:
	world_object(std::string object_name, sf::Vector2f center_position);
	virtual ~world_object();

	[[nodiscard]] int get_z_coords() const	{ return z_coordinate_; }
	[[nodiscard]] int get_type() const	{ return type_of_object_; }
	[[nodiscard]] int get_variety_of_types() const	{ return variety_of_types_; }
	[[nodiscard]] float get_health_point() const	{ return health_point_; }
	[[nodiscard]] float get_max_health_point_value() const;
	[[nodiscard]] bool get_delete_promise() const { return delete_promise_; }
	[[nodiscard]] bool get_mirrored_state() const { return mirrored_; }
	[[nodiscard]] float get_radius() const { return radius_; }
	[[nodiscard]] std::string get_name() const { return name_; }
	virtual std::vector<unique_ptr<sprite_chain_element>> prepare_sprites(long long elapsed_time) = 0;
	virtual void on_sprite_change();
	virtual int get_sprite_number() = 0;
	[[nodiscard]] sf::Vector2f get_position() const { return position_; }
	sf::Vector2f *get_ptr_position() { return &position_; }
	[[nodiscard]] sf::Vector2f get_texture_size() const { return { texture_box_.width, texture_box_.height }; }
	[[nodiscard]] sf::Vector2f get_texture_offset() const { return { texture_box_offset_.x, texture_box_offset_.y }; }
	[[nodiscard]] sf::Vector2f get_scale_ratio() const;
	[[nodiscard]] sf::Vector2f get_conditional_size_units() const { return conditional_size_units_; }
	[[nodiscard]] std::vector<sf::Vector2i> get_locked_micro_blocks() const { return locked_route_blocks_; }
	virtual sf::Vector2f get_build_position(std::vector<world_object*> visible_items, float scale_factor, sf::Vector2f camera_position) { return { -1 ,-1 }; };
	virtual int get_build_type(sf::Vector2f own_pos, sf::Vector2f other_pos) { return 1; };
	[[nodiscard]] sf::FloatRect get_original_texture_box() const { return original_texture_box_; }
	[[nodiscard]] state get_state() const { return state_; }	
	std::pair<std::stack<birth_static_info>, std::stack<birth_dynamic_info>> get_birth_objects() { return std::make_pair(birth_statics_, birth_dynamics_); }	

	void manually_disable_mirroring() { mirrored_ = false; }
	void cancel_mirroring();
	void clear_birth_stack() { birth_statics_ = std::stack<birth_static_info>(); birth_dynamics_ = std::stack<birth_dynamic_info>(); }
	virtual void set_position(sf::Vector2f new_position) = 0;
	void set_health_point(const float health_point) { this->health_point_ = health_point; }
	void set_name(std::string name) { this->name_ = std::move(name); }
	void delete_promise_on() { delete_promise_ = true; }
	static void set_unscaled(const std::vector<unique_ptr<sprite_chain_element>>& items);
	virtual void set_texture_size(sf::Vector2f texture_size);
	void set_state(state state) { this->state_ = state; }
	virtual void take_damage(float damage, sf::Vector2f attacker_pos = { -1, -1 });
	virtual void init_route_blocks();
	virtual bool is_locked_place(const std::map<std::pair<int, int>, bool>& check_blocks);

	bool is_transparent = false;
	bool is_selected = false;
	bool is_visible_inventory = false;
	bool is_processed = false;
	bool is_background = false;
	bool is_terrain = false;
	bool is_dots_adjusted = false;
	bool is_multi_ellipse = false;
	bool intangible = false;

	virtual sf::Vector2f calculate_texture_offset() = 0;
	virtual void init_pedestal();

	sf::Color color = sf::Color(255, 255, 255);
	std::vector<std::pair<entity_tag, int>> inventory;
	static sf::Vector2f micro_block_size;
	entity_tag tag;
protected:
	int type_of_object_ = 1;
	int z_coordinate_ = 1;
	std::vector<int> current_sprite_ = { 1 };
	int variety_of_types_ = 0;
	bool delete_promise_ = false;
	bool mirrored_ = false;
	long long time_for_new_sprite_ = 0;
	float animation_speed_ = 0;
	float health_point_ = 0;
	float armor_ = 1;
	float max_health_point_value_ = 0;
	std::string name_;
	sf::FloatRect texture_box_;
	sf::FloatRect original_texture_box_;
	sf::Vector2f texture_box_offset_;
	sf::Vector2f conditional_size_units_;
	sf::Vector2f position_;
	float radius_ = 0;
	state state_ = common;
	std::stack<birth_static_info> birth_statics_;
	std::stack<birth_dynamic_info> birth_dynamics_;
	std::vector<sf::Vector2i> locked_route_blocks_;
};
