#include "world_generator.h"
#include "static_object.h"
#include "dynamic_object.h"
#include "object_initializer.h"
#include "grid_map.h"
#include "scale_system.h"
#include "world_metrics.h"
#include "sprite_pack.h"

#include <cassert>
#include <utility>

world_generator::world_generator(
	  shared_ptr<grid_map> grid_map
	, const shared_ptr<scale_system>& scale_system
	, const shared_ptr<std::map<pack_tag, sprite_pack>>& packs_map) :
	  grid_map_(grid_map)
	, scale_system_(scale_system)
	, packs_map_(packs_map)
	, biome_matrix(world_metrics::matrix_size.x, std::vector<biome>(world_metrics::matrix_size.y))
{
}

const shared_ptr<static_object>& world_generator::initialize_static_item(
	const entity_tag item_class,
	const sf::Vector2f item_position,
	const int item_type,
	const std::string& item_name,
	const int count,
	const biome biome,
	const bool mirrored,
	const std::vector<std::pair<entity_tag, int>>& inventory)
{
	auto item = object_initializer::initialize_static_item(item_class, item_position, item_type, item_name, count, ::biome(biome), packs_map_, mirrored, inventory);
	assert(item);
	const auto name = item->get_name();
	all_static_objects_[name] = std::move(item);
	return all_static_objects_.at(name);
}

const shared_ptr<dynamic_object>& world_generator::initialize_dynamic_item(const entity_tag item_class, const sf::Vector2f item_position, const std::string& item_name, const shared_ptr<world_object>& owner)
{
	auto item = object_initializer::initialize_dynamic_item(item_class, item_position, item_name, packs_map_, owner);
	assert(item);
	const auto name = item->get_name();
	all_dynamic_objects_[name] = std::move(item);
	return all_dynamic_objects_.at(name);
}

void world_generator::primordial_generation()
{
	for (auto& column : biome_matrix)
		for (auto& cell : column)
			cell = biome::swampy_trees;
	
	all_dynamic_objects_["hero"] = shared_ptr<dynamic_object>(initialize_dynamic_item(entity_tag::hero, world_metrics::center, "hero"));
	player_ = all_dynamic_objects_["hero"];
	assert(player_.lock());
	all_static_objects_["brazier"] = shared_ptr<static_object>(initialize_static_item(entity_tag::brazier, world_metrics::center + sf::Vector2f(200, 200), 1, "brazier"));
	main_object_ = all_static_objects_["brazier"];
	assert(main_object_.lock());
	grid_map_->add_constant_block(grid_map_->get_block_by_point(main_object_.lock()->get_position()));
	initialize_static_item(entity_tag::tree, world_metrics::center - sf::Vector2f(200, 200), -1, "tree");
}

void world_generator::fill_zone()
{
	const auto max_scale = scale_system_.lock()->calculate_scale() * scale_system_.lock()->further_scale;

	auto& zone = world_metrics::constant_zone;
	for (auto x = zone.left; x <= zone.left + zone.width; x += world_metrics::block_size.x)
		for (auto y = zone.left; y <= zone.left + zone.width; y += world_metrics::block_size.y)
			in_block_generate(grid_map::get_block_by_point(sf::Vector2f(x, y)));
}

void world_generator::generate_ground(const sf::Vector2u index)
{
	const auto position = grid_map_->get_point_by_block(index);
	const auto biome = biome_matrix[index.x][index.y];

	initialize_static_item(entity_tag::ground, position, int(biome), "", 1, biome);
	/*initialize_static_item(entity_tag::groundConnection, position, (biome - 1) * 4 + 1, "", 1, biome);
	initialize_static_item(entity_tag::groundConnection, position.x, position.y + world_metrics::block_size.y - 1), (biome - 1) * 4 + 2, "", 1, biome);
	initialize_static_item(entity_tag::groundConnection, position, (biome - 1) * 4 + 3, "", 1, biome);
	initialize_static_item(entity_tag::groundConnection, sf::Vector2f(position.x + world_metrics::block_size.x - 1, position.y), (biome - 1) * 4 + 4, "", 1, biome);*/
}

