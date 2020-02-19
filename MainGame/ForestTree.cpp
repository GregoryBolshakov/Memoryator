#include "ForestTree.h"
#include "Helper.h"

ForestTree::ForestTree(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 18; // BirchGrove: 1-7; DarkWoods: 8-13; SwampyTrees: 14-18
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
		conditionalSizeUnits = { 396, 1090 };
	if (typeOfObject == 2)
		conditionalSizeUnits = { 536, 1150 };
	if (typeOfObject == 3)
		conditionalSizeUnits = { 244, 799 };
	if (typeOfObject == 4)
		conditionalSizeUnits = { 623, 875 };
	if (typeOfObject == 5)
		conditionalSizeUnits = { 644, 1235 };
	if (typeOfObject == 6)
		conditionalSizeUnits = { 681, 1027 };
	if (typeOfObject == 7)
		conditionalSizeUnits = { 616, 1169 };
	if (typeOfObject == 8)
		conditionalSizeUnits = { 457, 1152 };
	if (typeOfObject == 9)
		conditionalSizeUnits = { 527, 1188 };
	if (typeOfObject == 10)
		conditionalSizeUnits = { 786, 1296 };
	if (typeOfObject == 11)
		conditionalSizeUnits = { 806, 1292 };
	if (typeOfObject == 12)
		conditionalSizeUnits = { 795, 1190 };
	if (typeOfObject == 13)
		conditionalSizeUnits = { 1221, 386 };
	if (typeOfObject == 14)
		conditionalSizeUnits = { 952, 962 };
	if (typeOfObject == 15)
		conditionalSizeUnits = { 565, 1261 };
	if (typeOfObject == 16)
		conditionalSizeUnits = { 914, 1108 };
	if (typeOfObject == 17)
		conditionalSizeUnits = { 652, 1359 };
	if (typeOfObject == 18)
		conditionalSizeUnits = { 667, 717 };

	const float extension = int(90 + rand() % 20) / 100.0f; // in percents
	//conditionalSizeUnits.x *= extension; conditionalSizeUnits.y *= extension;
}

Vector2f ForestTree::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;

	switch (typeOfObject)
	{
		case 1:
			return { textureBox.width * 0.257f, textureBox.height * 0.932f };
		case 2:
			return { textureBox.width * 0.45f, textureBox.height * 0.96f };
		case 3:
			return { textureBox.width * 0.55f, textureBox.height * 0.96f };
		case 4:
			return { textureBox.width * 0.409f, textureBox.height * 0.945f };
		case 5:
			return { textureBox.width * 0.41f, textureBox.height * 0.935f };
		case 6:
			return { textureBox.width * 0.536f, textureBox.height * 0.94f };
		case 7:
			return { textureBox.width * 0.526f, textureBox.height * 0.936f };
		case 8:
			return { textureBox.width * 0.560f, textureBox.height * 0.96f };
		case 9:
			return { textureBox.width * 0.467f, textureBox.height * 0.94f };
		case 10:
			return { textureBox.width * 0.467f, textureBox.height * 0.94f };
		case 11:
			return { textureBox.width * 0.554f, textureBox.height * 0.95f };
		case 12:
			return { textureBox.width * 0.327f, textureBox.height * 0.93f };
		case 13:
			return { textureBox.width * 0.574f, textureBox.height * 0.508f };
		case 14:
			return { textureBox.width * 0.521f, textureBox.height * 0.883f };
		case 15:
			return { textureBox.width * 0.509f, textureBox.height * 0.857f };
		case 16:
			return { textureBox.width * 0.537f, textureBox.height * 0.83f };
		case 17:
			return { textureBox.width * 0.349f, textureBox.height * 0.861f };
		case 18:
			return { textureBox.width * 0.439f, textureBox.height * 0.857f };
		default:
			return { textureBox.width * 0.45f, textureBox.height * 0.96f };
	}
}

void ForestTree::initPedestal()
{
	std::pair<Vector2f, Vector2f> microEllipse;
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
			this->isMultiellipse = true;
			focus1 = Vector2f(position.x, position.y);
			focus2 = Vector2f(position.x, position.y);

			microEllipse.first = Vector2f(position.x - textureBox.width * 0.42, position.y + textureBox.height * 0.247);
			microEllipse.second = Vector2f(position.x - textureBox.width * 0.178, position.y + textureBox.height * 0.247);
			internalEllipses.push_back(microEllipse);

			microEllipse.first = Vector2f(position.x - textureBox.width * 0.235, position.y + textureBox.height * 0.171);
			microEllipse.second = Vector2f(position.x - textureBox.width * 0.026, position.y + textureBox.height * 0.171);
			internalEllipses.push_back(microEllipse);

			microEllipse.first = Vector2f(position.x - textureBox.width * 0.044, position.y + textureBox.height * 0.047);
			microEllipse.second = Vector2f(position.x + textureBox.width * 0.132, position.y + textureBox.height * 0.047);
			internalEllipses.push_back(microEllipse);

			microEllipse.first = Vector2f(position.x + textureBox.width * 0.14, position.y - textureBox.height * 0.012);
			microEllipse.second = Vector2f(position.x + textureBox.width * 0.385, position.y - textureBox.height * 0.012);
			internalEllipses.push_back(microEllipse);
			ellipseSizeMultipliers = { 1.03, 1.09, 1.27, 1.12 };
			break;
		case 14:
			focus1 = Vector2f(position.x - textureBox.width * 0.079f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.079f, position.y);
			ellipseSizeMultipliers[0] = { 1.42 };
			break;
		case 15:
			focus1 = Vector2f(position.x - textureBox.width * 0.09f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.09f, position.y);
			ellipseSizeMultipliers[0] = { 1.55 };
			break;
		case 16:
			focus1 = Vector2f(position.x - textureBox.width * 0.066f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.066f, position.y);
			ellipseSizeMultipliers[0] = { 1.46 };
			break;
		case 17:
			focus1 = Vector2f(position.x - textureBox.width * 0.162f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.162f, position.y);
			ellipseSizeMultipliers[0] = { 1.25 };
			break;
		case 18:
			focus1 = Vector2f(position.x - textureBox.width * 0.109f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.109f, position.y);
			ellipseSizeMultipliers[0] = { 1.25 };
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

std::vector<SpriteChainElement*> ForestTree::prepareSprites(long long elapsedTime)
{
	SpriteChainElement* body = new SpriteChainElement(PackTag::darkWoods, PackPart::tree, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset), color, mirrored);

	if (typeOfObject >= 1 && typeOfObject <= 7)
		body->packTag = PackTag::birchGrove;
	if (typeOfObject >= 8 && typeOfObject <= 13)
	{
		body->packTag = PackTag::darkWoods;
		body->number -= 7;
	}
	if (typeOfObject >= 14 && typeOfObject <= 18)
	{
		body->packTag = PackTag::swampyTrees;
		body->number -= 13;
	}

	if (state == absorbed)
	{
		body->animationLength = 15;
		color.a = 255 - currentSprite[0] * 255 / body->animationLength;
	}

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 1e6 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > body->animationLength)
		{
			if (state == absorbed)
			{
				state = common;
				deletePromiseOn();
			}
			currentSprite[0] = 1;
		}
	}

	return { body };
}