#include <fstream>

#include "Brazier.h"
#include "Helper.h"

Brazier::Brazier(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : TerrainObject(std::move(objectName), centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 450.0f;
	plateRadius = 100.0f;
	toSaveName = "brazier";
	Brazier::setType(typeOfObject);
	isMultiEllipse = true;
	mirrored = false;
	initCraftRecipes();
	tag = Tag::brazier;
}

void Brazier::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = {900, 900};
}

Vector2f Brazier::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width * 0.5f, textureBox.height * 0.762f };
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
		microEllipse.first = Vector2f(position.x - textureBox.width * 0.333f, position.y + textureBox.height * 0.04f);
		microEllipse.second = Vector2f(position.x - textureBox.width * 0.124f, position.y + textureBox.height * 0.04f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.25f, position.y - textureBox.height * 0.04f);
		microEllipse.second = Vector2f(position.x, position.y - textureBox.height * 0.04f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x, position.y - textureBox.height * 0.02f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.25f, position.y - textureBox.height * 0.02f);		
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.191f, position.y + textureBox.height * 0.0211f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.335f, position.y + textureBox.height * 0.0211f);		
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.045f, position.y + textureBox.height * 0.19f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.0698f, position.y + textureBox.height * 0.19f);		
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.155f, position.y + textureBox.height * 0.126f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.327f, position.y + textureBox.height * 0.126f);		
		internalEllipses.push_back(microEllipse);
	}
	ellipseSizeMultipliers = { 1.2f, 1.25f, 1.17f, 1.58f, 1.25f, 1.43f };
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

void Brazier::resultAnalysis() const
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

	return Tag::emptyCell;
}

void Brazier::putItemToCraft(const Tag id)
{
	if (currentCraft.count(id) > 0)
		currentCraft.at(id)++;
	else
		currentCraft[id]++;

	craftResult = checkCraftResult();
}

Vector2f Brazier::getBuildPosition(std::vector<WorldObject*>, float, Vector2f)
{
	return { -1, -1 };
}

int Brazier::getBuildType(Vector2f, Vector2f)
{
	return 1;
}

std::vector<SpriteChainElement*> Brazier::prepareSprites(long long)
{
    const Vector2f frontOffset(textureBox.width * 0.506f, textureBox.height * 0.949f);
	const Vector2f frontPosition(position.x - textureBoxOffset.x + frontOffset.x, position.y - textureBoxOffset.y + frontOffset.y);
	
    const auto back = new SpriteChainElement(PackTag::locations, PackPart::brazier, Direction::DOWN, 1, position, conditionalSizeUnits, Vector2f(textureBoxOffset));
    const auto front = new SpriteChainElement(PackTag::locations, PackPart::brazier, Direction::DOWN, 2, frontPosition, conditionalSizeUnits, frontOffset);
	
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