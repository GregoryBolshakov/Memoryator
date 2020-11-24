#include "brazier.h"

#include <fstream>

#include "helper.h"

brazier::brazier(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
{
	variety_of_types_ = 1;
	this->type_of_object_ = typeOfObject;
	radius_ = 450.0f;
	plateRadius = 100.0f;
	to_save_name_ = "brazier";
	brazier::setType(typeOfObject);
	is_multi_ellipse = true;
	mirrored_ = false;
	initCraftRecipes();
	tag = entity_tag::brazier;
}

void brazier::setType(int typeOfObject)
{
	this->type_of_object_ = typeOfObject;
	this->conditional_size_units_ = {900, 900};
}

Vector2f brazier::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return { texture_box_.width * 0.5f, texture_box_.height * 0.762f };
}

void brazier::init_pedestal()
{
	if (type_of_object_ == 1)
	{
		/*focus1 = Vector2f (position.x - textureBox.width / 4, position.y);
		focus2 = Vector2f (position.x + textureBox.width / 4, position.y);
		ellipseSize = float((focus2.x - focus1.x) * 1.17);*/
		focus1_ = Vector2f(position_.x, position_.y);
		focus2_ = Vector2f(position_.x, position_.y);

		std::pair<Vector2f, Vector2f> microEllipse;
		microEllipse.first = Vector2f(position_.x - texture_box_.width * 0.333f, position_.y + texture_box_.height * 0.04f);
		microEllipse.second = Vector2f(position_.x - texture_box_.width * 0.124f, position_.y + texture_box_.height * 0.04f);
		internal_ellipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position_.x - texture_box_.width * 0.25f, position_.y - texture_box_.height * 0.04f);
		microEllipse.second = Vector2f(position_.x, position_.y - texture_box_.height * 0.04f);
		internal_ellipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position_.x, position_.y - texture_box_.height * 0.02f);
		microEllipse.second = Vector2f(position_.x + texture_box_.width * 0.25f, position_.y - texture_box_.height * 0.02f);		
		internal_ellipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position_.x + texture_box_.width * 0.191f, position_.y + texture_box_.height * 0.0211f);
		microEllipse.second = Vector2f(position_.x + texture_box_.width * 0.335f, position_.y + texture_box_.height * 0.0211f);		
		internal_ellipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position_.x - texture_box_.width * 0.045f, position_.y + texture_box_.height * 0.19f);
		microEllipse.second = Vector2f(position_.x + texture_box_.width * 0.0698f, position_.y + texture_box_.height * 0.19f);		
		internal_ellipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position_.x + texture_box_.width * 0.155f, position_.y + texture_box_.height * 0.126f);
		microEllipse.second = Vector2f(position_.x + texture_box_.width * 0.327f, position_.y + texture_box_.height * 0.126f);		
		internal_ellipses.push_back(microEllipse);
	}
	ellipse_size_multipliers = { 1.2f, 1.25f, 1.17f, 1.58f, 1.25f, 1.43f };
	init_micro_blocks();
}

void brazier::initCraftRecipes()
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

void brazier::resultAnalysis() const
{
	if (craftResult == entity_tag::emptyCell)
		return;

	if (craftResult == entity_tag::heroBag)
	{
		
	}
}

entity_tag brazier::checkCraftResult()
{
	for (auto& recipes : craftRecipes)
		for (auto& recipe : recipes.second)
		{
			bool match = true;
			for (auto ingredient : recipe)
			{
				if (currentCraft.count(ingredient.first) < unsigned int(ingredient.second))
				{
					match = false;
				}
			}
			if (match)
			{
				//buildSystem->selectedObject = recipes.first;
				return recipes.first;
			}
		}

	return entity_tag::emptyCell;
}

void brazier::putItemToCraft(const entity_tag id)
{
	if (currentCraft.count(id) > 0)
		currentCraft.at(id)++;
	else
		currentCraft[id]++;

	craftResult = checkCraftResult();
}

Vector2f brazier::get_build_position(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

int brazier::get_build_type(Vector2f, Vector2f)
{
	return 1;
}

std::vector<unique_ptr<sprite_chain_element>> brazier::prepare_sprites(long long)
{
	std::vector<unique_ptr<sprite_chain_element>> result;
	const Vector2f front_offset(texture_box_.width * 0.506f, texture_box_.height * 0.949f);
	const Vector2f front_position(position_.x - texture_box_offset_.x + front_offset.x, position_.y - texture_box_offset_.y + front_offset.y);

	auto back = make_unique<sprite_chain_element>(pack_tag::locations, pack_part::brazier, direction::DOWN, 1, position_, conditional_size_units_, Vector2f(texture_box_offset_));
	auto front = make_unique<sprite_chain_element>(pack_tag::locations, pack_part::brazier, direction::DOWN, 2, front_position, conditional_size_units_, front_offset);

	result.emplace_back(std::move(back));
	result.emplace_back(std::move(front));

	return result;

	/*additionalSprites.clear();
	SpriteChainElement brazierBack, brazierFront, fire, craftIcon;
	brazierBack.packTag = PackTag::locations; brazierBack.packPart = PackPart::main_object, brazierBack.number = 1;
	brazierBack.size = Vector2f(conditionalSizeUnits);	
	brazierBack.offset = Vector2f(textureBoxOffset.x, textureBoxOffset.y - conditionalSizeUnits.y * 0.2f);
	brazierBack.position = { position.x, position.y - conditionalSizeUnits.y * 0.2f };
	brazierBack.antiTransparent = true;*/
	/*if (!currentCraft.empty())
	{
		fire.path = "Game/worldSprites/terrainObjects/main_object/fire.png";
		fire.size = Vector2f(conditionalSizeUnits.x * 0.22f, conditionalSizeUnits.y * 0.22f);
		fire.offset = Vector2f(conditionalSizeUnits.x * 0.06f, textureBoxOffset.y - conditionalSizeUnits.y * 0.555f + scaleFactor * conditionalSizeUnits.y / 20);
		fire.position = { position.x, position.y };
		fire.antiTransparent = true;
	}*/
	/*brazierFront.packTag = PackTag::locations; brazierFront.packPart = PackPart::main_object, brazierFront.number = 2;
	brazierFront.size = Vector2f(conditionalSizeUnits);
	brazierFront.offset = Vector2f(textureBoxOffset.x, textureBoxOffset.y + conditionalSizeUnits.y * 0.2f);
	brazierFront.position = { position.x, position.y + conditionalSizeUnits.y * 0.2f };
	brazierFront.antiTransparent = true;
	additionalSprites.push_back(brazierBack);
	additionalSprites.push_back(fire);
	additionalSprites.push_back(brazierFront);*/
}