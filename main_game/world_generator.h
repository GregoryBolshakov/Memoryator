#pragma once
#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include "object_initializer.h"
#include "grid_list.h"

class world_generator
{
public:	
	const float CLOSEST_SCALE = 1.7f;
	const float FARTHEST_SCALE = 0.998f;
	
	void generate();
	void init(int width, int height, Vector2f block_size, Vector2f micro_block_size, grid_list* static_grid, grid_list* dynamic_grid, std::map<pack_tag, sprite_pack>* packs_map);
    void init_main_scale();

	auto initialize_static_item(
		entity_tag item_class,
		Vector2f item_position,
		int item_type,
		const std::string& item_name,
		int count = 1,
		biomes biome = dark_woods,
		bool mirrored = true,
		const std::vector<std::pair<entity_tag, int>>& inventory = {}) const -> void;
	
	void initialize_dynamic_item(entity_tag item_class, Vector2f item_position, const std::string& item_name, world_object* owner = nullptr);

	//active generation
	void in_block_generate(int block_index);
	[[nodiscard]] bool can_be_regenerated(int block_index) const;
	void biomes_generate();
	void perimeter_generation();
	void beyond_screen_generation();

	world_generator();
	~world_generator();

	biomes biome_matrix[100][100]{};
	std::map<int, bool> remembered_blocks = { {0, true} };
	dynamic_object* focused_object = nullptr;
	float scale_factor = 1;
	float main_scale = 1;

	float get_scale_delta_normalized() const;
	
private:
	grid_list* static_grid_ = nullptr;
	grid_list* dynamic_grid_ = nullptr;	
	std::map<pack_tag, sprite_pack>* packs_map_{};
	int width_ = 0;
	int height_ = 0;
	Vector2f block_size_ = { 0, 0 };
	Vector2f micro_block_size_ = { 0, 0 };
	Vector2i focused_object_block_ = { 0, 0 };

	// block generation
	void generate_ground(int block_index);
	[[nodiscard]] bool is_roomy_step_block(int x, int y) const;
	std::map<std::pair<int, int>, bool> roomy_step_blocks_ = {
		{std::make_pair(1, 1), true}, {std::make_pair(3, 1), true}, { std::make_pair(5, 1), true}, { std::make_pair(7, 1), true},
		{std::make_pair(2, 3), true}, { std::make_pair(4, 3), true}, { std::make_pair(6, 3), true}, { std::make_pair(8, 3), true},
		{ std::make_pair(1, 5), true}, { std::make_pair(3, 5), true}, { std::make_pair(5, 5), true}, { std::make_pair(7, 5), true},
		{ std::make_pair(2, 7), true}, { std::make_pair(4, 7), true}, { std::make_pair(6, 7), true}, { std::make_pair(8, 7), true} };
	Vector2f step_size_ = { 0, 0 };

	// biomes generation
	void init_biomes_generation_info();
	[[nodiscard]] bool is_trigger_block(int x, int y) const;
	Vector2i biomes_change_center_ = { 0, 0 };
	std::vector<std::vector<Vector2i>> biomes_blocks_offsets_ = {};
};

#endif