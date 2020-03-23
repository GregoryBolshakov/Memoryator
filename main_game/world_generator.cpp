#include "world_generator.h"

#include "dynamic_object.h"
#include "empty_object.h"

world_generator::world_generator()
= default;

void world_generator::init(
	const int width,
	const int height,
	const Vector2f block_size,
	const Vector2f micro_block_size,
	grid_list* static_grid,
	grid_list* dynamic_grid,
	scale_system* scale_system,
	std::map<pack_tag, sprite_pack>* packs_map)
{
	this->width_ = width;
	this->height_ = height;
	this->block_size_ = block_size;
	this->micro_block_size_ = micro_block_size;
	this->static_grid_ = static_grid;
	this->dynamic_grid_ = dynamic_grid;
	this->packs_map_ = packs_map;
	scale_system_ = scale_system;
	step_size_ = { block_size.x / 10, block_size.y / 10 };}

void world_generator::initialize_static_item(
	const entity_tag item_class,
	const Vector2f item_position,
	const int item_type,
	const std::string& item_name,
	const int count,
	const biomes biome,
	const bool mirrored,
	const std::vector<std::pair<entity_tag, int>>& inventory) const
{
	auto item = object_initializer::initialize_static_item(item_class, item_position, item_type, item_name, count, biome, packs_map_, mirrored, inventory);
	
	if (item == nullptr)
		return;

	// locked place check
	const auto terrain = dynamic_cast<terrain_object*>(item);
	if (terrain)
	{
		std::map<std::pair<int, int>, bool> checkBlocks = {};

		const auto end_i = int((item->get_position().x + item->get_micro_block_check_area_bounds().x) / micro_block_size_.x);
		const auto end_j = int((item->get_position().y + item->get_micro_block_check_area_bounds().y) / micro_block_size_.y);

		const auto width_to_micro_x = width_ / int(micro_block_size_.x);
		const auto height_to_micro_y = height_ / int(micro_block_size_.y);

		auto i = int((item->get_position().x - item->get_micro_block_check_area_bounds().x) / micro_block_size_.x);
		while (i <= end_i)
		{
			auto j = int((item->get_position().y - item->get_micro_block_check_area_bounds().y) / micro_block_size_.y);
			while (j <= end_j)
			{
				if (!(i < 0 || i >width_to_micro_x || j < 0 || j >height_to_micro_y) && !static_grid_->micro_block_matrix[int(round(i))][int(round(j))])
					checkBlocks[{i, j}] = true;
				j++;
			}
			i++;
		}

		if (item->is_locked_place(checkBlocks))
		{
			delete item;
			return;
		}
	}
	
	static_grid_->add_item(item, item->get_name(), item_position.x, item_position.y);
}

void world_generator::initialize_dynamic_item(const entity_tag item_class, const Vector2f item_position, const std::string& item_name, world_object* owner)
{
	const auto item = object_initializer::initialize_dynamic_item(item_class, item_position, item_name, packs_map_, owner);
	
	if (item == nullptr)
		return;

	if (item_class == entity_tag::hero)	
		focused_object = item;			

	dynamic_grid_->add_item(item, item->get_name(), item_position.x, item_position.y);
}

void world_generator::generate()
{	
	initialize_dynamic_item(entity_tag::hero, Vector2f(15800, 16300), "hero");
	initialize_dynamic_item(entity_tag::owl, Vector2f(15000, 16350), "owl");
	initialize_static_item(entity_tag::brazier, Vector2f(16300, 15800), 1, "brazier");

	// world generation
	init_biomes_generation_info();
	
	const auto max_scale = scale_system_->get_main_scale() * scale_system_->further_scale;
	
	const Vector2f upper_left(
		floor(focused_object->get_position().x - (helper::GetScreenSize().x / 2.0f + block_size_.x) / max_scale),
		floor(focused_object->get_position().y - (helper::GetScreenSize().y / 2.0f + block_size_.x) / max_scale));
	const Vector2f bottom_right(
		floor(focused_object->get_position().x + (helper::GetScreenSize().x / 2.0f + block_size_.y) / max_scale),
		floor(focused_object->get_position().y + (helper::GetScreenSize().y / 2.0f + block_size_.y) / max_scale));

	for (auto& block : static_grid_->get_blocks_in_sight(upper_left.x, upper_left.y, bottom_right.x, bottom_right.y))	
		in_block_generate(block);	
}

bool cmpByChance(const std::pair<entity_tag, int> a, const std::pair<entity_tag, int> b)
{
	return a.second < b.second;
}

