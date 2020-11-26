#pragma once

#include <map>
#include <memory>
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

class direction_system;
class static_object;
class grid_map;
enum class entity_tag;
enum class action;

struct bump_chain_element
{
	bump_chain_element(sf::Vector2f position, bool cancelable) : position{ position }, cancelable{ cancelable } {};
	sf::Vector2f position;
	bool cancelable = true;
};

class move_system
{
public:
	move_system(entity_tag& tag, float& radius, sf::Vector2f& position, sf::Color& color, action& current_action, direction_system& direction_system);
	virtual ~move_system();
	void push_away(long long elapsed_time, float push_speed = 0);
	void end_push();
	void push_by_bumping(sf::Vector2f position, float radius, bool can_crash);
	void set_move_offset(long long elapsedTime);
	[[nodiscard]] sf::Vector2f ellipse_slip(sf::Vector2f new_pos, sf::Vector2f destination, sf::Vector2f f1, sf::Vector2f f2, float ellipse_size, float height, long long elapsed_time) const;
	sf::Vector2f do_move(long long elapsedTime);
	[[nodiscard]] sf::Vector2f do_slip(
		sf::Vector2f new_position
		, const std::map<std::string, std::shared_ptr<static_object>>& local_static_items
		, float height
		, long long elapsed_time) const;
	void is_route_needed(std::vector<std::vector<bool>>& micro_block_matrix, sf::Vector2f& micro_block_size);
	void make_route(long long elapsed_time, grid_map& grid_map, float zone_offset);
	void pass_route_beginning(sf::Vector2f micro_block_size);
	void reset_timers();
	
	std::vector<std::pair<int, int>> route = { {} };
	bool need_route = false, route_generation_ability = true;
	bool can_crash_into_dynamic = true;
	bool can_crash_into_static = true;
	sf::Vector2f move_offset = { -1, -1 }, move_position = { -1, -1 }, lax_move_position = { -1, -1 };
	float default_speed = 0;
	float speed = 0;
	long long time_for_new_route = long(3e5), time_after_new_route = long(3e5);

	std::vector<bump_chain_element> bumped_positions = {};
	float bump_distance = 0;
	float push_damage = 0;
	float push_distance = 0;
	sf::Vector2f push_direction = { 0, 0 };
	sf::Vector2f push_vector = { 0, 0 };
	long long push_duration = 0;
	long long push_rest_duration = 0;
	long long red_duration = 0;
	long long red_rest_duration = 0;
	const long long default_push_duration = long(2e5);
	const float default_push_speed = 0.00025f;

private:
	// owner info
	entity_tag& tag;
	float& radius_;
	sf::Vector2f& position_;
	sf::Color& color_;
	action& current_action_;
	direction_system& direction_system_;
};
