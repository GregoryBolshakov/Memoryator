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

	switch (typeOfObject)
	{
		case 1:
			return Vector2i(textureBox.width * 0.257f, int(textureBox.height * 0.932f));
		case 2:
			return Vector2i(textureBox.width * 0.45f, int(textureBox.height * 0.96f));
		case 3:
			return Vector2i(textureBox.width * 0.55f, int(textureBox.height * 0.96f));
		case 4:
			return Vector2i(textureBox.width * 0.409f, int(textureBox.height * 0.945f));
		case 5:
			return Vector2i(textureBox.width * 0.41f, int(textureBox.height * 0.935f));
		case 6:
			return Vector2i(textureBox.width * 0.536f, int(textureBox.height * 0.94f));
		case 7:
			return Vector2i(textureBox.width * 0.526f, int(textureBox.height * 0.936f));
		case 8:
			return Vector2i(textureBox.width * 0.560f, int(textureBox.height * 0.96f));
		case 9:
			return Vector2i(textureBox.width * 0.467f, int(textureBox.height * 0.94f));
		case 10:
			return Vector2i(textureBox.width * 0.467f, int(textureBox.height * 0.94f));
		case 11:
			return Vector2i(textureBox.width * 0.554f, int(textureBox.height * 0.95f));
		case 12:
			return Vector2i(textureBox.width * 0.327f, int(textureBox.height * 0.93f));
		case 13:
			return Vector2i(textureBox.width * 0.736f, int(textureBox.height * 0.51f));
		default:
			return Vector2i(textureBox.width * 0.45f, int(textureBox.height * 0.96f));
	}
}

void ForestTree::initPedestal()
{
	switch (typeOfObject)
	{
		case 1:		
			focus1 = Vector2f(position.x - textureBox.width * 0.158f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.158f, position.y);
			ellipseSizeMultipliers[0] = { 1.44 };
			break;		
		case 2:		
			focus1 = Vector2f(position.x - textureBox.width * 0.07f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.07f, position.y);
			ellipseSizeMultipliers[0] = { 1.58 };
			break;		
		case 3:		
			focus1 = Vector2f(position.x - textureBox.width * 0.184f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.184f, position.y);
			ellipseSizeMultipliers[0] = { 1.42 };
			break;		
		case 4:		
			focus1 = Vector2f(position.x - textureBox.width * 0.082f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.082f, position.y);
			ellipseSizeMultipliers[0] = { 1.42 };
			break;		
		case 5:		
			focus1 = Vector2f(position.x - textureBox.width * 0.088f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.088f, position.y);
			ellipseSizeMultipliers[0] = { 1.57 };
			break;		
		case 6:		
			focus1 = Vector2f(position.x - textureBox.width * 0.084f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.084f, position.y);
			ellipseSizeMultipliers[0] = { 1.31 };
			break;		
		case 7:		
			focus1 = Vector2f(position.x - textureBox.width * 0.131f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.131f, position.y);
			ellipseSizeMultipliers[0] = { 1.28 };
			break;		
		case 8:		
			focus1 = Vector2f(position.x - textureBox.width * 0.118f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.118f, position.y);
			ellipseSizeMultipliers[0] = { 1.42 };
			break;
		case 9:		
			focus1 = Vector2f(position.x - textureBox.width * 0.124f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.124f, position.y);
			ellipseSizeMultipliers[0] = { 1.21 };
			break;		
		case 10:		
			focus1 = Vector2f(position.x - textureBox.width * 0.127f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.127f, position.y);
			ellipseSizeMultipliers[0] = { 1.25 };
			break;		
		case 11:		
			focus1 = Vector2f(position.x - textureBox.width * 0.163f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.163f, position.y);
			ellipseSizeMultipliers[0] = { 1.25 };
			break;		
		case 12:		
			focus1 = Vector2f(position.x - textureBox.width * 0.137f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.137f, position.y);
			ellipseSizeMultipliers[0] = { 1.25 };
			break;		
		case 13:		
			focus1 = Vector2f(position.x - textureBox.width * 0.195f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.195f, position.y);
			ellipseSizeMultipliers[0] = { 1.05 };
			break;		
		default:		
			focus1 = Vector2f(position.x - textureBox.width * 0.106f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.106f, position.y);
			ellipseSizeMultipliers[0] = { 1.82 };
			break;		
	}
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
		animationLength = 1;
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