void world_generator::in_block_generate(const int block_index)
{
	const auto block_type_probability = rand() % 100;
	const auto ground_ind_x = int(ceil(static_grid_->get_point_by_index(block_index).x / block_size_.x));
	const auto ground_ind_y = int(ceil(static_grid_->get_point_by_index(block_index).y / block_size_.y));

	std::vector<std::pair<entity_tag, int>> roomedBlocksContent = {};
	std::vector<std::pair<entity_tag, int>> otherBlocksContent = {};

	if (biome_matrix[ground_ind_x][ground_ind_y] == dark_woods)
	{
		if (block_type_probability <= 10) // block with roofs		
			roomedBlocksContent = { {entity_tag::roof, 10}, {entity_tag::rock, 2}, {entity_tag::stump, 2}, {entity_tag::tree, 7} };
		else
			if (block_type_probability <= 30) // block with yarrow
				roomedBlocksContent = { {entity_tag::yarrow, 0}, {entity_tag::rock, 2}, {entity_tag::stump, 2}, {entity_tag::tree, 7} };
			else
				if (block_type_probability <= 99) // common block				
					roomedBlocksContent = { {entity_tag::rock, 2}, {entity_tag::stump, 2}, {entity_tag::tree, 7} };
		otherBlocksContent = { {entity_tag::grass, 6} , {entity_tag::mushroom, 4} };
	}
	else
		if (biome_matrix[ground_ind_x][ground_ind_y] == birch_grove)
		{
			if (block_type_probability <= 30) // block with chamomile
				roomedBlocksContent = { {entity_tag::chamomile, 0}, {entity_tag::rock, 2}, {entity_tag::stump, 2}, {entity_tag::log, 2}, {entity_tag::bush, 5}, {entity_tag::tree, 7} };
			else
				if (block_type_probability <= 99) // common block
					roomedBlocksContent = { {entity_tag::rock, 5}, {entity_tag::stump, 2}, {entity_tag::log, 2}, {entity_tag::bush, 5}, {entity_tag::tree, 7} };
			otherBlocksContent = { {entity_tag::grass, 6} , {entity_tag::mushroom, 3} };
		}
		else
			if (biome_matrix[ground_ind_x][ground_ind_y] == swampy_trees)
			{
				if (block_type_probability <= 30) // block with chamomile
					roomedBlocksContent = { {entity_tag::rock, 2}, {entity_tag::lake, 2}, {entity_tag::stump, 2}, {entity_tag::root, 2}, {entity_tag::bush, 5}, {entity_tag::tree, 7} };
				else
					if (block_type_probability <= 99) // common block
						roomedBlocksContent = { {entity_tag::rock, 2}, {entity_tag::lake, 2}, {entity_tag::stump, 2}, {entity_tag::root, 2}, {entity_tag::bush, 5}, {entity_tag::tree, 7} };
				otherBlocksContent = { {entity_tag::grass, 6} , {entity_tag::mushroom, 2} };
			}

	std::sort(roomedBlocksContent.begin(), roomedBlocksContent.end(), cmpByChance);

	const auto blockTransform = IntRect(
		int(static_grid_->get_point_by_index(block_index).x),
		int(static_grid_->get_point_by_index(block_index).y),
		int(Vector2f(block_size_).x),
		int(Vector2f(block_size_).y));
	
	generate_ground(block_index);

	//block filling

	for (auto x = blockTransform.left; x < blockTransform.left + blockTransform.width; x += 100)
	{
		for (auto y = blockTransform.top; y < blockTransform.top + blockTransform.height; y += 100)
		{
			if (is_roomy_step_block(x, y))
			{
				for (auto& item : roomedBlocksContent)
				{
					const auto placedObjectDeterminant = rand() % 100;
					if (item.second > placedObjectDeterminant)
					{
						initialize_static_item(item.first, Vector2f(float(x), float(y)), -1, "", 1, biome_matrix[ground_ind_x][ground_ind_y]);
						break;
					}
				}
			}
			else
				for (auto& item : otherBlocksContent)
				{
					const auto placedObjectDeterminant = rand() % 100;
					if (item.second > placedObjectDeterminant)
					{
						initialize_static_item(item.first, Vector2f(float(x), float(y)), -1, "", 1, biome_matrix[ground_ind_x][ground_ind_y]);
						break;
					}
				}
		}
	}	
}

void world_generator::generate_ground(const int block_index)
{
	const auto position = static_grid_->get_point_by_index(block_index);
	const auto groundIndX = int(ceil(position.x / block_size_.x));
	const auto groundIndY = int(ceil(position.y / block_size_.y));
	const auto biome = biome_matrix[groundIndX][groundIndY];
	
	initialize_static_item(entity_tag::ground, position, int(biome), "", 1, biome);
	initialize_static_item(entity_tag::groundConnection, Vector2f(position), (biome - 1) * 4 + 1, "", 1);
	initialize_static_item(entity_tag::groundConnection, Vector2f(position.x, position.y + block_size_.y - 1), (biome - 1) * 4 + 2, "", 1);
	initialize_static_item(entity_tag::groundConnection, Vector2f(position), (biome - 1) * 4 + 3, "", 1);
	initialize_static_item(entity_tag::groundConnection, Vector2f(position.x + block_size_.x - 1, position.y), (biome - 1) * 4 + 4, "", 1);
}

