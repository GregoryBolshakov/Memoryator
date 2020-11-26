//#pragma once
//
//#include "helper.h"
//#include "neutral_mob.h"
//
//using namespace sf;
//
//class hare : public neutral_mob
//{
//public:
//	hare(const std::string& objectName, sf::Vector2f centerPosition);
//	~hare();
//	sf::Vector2f calculate_texture_offset() override;
//	std::vector<sprite_chain_element*> prepare_sprites(long long elapsedTime) override;
//	void behavior_with_static(world_object* target, long long elapsedTime) override;
//	void behavior_with_dynamic(dynamic_object* target, long long elapsedTime) override;
//	void behavior(long long elapsed_time) override;
//	bool bounce_to_trap(long long elapsed_time);
//	bool run_away_from_enemy(long long elapsed_time);
//	void calm_behavior(long long elapsed_time);
//	void ending_previous_action();
//	sf::Vector2f get_build_position(std::vector<world_object*> visible_items, float scale_factor, sf::Vector2f camera_position) override;
//	int get_build_type(sf::Vector2f oun_pos, sf::Vector2f other_pos) override;
//	void jerk(float power, float deceleration, sf::Vector2f destination_point) override;
//
//private:
//	bool calm_state_ = true, stand_ = true;
//	std::vector<action> calm_actions_stack = {};
//	long long time_for_new_calm_state_ = long(15e5), time_after_calm_state_ = long(15e5);
//	long long time_after_fear_ = 0;
//	long long fear_time_ = long(1e6);
//	sf::Vector2f cheat_hero_pos_ = { 0, 0 };
//};