void world_generator::in_block_generate(const sf::Vector2u index)
{
	generate_ground(index);
	//const auto block_type_probability = rand() % 100;
	//const auto ground_ind_x = int(ceil(static_grid_->get_point_by_index(block_index).x / block_size_.x));
	//const auto ground_ind_y = int(ceil(static_grid_->get_point_by_index(block_index).y / block_size_.y));

	//std::vector<std::pair<entity_tag, int>> roomedBlocksContent = {};
	//std::vector<std::pair<entity_tag, int>> otherBlocksContent = {};

	//if (biome_matrix[ground_ind_x][ground_ind_y] == dark_woods)
	//{
	//	if (block_type_probability <= 10) // block with roofs		
	//		roomedBlocksContent = { {entity_tag::roof, 10}, {entity_tag::rock, 2}, {entity_tag::stump, 2}, {entity_tag::tree, 7} };
	//	else
	//		if (block_type_probability <= 30) // block with yarrow
	//			roomedBlocksContent = { {entity_tag::yarrow, 0}, {entity_tag::rock, 2}, {entity_tag::stump, 2}, {entity_tag::tree, 7} };
	//		else
	//			if (block_type_probability <= 99) // common block				
	//				roomedBlocksContent = { {entity_tag::rock, 2}, {entity_tag::stump, 2}, {entity_tag::tree, 7} };
	//	otherBlocksContent = { {entity_tag::grass, 6} , {entity_tag::mushroom, 4} };
	//}
	//else
	//	if (biome_matrix[ground_ind_x][ground_ind_y] == birch_grove)
	//	{
	//		if (block_type_probability <= 30) // block with chamomile
	//			roomedBlocksContent = { {entity_tag::chamomile, 0}, {entity_tag::rock, 2}, {entity_tag::stump, 2}, {entity_tag::log, 2}, {entity_tag::bush, 5}, {entity_tag::tree, 7} };
	//		else
	//			if (block_type_probability <= 99) // common block
	//				roomedBlocksContent = { {entity_tag::rock, 5}, {entity_tag::stump, 2}, {entity_tag::log, 2}, {entity_tag::bush, 5}, {entity_tag::tree, 7} };
	//		otherBlocksContent = { {entity_tag::grass, 6} , {entity_tag::mushroom, 3} };
	//	}
	//	else
	//		if (biome_matrix[ground_ind_x][ground_ind_y] == swampy_trees)
	//		{
	//			if (block_type_probability <= 30) // block with chamomile
	//				roomedBlocksContent = { {entity_tag::rock, 2}, {entity_tag::lake, 2}, {entity_tag::stump, 2}, {entity_tag::root, 2}, {entity_tag::bush, 5}, {entity_tag::tree, 7} };
	//			else
	//				if (block_type_probability <= 99) // common block
	//					roomedBlocksContent = { {entity_tag::rock, 2}, {entity_tag::lake, 2}, {entity_tag::stump, 2}, {entity_tag::root, 2}, {entity_tag::bush, 5}, {entity_tag::tree, 7} };
	//			otherBlocksContent = { {entity_tag::grass, 6} , {entity_tag::mushroom, 2} };
	//		}

	//std::sort(roomedBlocksContent.begin(), roomedBlocksContent.end(), cmpByChance);

	//const auto blockTransform = sf::IntRect(
	//	int(static_grid_->get_point_by_index(block_index).x),
	//	int(static_grid_->get_point_by_index(block_index).y),
	//	int(sf::Vector2f(block_size_).x),
	//	int(sf::Vector2f(block_size_).y));
	//
	//generate_ground(block_index);

	////block filling

	//for (auto x = blockTransform.left; x < blockTransform.left + blockTransform.width; x += 100)
	//{
	//	for (auto y = blockTransform.top; y < blockTransform.top + blockTransform.height; y += 100)
	//	{
	//		if (is_roomy_step_block(x, y))
	//		{
	//			for (auto& item : roomedBlocksContent)
	//			{
	//				const auto placedObjectDeterminant = rand() % 100;
	//				if (item.second > placedObjectDeterminant)
	//				{
	//					initialize_static_item(item.first, sf::Vector2f(float(x), float(y)), -1, "", 1, biome_matrix[ground_ind_x][ground_ind_y]);
	//					break;
	//				}
	//			}
	//		}
	//		else
	//			for (auto& item : otherBlocksContent)
	//			{
	//				const auto placedObjectDeterminant = rand() % 100;
	//				if (item.second > placedObjectDeterminant)
	//				{
	//					initialize_static_item(item.first, sf::Vector2f(float(x), float(y)), -1, "", 1, biome_matrix[ground_ind_x][ground_ind_y]);
	//					break;
	//				}
	//			}
	//	}
	//}	
}

bool world_generator::whether_block_regeneretable(const sf::Vector2u index) const
{
	return (*grid_map_->get_constant_blocks())[index.x][index.y];
}