bool world_generator::is_roomy_step_block(const int x, const int y) const
{
	return roomy_step_blocks_.count(std::make_pair(x % int(block_size_.x) / int(step_size_.x), y % int(block_size_.y) / int(step_size_.y))) > 0;
}

bool world_generator::is_trigger_block(const int x, const int y) const
{
	return abs(x - biomes_change_center_.x) >= 4 || abs(y - biomes_change_center_.y) >= 4 || abs(x - biomes_change_center_.x) + abs(y - biomes_change_center_.y) >= 5;
}

void world_generator::init_biomes_generation_info()
{
	biomes_blocks_offsets_.resize(8);
	for (auto x = -5; x <= 5; x++)
		for (auto y = -5; y <= 5; y++)		
			if (abs(x) + abs(y) >= 6)
			{
				if (x < 0 && y < 0)
					biomes_blocks_offsets_[0].emplace_back(x, y);
				if (x > 0 && y < 0)
					biomes_blocks_offsets_[2].emplace_back(x, y);
				if (x > 0 && y > 0)
					biomes_blocks_offsets_[4].emplace_back(x, y);
				if (x < 0 && y > 0)
					biomes_blocks_offsets_[6].emplace_back(x, y);
			}
			else
				if (abs(x) >= 3 || abs(y) >= 3)
				{
					if (y < 0 && abs(x) < abs(y))
						biomes_blocks_offsets_[1].emplace_back(x, y);
					if (x > 0 && abs(x) > abs(y))
						biomes_blocks_offsets_[3].emplace_back(x, y);
					if (y > 0 && abs(x) < abs(y))
						biomes_blocks_offsets_[5].emplace_back(x, y);
					if (x < 0 && abs(x) > abs(y))
						biomes_blocks_offsets_[7].emplace_back(x, y);
				}
	biomes_change_center_ = Vector2i(
		int(focused_object->get_position().x / block_size_.x),
		int(focused_object->get_position().y / block_size_.y));
	focused_object_block_ = biomes_change_center_;
	biomes_generate();
}

void world_generator::biomes_generate()
{
	biomes_change_center_ = focused_object_block_;

	for (auto& biome : biomes_blocks_offsets_)
	{
		const auto biomeId = rand() % 3 + 1;
		for (const auto offset : biome)
			biome_matrix[biomes_change_center_.x + offset.x][biomes_change_center_.y + offset.y] = biomes(biomeId);
	}
}

void world_generator::perimeter_generation()
{
	const auto screen_size = helper::GetScreenSize();
	const auto character_position = focused_object->get_position();
	
	const auto max_scale = scale_system_->get_main_scale() * scale_system_->further_scale;
	
	const Vector2f world_upper_left(
		ceil(character_position.x - (screen_size.x / 2 + block_size_.x) / max_scale),
		ceil(character_position.y - (screen_size.y / 2 + block_size_.y) / max_scale));
	const Vector2f world_bottom_right(
		ceil(character_position.x + (screen_size.x / 2 + block_size_.x) / max_scale),
		ceil(character_position.y + (screen_size.y / 2 + block_size_.y) / max_scale));

	if (focused_object->direction_system.direction != direction::STAND)
	{
		for (auto& block : static_grid_->get_blocks_around(world_upper_left.x, world_upper_left.y, world_bottom_right.x, world_bottom_right.y))
		{
			if (can_be_regenerated(block))
			{
				static_grid_->clear_cell(block);
				dynamic_grid_->clear_cell(block);
				in_block_generate(block);
			}
		}
	}
}

void world_generator::beyond_screen_generation()
{
	const auto block = Vector2i(
		int(focused_object->get_position().x / block_size_.x),
		int(focused_object->get_position().y / block_size_.y));
	
	if (focused_object_block_ != block)
	{		
		perimeter_generation();
		focused_object_block_ = block;
	}
	if (is_trigger_block(focused_object_block_.x, focused_object_block_.y))
		biomes_generate();
}

bool world_generator::can_be_regenerated(const int block_index) const
{
	if (remembered_blocks.count(block_index) > 0)
		return false;

	for (auto& item : dynamic_grid_->get_items(block_index))
	{
		const auto dynamicItem = dynamic_cast<dynamic_object*>(item);
		if (dynamicItem)
			return false;
	}

	return true;

	/*if (dynamicGrid->getItems(blockIndex).size() != 0)
		return false;

	return true;*/
}

