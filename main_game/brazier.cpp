#include "brazier.h"
#include "build_system.h"
#include "direction_system.h"
#include "helper.h"
#include "sprite_chain_element.h"

#include <fstream>

brazier::brazier(std::string name, const sf::Vector2f position, const int kind) : terrain_object(std::move(name), position, kind),
	craftResult(entity_tag::empty_cell)
{
	tag = entity_tag::brazier;
	size_ = { 900, 900 };
	offset_ = { size_.x * 0.5f, size_.y * 0.762f };
	radius_ = 450.0f;
	plateRadius = 100.0f;
	is_multi_ellipse = true;
	mirrored_ = false;
	initCraftRecipes();
}

void brazier::init_pedestal()
{
	if (kind_ == 1)
	{
		/*focus1 = sf::Vector2f (position.x - textureBox.width / 4, position.y);
		focus2 = sf::Vector2f (position.x + textureBox.width / 4, position.y);
		ellipseSize = float((focus2.x - focus1.x) * 1.17);*/
		focus1_ = sf::Vector2f(position_.x, position_.y);
		focus2_ = sf::Vector2f(position_.x, position_.y);

		std::pair<sf::Vector2f, sf::Vector2f> microEllipse;
		microEllipse.first = sf::Vector2f(position_.x - size_.x * 0.333f, position_.y + size_.y * 0.04f);
		microEllipse.second = sf::Vector2f(position_.x - size_.x * 0.124f, position_.y + size_.y * 0.04f);
		internal_ellipses.push_back(microEllipse);

		microEllipse.first = sf::Vector2f(position_.x - size_.x * 0.25f, position_.y - size_.y * 0.04f);
		microEllipse.second = sf::Vector2f(position_.x, position_.y - size_.y * 0.04f);
		internal_ellipses.push_back(microEllipse);

		microEllipse.first = sf::Vector2f(position_.x, position_.y - size_.y * 0.02f);
		microEllipse.second = sf::Vector2f(position_.x + size_.x * 0.25f, position_.y - size_.y * 0.02f);
		internal_ellipses.push_back(microEllipse);

		microEllipse.first = sf::Vector2f(position_.x + size_.x * 0.191f, position_.y + size_.y * 0.0211f);
		microEllipse.second = sf::Vector2f(position_.x + size_.x * 0.335f, position_.y + size_.y * 0.0211f);
		internal_ellipses.push_back(microEllipse);

		microEllipse.first = sf::Vector2f(position_.x - size_.x * 0.045f, position_.y + size_.y * 0.19f);
		microEllipse.second = sf::Vector2f(position_.x + size_.x * 0.0698f, position_.y + size_.y * 0.19f);
		internal_ellipses.push_back(microEllipse);

		microEllipse.first = sf::Vector2f(position_.x + size_.x * 0.155f, position_.y + size_.y * 0.126f);
		microEllipse.second = sf::Vector2f(position_.x + size_.x * 0.327f, position_.y + size_.y * 0.126f);
		internal_ellipses.push_back(microEllipse);
	}
	ellipse_size_multipliers = { 1.2f, 1.25f, 1.17f, 1.58f, 1.25f, 1.43f };
	init_route_blocks();
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
	if (craftResult == entity_tag::empty_cell)
		return;

	if (craftResult == entity_tag::hero_bag)
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

	return entity_tag::empty_cell;
}

void brazier::putItemToCraft(const entity_tag id)
{
	if (currentCraft.count(id) > 0)
		currentCraft.at(id)++;
	else
		currentCraft[id]++;

	craftResult = checkCraftResult();
}

std::vector<unique_ptr<sprite_chain_element>> brazier::prepare_sprites(long long)
{
	std::vector<unique_ptr<sprite_chain_element>> result;

	auto back = make_unique<sprite_chain_element>(pack_tag::locations, pack_part::brazier, direction::DOWN, 1, position_, size_, offset_);
	auto front = make_unique<sprite_chain_element>(pack_tag::locations, pack_part::brazier, direction::DOWN, 2, position_, size_, offset_);

	result.emplace_back(std::move(back));
	result.emplace_back(std::move(front));

	set_isometric_state(result, true);
	return result;

	/*additionalSprites.clear();
	SpriteChainElement brazierBack, brazierFront, fire, craftIcon;
	brazierBack.packTag = PackTag::locations; brazierBack.packPart = PackPart::main_object, brazierBack.number = 1;
	brazierBack.size = sf::Vector2f(conditionalSizeUnits);	
	brazierBack.offset = sf::Vector2f(textureBoxOffset.x, textureBoxOffset.y - conditionalSizeUnits.y * 0.2f);
	brazierBack.position = { position.x, position.y - conditionalSizeUnits.y * 0.2f };
	brazierBack.antiTransparent = true;*/
	/*if (!currentCraft.empty())
	{
		fire.path = "Game/worldSprites/terrainObjects/main_object/fire.png";
		fire.size = sf::Vector2f(conditionalSizeUnits.x * 0.22f, conditionalSizeUnits.y * 0.22f);
		fire.offset = sf::Vector2f(conditionalSizeUnits.x * 0.06f, textureBoxOffset.y - conditionalSizeUnits.y * 0.555f + scaleFactor * conditionalSizeUnits.y / 20);
		fire.position = { position.x, position.y };
		fire.antiTransparent = true;
	}*/
	/*brazierFront.packTag = PackTag::locations; brazierFront.packPart = PackPart::main_object, brazierFront.number = 2;
	brazierFront.size = sf::Vector2f(conditionalSizeUnits);
	brazierFront.offset = sf::Vector2f(textureBoxOffset.x, textureBoxOffset.y + conditionalSizeUnits.y * 0.2f);
	brazierFront.position = { position.x, position.y + conditionalSizeUnits.y * 0.2f };
	brazierFront.antiTransparent = true;
	additionalSprites.push_back(brazierBack);
	additionalSprites.push_back(fire);
	additionalSprites.push_back(brazierFront);*/
}