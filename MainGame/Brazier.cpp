#include "Brazier.h"
#include "Helper.h"
#include <fstream>

Brazier::Brazier(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 450;
	plateRadius = 100;
	toSaveName = "brazier";
	Brazier::setType(typeOfObject);
	isMultiellipse = true;
	mirrored = false;
	initCraftRecipes();
	tag = Tag::brazier;
}

void Brazier::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2i (900, 900);
}

Vector2i Brazier::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width * 0.5, int(textureBox.height * 0.762));
}

void Brazier::initPedestal()
{
	if (typeOfObject == 1)
	{
		/*focus1 = Vector2f (position.x - textureBox.width / 4, position.y);
		focus2 = Vector2f (position.x + textureBox.width / 4, position.y);
		ellipseSize = float((focus2.x - focus1.x) * 1.17);*/
		focus1 = Vector2f(position.x, position.y);
		focus2 = Vector2f(position.x, position.y);

		std::pair<Vector2f, Vector2f> microEllipse;
		microEllipse.first = Vector2f(position.x - textureBox.width * 0.333, position.y + textureBox.height * 0.04);
		microEllipse.second = Vector2f(position.x - textureBox.width * 0.124, position.y + textureBox.height * 0.04);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.25, position.y - textureBox.height * 0.04);
		microEllipse.second = Vector2f(position.x, position.y - textureBox.height * 0.04);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x, position.y - textureBox.height * 0.02);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.25, position.y - textureBox.height * 0.02);		
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.191, position.y + textureBox.height * 0.0211);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.335, position.y + textureBox.height * 0.0211);		
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.045, position.y + textureBox.height * 0.19);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.0698, position.y + textureBox.height * 0.19);		
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.155, position.y + textureBox.height * 0.126);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.327, position.y + textureBox.height * 0.126);		
		internalEllipses.push_back(microEllipse);
	}
	ellipseSizeMultipliers = { 1.2, 1.25, 1.17, 1.58, 1.25, 1.43 };
	initMicroBlocks();
}

void Brazier::initCraftRecipes()
{
	std::ifstream fin(craftRecipesPath);

	int craftedItemId, recipeSize;

	while (fin >> craftedItemId >> recipeSize)
	{
		std::vector<std::pair<Tag, int>> recipe = {};
		for (int i = 0; i < recipeSize; i++)
		{
			int ingredientId = 0, ingredientCount = 0;
			fin >> ingredientId >> ingredientCount;
			recipe.push_back(std::make_pair(Tag(ingredientId), ingredientCount));
		}
		craftRecipes[Tag(craftedItemId)].push_back(recipe);
	}

	fin.close();
}

void Brazier::resultAnalysis()
{
	if (craftResult == Tag::emptyCell)
		return;

	if (craftResult == Tag::heroBag)
	{
		
	}
}

Tag Brazier::checkCraftResult()
{
	for (auto& recipes : craftRecipes)
		for (auto& recipe : recipes.second)
		{
			bool match = true;
			for (auto ingredient : recipe)		
				if (currentCraft.count(ingredient.first) < ingredient.second)
				{
					match = false;
					continue;
				}
			if (match)
			{
				//buildSystem->selectedObject = recipes.first;
				return recipes.first;
			}
		}

	return Tag::emptyCell;
}

void Brazier::putItemToCraft(Tag id)
{
	if (currentCraft.count(id) > 0)
		currentCraft.at(id)++;
	else
		currentCraft[id]++;

	craftResult = checkCraftResult();
}

Vector2f Brazier::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Brazier::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<SpriteChainElement> Brazier::prepareSprites(long long elapsedTime)
{
    const Vector2f frontOffset(textureBox.width * 0.506, int(textureBox.height * 0.949)), frontPosition(position.x - textureBoxOffset.x + frontOffset.x, position.y - textureBoxOffset.y + frontOffset.y);
    SpriteChainElement back(PackTag::locations, PackPart::brazier, Direction::DOWN, 1, position, conditionalSizeUnits, Vector2f(textureBoxOffset));	
    SpriteChainElement front(PackTag::locations, PackPart::brazier, Direction::DOWN, 2, frontPosition, conditionalSizeUnits, frontOffset);
    return {back, front};
	/*additionalSprites.clear();
	SpriteChainElement brazierBack, brazierFront, fire, craftIcon;
	brazierBack.packTag = PackTag::locations; brazierBack.packPart = PackPart::brazier, brazierBack.number = 1;
	brazierBack.size = Vector2f(conditionalSizeUnits);	
	brazierBack.offset = Vector2f(textureBoxOffset.x, textureBoxOffset.y - conditionalSizeUnits.y * 0.2f);
	brazierBack.position = { position.x, position.y - conditionalSizeUnits.y * 0.2f };
	brazierBack.antiTransparent = true;*/
	/*if (!currentCraft.empty())
	{
		fire.path = "Game/worldSprites/terrainObjects/brazier/fire.png";
		fire.size = Vector2f(conditionalSizeUnits.x * 0.22f, conditionalSizeUnits.y * 0.22f);
		fire.offset = Vector2f(conditionalSizeUnits.x * 0.06f, textureBoxOffset.y - conditionalSizeUnits.y * 0.555f + scaleFactor * conditionalSizeUnits.y / 20);
		fire.position = { position.x, position.y };
		fire.antiTransparent = true;
	}*/
    /*brazierFront.packTag = PackTag::locations; brazierFront.packPart = PackPart::brazier, brazierFront.number = 2;
	brazierFront.size = Vector2f(conditionalSizeUnits);
	brazierFront.offset = Vector2f(textureBoxOffset.x, textureBoxOffset.y + conditionalSizeUnits.y * 0.2f);
	brazierFront.position = { position.x, position.y + conditionalSizeUnits.y * 0.2f };
	brazierFront.antiTransparent = true;
	additionalSprites.push_back(brazierBack);
	additionalSprites.push_back(fire);
    additionalSprites.push_back(brazierFront);*/
}