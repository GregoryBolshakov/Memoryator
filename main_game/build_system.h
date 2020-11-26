//#pragma once
//#ifndef BUILD_SYSTEM_H
//#define BUILD_SYSTEM_H
//
//#include <SFML/Graphics.hpp>
//
//#include "grid_map.h"
//#include "helper.h"
//#include "inventory_system.h"
//#include "sprite_pack.h"
//#include "world_object.h"
//
//using namespace sf;
//
//struct object_info {
//	std::string type;
//	std::string icon;
//	Sprite sprite;
//	Sprite icon_sprite;
//	Texture icon_texture;
//	std::vector<std::pair<entity_tag, int>> recipe;
//};
//
//class build_system
//{
//public:	
//	build_system();
//	~build_system();
//	void inventory_bounding(std::vector<hero_bag>* bound_bags);
//	void init();	
//	void on_mouse_up();
//	void build_held_item(sf::Vector2f focused_object_position, float scale_factor);
//	void interact(sf::Vector2f camera_position = {0, 0}, float scale_factor = 1);
//	void clear_hare_bags(int block, grid_map& static_grid, std::vector<world_object*>* visible_items);
//	void was_placed();
//	std::vector<sprite_chain_element*> prepare_sprites(grid_map& static_grid, const std::vector<world_object*>& visible_items, std::map<pack_tag, sprite_pack>* packs_map);
//
//	int get_current_object() const { return current_object_; }
//	bool get_used_mouse() const { return used_mouse_; }
//	bool get_success_init() const { return success_init_; }
//
//	void set_current_object(int n) { current_object_ = n; }
//	void set_held_item(std::pair<entity_tag, int>& held_item) { if (nullptr != &held_item) this->held_item_ = &held_item; }
//	
//	bool can_afford();
//
//	std::vector<hero_bag>* bound_bags;
//	bool can_be_placed = false;
//	bool instant_build = false;
//	entity_tag selected_object = entity_tag::emptyCell;
//	sf::Vector2f building_position = { -1, -1 };
//	std::map<entity_tag, bool> dropped_loot_id_list = { {entity_tag::heroBag, 1} };
//	int build_type = 1;
//private:
//	//std::unordered_map<Tag, CellSprite> craftIngredientsSpriteList;
//	std::pair<entity_tag, int> *held_item_ = nullptr;
//	sf::Vector2f mouse_world_pos_ = { 0, 0 };
//	sf::Vector2f sprite_build_pos_ = sf::Vector2f (-1, -1);
//	void initialize_objects_info();
//	void animator(long long elapsed_time);
//	float animation_speed_ = 0.001f;
//	bool used_mouse_ = false;
//	bool success_init_ = false;
//	std::vector<object_info> built_objects_;
//	int current_object_ = -1;
//	std::string build_system_objects_info_file_directory_ = "Game/buildSystem/buildSystemObjectsInfo.txt";
//	Font font_;
//};
//
//#endif