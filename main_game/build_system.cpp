#include "build_system.h"

#include <fstream>

#include "dropped_loot.h"
#include "object_initializer.h"

build_system::build_system()
= default;

build_system::~build_system()
= default;

void build_system::init()
{
	initialize_objects_info();

	font_.loadFromFile("Fonts/Bebas.ttf");
	success_init_ = true;
}

void build_system::inventory_bounding(std::vector<hero_bag>* bound_bags)
{
	this->bound_bags = bound_bags;
}

void build_system::initialize_objects_info()
{
	std::string objectIconPath, objectImageType;
	std::ifstream fin(build_system_objects_info_file_directory_);
	while (fin >> objectIconPath >> objectImageType)
	{
		object_info infoItem;

		infoItem.icon = objectIconPath;
		infoItem.type = objectImageType;

		int recipeItemId, recipeItemAmount, recipeLength;

		fin >> recipeLength;
		for (int i = 0; i < recipeLength; i++)
		{
			fin >> recipeItemId >> recipeItemAmount;
			infoItem.recipe.push_back(std::make_pair(entity_tag(recipeItemId), recipeItemAmount));
		}

		built_objects_.push_back(infoItem);
	}

	fin.close();
}

std::vector <sprite_chain_element*> build_system::prepare_sprites(grid_list& static_grid, const std::vector<world_object*>& visible_items, std::map<pack_tag, sprite_pack>* packs_map)
{
	if (selected_object == entity_tag::emptyCell)
		return {};

	static_object* terrain = nullptr;
	if (dropped_loot_id_list.count(selected_object) > 0)
		terrain = object_initializer::initialize_static_item(entity_tag::droppedLoot, mouse_world_pos_, int(selected_object), "", 1, dark_woods, packs_map);
	else
	{
		if (selected_object == entity_tag::totem)
		{
			for (auto item : visible_items)
			{
				if (static_grid.get_index_by_point(item->get_position().x, item->get_position().y) != static_grid.get_index_by_point(mouse_world_pos_.x, mouse_world_pos_.y))
					continue;
				bool match = false;
				auto droppedLoot = dynamic_cast<dropped_loot*>(item);
				if (droppedLoot && droppedLoot->get_type() == 201)
				{
					for (auto& cell : droppedLoot->inventory)
						if (cell.first == entity_tag::hare)
						{
							match = true;
							break;
						}
				}
				if (match)
				{
					build_type = 2;
					break;
				}
			}
		}
		terrain = object_initializer::initialize_static_item(selected_object, mouse_world_pos_, build_type, "", 1, dark_woods, packs_map, false);
	}

	auto sprites = terrain->prepare_sprites(0);
	can_be_placed = true;

	if (static_grid.is_intersect_with_others(terrain))
	{
		for (auto& sprite : sprites)
			sprite->color = sf::Color(255,99,71);
		can_be_placed = false;
	}
	else
		for (auto& sprite : sprites)
			sprite->color = sf::Color(127, 255, 0);

	return sprites;
}

void build_system::interact(Vector2f camera_position, float scale_factor)
{
	const Vector2f mousePos = Vector2f(Mouse::getPosition());
	mouse_world_pos_ = Vector2f((mousePos.x - helper::GetScreenSize().x / 2 + camera_position.x * scale_factor) / scale_factor,
		(mousePos.y - helper::GetScreenSize().y / 2 + camera_position.y * scale_factor) / scale_factor);
}

void build_system::on_mouse_up()
{
	used_mouse_ = false;

	if (selected_object != entity_tag::emptyCell || current_object_ != -1)
		used_mouse_ = true;

	if (selected_object != entity_tag::emptyCell && current_object_ == -1 && can_be_placed)
	{
		if (sprite_build_pos_ != Vector2f (-1, -1))
			building_position = sprite_build_pos_;
		else
			building_position = mouse_world_pos_;
	}
	else
		building_position = Vector2f (-1, -1);
}

void build_system::build_held_item(Vector2f focused_object_position, float scale_factor)
{
	if (held_item_->first == entity_tag::emptyCell)
	{
		building_position = Vector2f (-1, -1);
		return;
	}

	if (can_be_placed)
	{
		building_position = Vector2f ((Mouse::getPosition().x - helper::GetScreenSize().x / 2 + focused_object_position.x * scale_factor) / scale_factor,
			(Mouse::getPosition().y - helper::GetScreenSize().y / 2 + focused_object_position.y*scale_factor) / scale_factor);
	}
}

bool build_system::can_afford()
{
	if (current_object_ != -1)
	{
		std::vector<std::pair <entity_tag, int>> temporaryInventory = built_objects_[current_object_].recipe;

		for (auto&curRecipeItem = temporaryInventory.begin(); curRecipeItem != temporaryInventory.end(); ++curRecipeItem)
		{
			for (auto bag = bound_bags->begin(); bag != bound_bags->end(); bag++)
			{
				bool isBreak = false;
				for (auto&item : bag->cells)
				{
					if (curRecipeItem->first == item.content.first)
					{
						if (item.content.second >= curRecipeItem->second)
						{
							curRecipeItem->second = 0;
							isBreak = true;
							break;
						}
					}
					else
						curRecipeItem->second -= item.content.second;
				}
				if (isBreak)
					break;
			}
			if (curRecipeItem->second > 0)
				return false;
		}

		return true;
	}
	return false;
}

void build_system::was_placed()
{
	selected_object = entity_tag::emptyCell;
	building_position = Vector2f (-1, -1);
}

void build_system::clear_hare_bags(int block, grid_list& static_grid, std::vector<world_object*>* visible_items)
{
	for (auto& item : *visible_items)
	{
		if (static_grid.get_index_by_point(item->get_position().x, item->get_position().y) != static_grid.get_index_by_point(mouse_world_pos_.x, mouse_world_pos_.y))
			continue;
		bool match = false;
		auto droppedLoot = dynamic_cast<dropped_loot*>(item);
		if (droppedLoot && droppedLoot->get_type() == 201)
		{
			for (auto& cell : droppedLoot->inventory)
				if (cell.first == entity_tag::hare)
				{
					match = true;
					break;
				}
		}
		if (match)
		{
			item->delete_promise_on();
			break;
		}
	}
}

void build_system::animator(long long elapsed_time)
{
	if (built_objects_[0].icon_sprite.getPosition().x < built_objects_[0].icon_sprite.getTextureRect().width / 8 && animation_speed_ > 0)
		animation_speed_ -= (float)elapsed_time / 400000000;
	else
	{
		animation_speed_ = 0;
		for (int i = 0; i < built_objects_.size(); i++)
		{
			built_objects_[i].icon_sprite.setPosition(built_objects_[i].icon_sprite.getTextureRect().width / 8 + animation_speed_ * elapsed_time, built_objects_[i].icon_sprite.getPosition().y);
		}
	}
	for (int i = 0; i < built_objects_.size(); i++)
	{
		built_objects_[i].icon_sprite.setPosition(built_objects_[i].icon_sprite.getPosition().x + animation_speed_ * elapsed_time, built_objects_[i].icon_sprite.getPosition().y);
	}
}
