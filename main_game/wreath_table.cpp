#include "wreath_table.h"

#include <fstream>

#include "object_initializer.h"

wreath_table::wreath_table(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
{
	variety_of_types_ = 1;
	this->type_of_object_ = typeOfObject;
	radius_ = 200;
	plateRadius = 100;
	to_save_name_ = "wreathTable";
	wreath_table::setType(typeOfObject);
	mirrored_ = false;
	initCraftRecipes();
	tag = entity_tag::wreathTable;
}

void wreath_table::setType(int typeOfObject)
{
	this->type_of_object_ = typeOfObject;
	this->conditional_size_units_ = { 588, 523 };
}

Vector2f wreath_table::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return { texture_box_.width / 2.0f, texture_box_.height / 1.3f };
}

void wreath_table::init_pedestal()
{
	focus1_ = Vector2f(position_.x - texture_box_.width / 4, position_.y);
	focus2_ = Vector2f(position_.x + texture_box_.width / 4, position_.y);
	ellipse_size_multipliers[0] = { 1.2f };
	init_micro_blocks();
}

void wreath_table::initCraftRecipes()
{
	std::ifstream fin(craftRecipesPath);

	int craftedItemId, recipeSize;

	while (fin >> craftedItemId >> recipeSize)
	{
		std::vector<std::pair<entity_tag, int>> recipe = {};
		for (int i = 0; i < recipeSize; i++)
		{
			int ingredientId = 0, ingredientCount = 0;
			fin >> ingredientId >> ingredientCount;
			recipe.push_back(std::make_pair(entity_tag(ingredientId), ingredientCount));
		}
		craftRecipes[entity_tag(craftedItemId)].push_back(recipe);
	}

	fin.close();
}

void wreath_table::resultAnalysis() const
{
	if (craftResult == entity_tag::emptyCell)
		return;

	if (craftResult == entity_tag::heroBag)
	{

	}
}

entity_tag wreath_table::checkCraftResult()
{
	for (auto& recipes : craftRecipes)
		for (auto& recipe : recipes.second)
		{
			bool match = true;
			for (auto ingredient : recipe)
				if (currentCraft.count(ingredient.first) < unsigned(ingredient.second))
				{
					match = false;
				}
			
			if (match)
			{
				//buildSystem->selectedObject = recipes.first;
				return recipes.first;
			}
		}

	return entity_tag::emptyCell;
}

void wreath_table::putItemToCraft(entity_tag id)
{
	if (currentCraft.count(id) > 0)
		currentCraft.at(id)++;
	else
		currentCraft[id]++;

	craftResult = checkCraftResult();
}

Vector2f wreath_table::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int wreath_table::get_build_type(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<sprite_chain_element*> wreath_table::prepare_sprites(long long elapsedTime)
{
	const auto body = new sprite_chain_element(pack_tag::locations, pack_part::wreathTable, direction::DOWN, type_of_object_, position_, conditional_size_units_, Vector2f(texture_box_offset_), color, mirrored_);

	return { body };
}