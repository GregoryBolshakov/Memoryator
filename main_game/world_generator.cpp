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

using coord_loop_t = long long;

world_generator::world_generator(
	const shared_ptr<grid_map>& grid_map
	, const shared_ptr<scale_system>& scale_system) :
	  grid_map_(grid_map)
	, scale_system_(scale_system)
{
}

std::weak_ptr<static_object> world_generator::initialize_static_item(
	const entity_tag item_class,
	const sf::Vector2f item_position,
	const int item_type,
	const std::string& item_name,
	const int count,
	const biome biome,
	const bool mirrored,
	const std::vector<std::pair<entity_tag, int>>& inventory)
{
	auto item = object_initializer::initialize_static_item(item_class, item_position, item_type, item_name, count, ::biome(biome), mirrored, inventory);
	assert(item);
	const auto name = item->get_name();
	all_static_objects_[name] = std::move(item);
	return all_static_objects_.at(name);
}

std::weak_ptr<dynamic_object> world_generator::initialize_dynamic_item(const entity_tag item_class, const sf::Vector2f item_position, const std::string& item_name, const shared_ptr<world_object>& owner)
{
	auto item = object_initializer::initialize_dynamic_item(item_class, item_position, item_name, owner);
	assert(item);
	const auto name = item->get_name();
	all_dynamic_objects_[name] = std::move(item);
	return all_dynamic_objects_.at(name);
}

void world_generator::clear_excess()
{
	for (auto it = all_static_objects_.begin(); it != all_static_objects_.cend();)
	{
		if (it->second->tag != entity_tag::ground)
		{
			++it;
			continue;
		}
		const sf::Vector2f upper_left(it->second->get_position().x, it->second->get_position().y);
		const sf::Vector2f bottom_right(
			it->second->get_position().x + world_metrics::block_size.x,
			it->second->get_position().y + world_metrics::block_size.y);
		const sf::Vector2f zone_upper_left(world_metrics::visible_zone.left, world_metrics::visible_zone.top);
		const sf::Vector2f zone_bottom_right(
			world_metrics::visible_zone.left + world_metrics::visible_zone.width,
			world_metrics::visible_zone.top + world_metrics::visible_zone.height);
		if (zone_upper_left.x - upper_left.x > world_metrics::block_size.x ||
			zone_upper_left.y - upper_left.y > world_metrics::block_size.y ||
			bottom_right.x - zone_bottom_right.x > world_metrics::block_size.x ||
			bottom_right.y - zone_bottom_right.y > world_metrics::block_size.y)
			it = all_static_objects_.erase(it);
		else
			++it;
	}
}

void world_generator::interact()
{
	std::set<direction> generation_directions;

	auto& zone = world_metrics::constant_zone;
	if (world_metrics::constant_zone.left + world_metrics::block_size.x > world_metrics::visible_zone.left)
		generation_directions.emplace(direction::LEFT);

	if (world_metrics::constant_zone.left + world_metrics::constant_zone.width - world_metrics::visible_zone.left - world_metrics::visible_zone.width < world_metrics::block_size.x)
		generation_directions.emplace(direction::RIGHT);

	if (world_metrics::constant_zone.top + world_metrics::block_size.y > world_metrics::visible_zone.top)
		generation_directions.emplace(direction::UP);

	if (world_metrics::constant_zone.top + world_metrics::constant_zone.height - world_metrics::visible_zone.top - world_metrics::visible_zone.height < world_metrics::block_size.y)
		generation_directions.emplace(direction::DOWN);

	if (!generation_directions.empty())
	{
		clear_excess();
		fill_outer_zone(generation_directions);
	}

	world_metrics::update_scale(world_metrics::scale);
}

void world_generator::primordial_generation()
{
	all_static_objects_.clear();
	all_dynamic_objects_.clear();

	player_ = shared_ptr<dynamic_object>(initialize_dynamic_item(entity_tag::hero, sf::Vector2f(0, 0), "hero"));
	assert(player_.lock());
	main_object_ = shared_ptr<static_object>(initialize_static_item(entity_tag::brazier, sf::Vector2f(1500, 700), 1, "brazier"));
	assert(main_object_.lock());
	initialize_static_item(entity_tag::tree, sf::Vector2f(1000, 400), 0, "tree");
}

void world_generator::fill_inner_zone()
{
	biome_matrix_.resize(world_metrics::matrix_size.x, std::vector<biome>(world_metrics::matrix_size.y));
	for (auto& column : biome_matrix_)
		for (auto& cell : column)
			cell = biome::dark_woods;

	auto& zone = world_metrics::constant_zone;
	for (auto x = zone.left + 1; x < zone.left + zone.width; x += world_metrics::block_size.x)
		for (auto y = zone.top + 1; y < zone.top + zone.height; y += world_metrics::block_size.y)
			in_block_generate(grid_map::get_block_by_point(sf::Vector2f(x, y)));
}

void world_generator::fill_outer_zone(std::set<direction> directions)
{
	auto& zone = world_metrics::constant_zone;
	if (directions.find(direction::LEFT) != directions.end())
		for (coord_loop_t y = zone.top; y < zone.top + zone.height; y += world_metrics::block_size.y)
			in_block_generate(grid_map::get_block_by_point(sf::Vector2f(zone.left, y)));

	if (directions.find(direction::RIGHT) != directions.end())
		for (coord_loop_t y = zone.top; y < zone.top + zone.height; y += world_metrics::block_size.y)
			in_block_generate(grid_map::get_block_by_point(sf::Vector2f(zone.left + zone.width, y)));
	
	if (directions.find(direction::UP) != directions.end())
		for (coord_loop_t x = zone.left; x < zone.left + zone.width; x += world_metrics::block_size.x)
			in_block_generate(grid_map::get_block_by_point(sf::Vector2f(x, zone.top)));

	if (directions.find(direction::DOWN) != directions.end())
		for (coord_loop_t x = zone.left; x < zone.left + zone.width; x += world_metrics::block_size.x)
			in_block_generate(grid_map::get_block_by_point(sf::Vector2f(x, zone.top + zone.height)));

	if (directions.find(direction::LEFT) != directions.end())
		world_metrics::constant_zone_no_scale.left -= world_metrics::block_size.x;

	if (directions.find(direction::RIGHT) != directions.end())
		world_metrics::constant_zone_no_scale.left += world_metrics::block_size.x;

	if (directions.find(direction::UP) != directions.end())
		world_metrics::constant_zone_no_scale.top -= world_metrics::block_size.y;

	if (directions.find(direction::DOWN) != directions.end())
		world_metrics::constant_zone_no_scale.top += world_metrics::block_size.y;

}

void world_generator::generate_ground(const sf::Vector2u index)
{
	const auto position = grid_map_.lock()->get_point_by_block(index);
	const auto biome = biome_matrix_[index.x][index.y];

	auto item1 = initialize_static_item(entity_tag::ground, position, int(biome), "", 1, biome).lock();
		for (auto item2 : all_static_objects_)
			if (item1 != item2.second)
			{
				if (item1->tag != entity_tag::ground || item2.second->tag != entity_tag::ground)
					continue;
				float tx = int(abs(item1->get_position().x - item2.second->get_position().x)) % int(world_metrics::block_size.x);
				float ty = int(abs(item1->get_position().y - item2.second->get_position().y)) % int(world_metrics::block_size.y);
				if (tx > 20 && tx < 182)
					auto test = 0;
				if (ty > 20 && ty < 93)
					auto test = 0;
			}
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
	return (*grid_map_.lock()->get_constant_blocks())[index.x][index.y];
}
