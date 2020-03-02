#include "wreath_table.h"

#include <fstream>

#include "object_initializer.h"

wreath_table::wreath_table(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 200;
	plateRadius = 100;
	toSaveName = "wreathTable";
	wreath_table::setType(typeOfObject);
	mirrored = false;
	initCraftRecipes();
	tag = entity_tag::wreathTable;
}

void wreath_table::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = { 588, 523 };
}

Vector2f wreath_table::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2.0f, textureBox.height / 1.3f };
}

void wreath_table::initPedestal()
{
	focus1 = Vector2f(position.x - textureBox.width / 4, position.y);
	focus2 = Vector2f(position.x + textureBox.width / 4, position.y);
	ellipseSizeMultipliers[0] = { 1.2f };
	initMicroBlocks();
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

Vector2f wreath_table::getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int wreath_table::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<sprite_chain_element*> wreath_table::prepareSprites(long long elapsedTime)
{
	const auto body = new sprite_chain_element(pack_tag::locations, pack_part::wreathTable, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset), color, mirrored);

	return { body };
}