#include "ForestTree.h"
#include "Helper.h"

ForestTree::ForestTree(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 13;
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "ForestTree";
	setType(typeOfObject);
	tag = Tag::tree;
}

void ForestTree::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i (396, 1090);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i (536, 1150);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i (244, 799);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i (623, 875);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i (644, 1235);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2i (681, 1027);
	if (typeOfObject == 7)
		conditionalSizeUnits = Vector2i (616, 1169);
	if (typeOfObject == 8)
		conditionalSizeUnits = Vector2i (457, 1152);
	if (typeOfObject == 9)
		conditionalSizeUnits = Vector2i (527, 1188);
	if (typeOfObject == 10)
		conditionalSizeUnits = Vector2i(786, 1296);
	if (typeOfObject == 11)
		conditionalSizeUnits = Vector2i(806, 1292);
	if (typeOfObject == 12)
		conditionalSizeUnits = Vector2i(795, 1190);
	if (typeOfObject == 13)
		conditionalSizeUnits = Vector2i(1221, 386);

	const float extension = int(90 + rand() % 20) / 100.0f; // in percents
	//conditionalSizeUnits.x *= extension; conditionalSizeUnits.y *= extension;
}

Vector2i ForestTree::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	return Vector2i (textureBox.width / 2.2, int(textureBox.height / 1.1));
}

void ForestTree::initPedestal()
{	
	focus1 = Vector2f (position.x - textureBox.width / 8, position.y);
	focus2 = Vector2f (position.x + textureBox.width / 8, position.y);
	ellipseSize = float((focus2.x - focus1.x) * 1.4);
	initMicroBlocks();
}

Vector2f ForestTree::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

Vector2f ForestTree::getOwlBase()
{
	return { position.x, position.y - conditionalSizeUnits.y / 2 };
}

int ForestTree::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void ForestTree::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
    additionalSprites.clear();
    spriteChainElement treeBody;
	treeBody.size = Vector2f(conditionalSizeUnits);
	treeBody.offset = Vector2f(textureBoxOffset);

	if (typeOfObject >= 1 && typeOfObject <= 7)
	{
		animationLength = 1;
		treeBody.path = "Game/worldSprites/BirchGrove/birch" + std::to_string(typeOfObject) + ".png";
	}
	if (typeOfObject >= 8 && typeOfObject <= 13)
	{
		animationLength = 28;
		treeBody.path = "Game/worldSprites/DarkWoods/tree" + std::to_string(typeOfObject - 7) + "/" + std::to_string(currentSprite[0]) + ".png";
	}

	if (state == absorbed)
	{
		animationLength = 15;
		color.a = 255 - currentSprite[0] * 255 / animationLength;
	}

    additionalSprites.push_back(treeBody);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 1e6 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			if (state == absorbed)
			{
				state = common;
				deletePromiseOn();
			}
			currentSprite[0] = 1;
		}
	}
}