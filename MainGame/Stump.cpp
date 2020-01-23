#include "Stump.h"
#include "Helper.h"

Stump::Stump(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 13; // BirchGrove: 1-4; DarkWoods: 5-10; SwampyTrees: 11-13
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "stump";
	setType(typeOfObject);
	tag = Tag::stump;
}

void Stump::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i (412, 256);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i (507, 337);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i (647, 312);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i (462, 269);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i(243, 203);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2i(236, 315);
	if (typeOfObject == 7)
		conditionalSizeUnits = Vector2i(286, 576);
	if (typeOfObject == 8)
		conditionalSizeUnits = Vector2i(354, 570);
	if (typeOfObject == 9)
		conditionalSizeUnits = Vector2i(335, 266);
	if (typeOfObject == 10)
		conditionalSizeUnits = Vector2i(186, 412);
	if (typeOfObject == 11)
		conditionalSizeUnits = Vector2i(250, 663);
	if (typeOfObject == 12)
		conditionalSizeUnits = Vector2i(526, 441);
	if (typeOfObject == 13)
		conditionalSizeUnits = Vector2i(267, 434);
}

Vector2i Stump::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	switch (typeOfObject)
	{
	case 1:
		return Vector2i(textureBox.width * 0.551f, int(textureBox.height * 0.645f));
	case 2:
		return Vector2i(textureBox.width * 0.542f, int(textureBox.height * 0.64f));
	case 3:
		return Vector2i(textureBox.width * 0.518f, int(textureBox.height * 0.446f));
	case 4:
		return Vector2i(textureBox.width * 0.485f, int(textureBox.height * 0.591f));
	case 5:
		return Vector2i(textureBox.width * 0.551f, int(textureBox.height * 0.771f));
	case 6:
		return Vector2i(textureBox.width * 0.468f, int(textureBox.height * 0.796f));
	case 7:
		return Vector2i(textureBox.width * 0.497, int(textureBox.height * 0.859f));
	case 8:
		return Vector2i(textureBox.width * 0.525f, int(textureBox.height * 0.889f));
	case 9:
		return Vector2i(textureBox.width * 0.501f, int(textureBox.height * 0.728f));
	case 10:
		return Vector2i(textureBox.width * 0.46f, int(textureBox.height * 0.83f));
	case 11:
		return Vector2i(textureBox.width * 0.467f, int(textureBox.height * 0.868f));
	case 12:
		return Vector2i(textureBox.width * 0.554f, int(textureBox.height * 0.758f));
	case 13:
		return Vector2i(textureBox.width * 0.46f, int(textureBox.height * 0.83f));
	default:
		return Vector2i(textureBox.width * 0.409f, int(textureBox.height * 0.945f));
	}	
}

void Stump::initPedestal()
{
	switch (typeOfObject)
	{
	case 1:
		focus1 = Vector2f(position.x - textureBox.width * 0.167f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.167f, position.y);
		ellipseSizeMultipliers[0] = { 1.4 };
		break;
	case 2:
		focus1 = Vector2f(position.x - textureBox.width * 0.353f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.353f, position.y);
		ellipseSizeMultipliers[0] = { 1.15 };
		break;
	case 3:
		focus1 = Vector2f(position.x - textureBox.width * 0.248f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.248f, position.y);
		ellipseSizeMultipliers[0] = { 1.25 };
		break;
	case 4:
		focus1 = Vector2f(position.x - textureBox.width * 0.311f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.311f, position.y);
		ellipseSizeMultipliers[0] = { 1.307 };
		break;
	case 5:
		focus1 = Vector2f(position.x - textureBox.width * 0.167f, position.y - textureBox.height * 0.091f);
		focus2 = Vector2f(position.x + textureBox.width * 0.167f, position.y - textureBox.height * 0.091f);
		ellipseSizeMultipliers[0] = { 1.64 };
		break;
	case 6:
		focus1 = Vector2f(position.x - textureBox.width * 0.327f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.327f, position.y);
		ellipseSizeMultipliers[0] = { 1.34 };
		break;
	case 7:
		focus1 = Vector2f(position.x - textureBox.width * 0.268f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.268f, position.y);
		ellipseSizeMultipliers[0] = { 1.388 };
		break;
	case 8:
		focus1 = Vector2f(position.x - textureBox.width * 0.142f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.142f, position.y);
		ellipseSizeMultipliers[0] = { 1.62 };
		break;
	case 9:
		focus1 = Vector2f(position.x - textureBox.width * 0.314f, position.y - textureBox.height * 0.07f);
		focus2 = Vector2f(position.x + textureBox.width * 0.314f, position.y - textureBox.height * 0.07f);
		ellipseSizeMultipliers[0] = { 1.24 };
		break;
	case 10:
		focus1 = Vector2f(position.x - textureBox.width * 0.335f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.335f, position.y);
		ellipseSizeMultipliers[0] = { 1.43};
		break;
	case 11:
		focus1 = Vector2f(position.x - textureBox.width * 0.236f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.236f, position.y);
		ellipseSizeMultipliers[0] = { 1.62 };
		break;
	case 12:
		focus1 = Vector2f(position.x - textureBox.width * 0.223f, position.y - textureBox.height * 0.062f);
		focus2 = Vector2f(position.x + textureBox.width * 0.223f, position.y - textureBox.height * 0.062f);
		ellipseSizeMultipliers[0] = { 1.19 };
		break;
	case 13:
		focus1 = Vector2f(position.x - textureBox.width * 0.335f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.335f, position.y);
		ellipseSizeMultipliers[0] = { 1.43 };
		break;
	default:
		focus1 = Vector2f(position.x - textureBox.width * 0.111f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.111f, position.y);
		ellipseSizeMultipliers[0] = { 1.78 };
		break;
	}
	initMicroBlocks();
}

Vector2f Stump::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Stump::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Stump::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement body;
	body.size = Vector2f(conditionalSizeUnits);
	body.offset = Vector2f(textureBoxOffset);
	body.antiTransparent = true;

	animationLength = 1;
	if (typeOfObject >= 1 && typeOfObject <= 4)
		body.path = "Game/worldSprites/BirchGrove/stump" + std::to_string(typeOfObject) + ".png";
	if (typeOfObject >= 5 && typeOfObject <= 10)
		body.path = "Game/worldSprites/DarkWoods/stump" + std::to_string(typeOfObject - 4) + ".png";
	if (typeOfObject >= 11 && typeOfObject <= 13)
		body.path = "Game/worldSprites/SwampyTrees/stump" + std::to_string(typeOfObject - 10) + ".png";

	additionalSprites.push_back(body);

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