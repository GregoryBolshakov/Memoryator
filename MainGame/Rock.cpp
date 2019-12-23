#include "Rock.h"
#include "Helper.h"

Rock::Rock(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 13;
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "rock";
	setType(typeOfObject);
	tag = Tag::rock;
}

void Rock::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i (343, 95);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i (236, 106);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i (269, 116);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i (349, 113);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i (278, 204);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2i (417, 113);
	if (typeOfObject == 7)
		conditionalSizeUnits = Vector2i(395, 231);
	if (typeOfObject == 8)
		conditionalSizeUnits = Vector2i(579, 294);
	if (typeOfObject == 9)
		conditionalSizeUnits = Vector2i(434, 266);
	if (typeOfObject == 10)
		conditionalSizeUnits = Vector2i(475, 437);
	if (typeOfObject == 11)
		conditionalSizeUnits = Vector2i(269, 125);
	if (typeOfObject == 12)
		conditionalSizeUnits = Vector2i(238, 139);
	if (typeOfObject == 13)
		conditionalSizeUnits = Vector2i(319, 311);

	radius = std::max(conditionalSizeUnits.x, conditionalSizeUnits.y) / 2;
}

Vector2i Rock::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	switch (typeOfObject)
	{
	case 1:
		return Vector2i(textureBox.width * 0.551f, int(textureBox.height * 0.01f));
	case 2:
		return Vector2i(textureBox.width * 0.59f, int(textureBox.height * 0.01f));
	case 3:
		return Vector2i(textureBox.width * 0.488f, int(textureBox.height * 0.671f));
	case 4:
		return Vector2i(textureBox.width * 0.49f, int(textureBox.height * 0.624f));
	case 5:
		return Vector2i(textureBox.width * 0.506f, int(textureBox.height * 0.632f));
	case 6:
		return Vector2i(textureBox.width * 0.59f, int(textureBox.height * 0.64f));
	case 7:
		return Vector2i(textureBox.width * 0.583f, int(textureBox.height * 0.813f));
	case 8:
		return Vector2i(textureBox.width * 0.512f, int(textureBox.height * 0.717f));
	case 9:
		return Vector2i(textureBox.width * 0.455f, int(textureBox.height * 0.68f));
	case 10:
		return Vector2i(textureBox.width * 0.512f, int(textureBox.height * 0.717f));
	case 11:
		return Vector2i(textureBox.width * 0.488f, int(textureBox.height * 0.68f));
	case 12:
		return Vector2i(textureBox.width * 0.478f, int(textureBox.height * 0.633f));
	case 13:
		return Vector2i(textureBox.width * 0.491f, int(textureBox.height * 0.569f));
	default:
		return Vector2i(textureBox.width * 0.409f, int(textureBox.height * 0.945f));
	}
}

void Rock::initPedestal()
{
	switch (typeOfObject)
	{
	case 1:
		focus1 = Vector2f(position.x, position.y);
		focus2 = Vector2f(position.x, position.y);
		ellipseSizeMultipliers[0] = { 0 };
		break;
	case 2:
		focus1 = Vector2f(position.x, position.y);
		focus2 = Vector2f(position.x, position.y);
		ellipseSizeMultipliers[0] = { 0 };
		break;
	case 3:
		focus1 = Vector2f(position.x - textureBox.width * 0.342f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.342f, position.y);
		ellipseSizeMultipliers[0] = { 1.64 };
		break;
	case 4:
		focus1 = Vector2f(position.x - textureBox.width * 0.311f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.311f, position.y);
		ellipseSizeMultipliers[0] = { 1.307 };
		break;
	case 5:
		focus1 = Vector2f(position.x - textureBox.width * 0.32f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.32f, position.y);
		ellipseSizeMultipliers[0] = { 1.52};
		break;
	case 6:
		focus1 = Vector2f(position.x - textureBox.width * 0.452f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.452f, position.y);
		ellipseSizeMultipliers[0] = { 1.07 };
		break;
	case 7:
		focus1 = Vector2f(position.x - textureBox.width * 0.341f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.341f, position.y);
		ellipseSizeMultipliers[0] = { 1.18 };
		break;
	case 8:
		focus1 = Vector2f(position.x - textureBox.width * 0.452f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.452f, position.y);
		ellipseSizeMultipliers[0] = { 1.07 };
		break;
	case 9:
		focus1 = Vector2f(position.x - textureBox.width * 0.315f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.315f, position.y);
		ellipseSizeMultipliers[0] = { 1.35 };
		break;
	case 10:
		focus1 = Vector2f(position.x - textureBox.width * 0.302f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.302f, position.y);
		ellipseSizeMultipliers[0] = { 1.307 };
		break;
	case 11:
		focus1 = Vector2f(position.x - textureBox.width * 0.229f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.229f, position.y);
		ellipseSizeMultipliers[0] = { 1.64 };
		break;
	case 12:
		focus1 = Vector2f(position.x - textureBox.width * 0.386f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.386f, position.y);
		ellipseSizeMultipliers[0] = { 1.15 };
		break;
	case 13:
		focus1 = Vector2f(position.x - textureBox.width * 0.273f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.273f, position.y);
		ellipseSizeMultipliers[0] = { 1.64 };
		break;
	default:
		focus1 = Vector2f(position.x - textureBox.width * 0.111f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.111f, position.y);
		ellipseSizeMultipliers[0] = { 1.78 };
		break;
	}
	ellipseSize = Helper::getDist(focus1, focus2) * ellipseSizeMultipliers[0];
	initMicroBlocks();
}

Vector2f Rock::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Rock::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Rock::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement rockBody;
	rockBody.size = Vector2f(conditionalSizeUnits);
	rockBody.offset = Vector2f(textureBoxOffset);
	animationLength = 1;

	if (typeOfObject >= 1 && typeOfObject <= 6)
		rockBody.path = "Game/worldSprites/BirchGrove/rock" + std::to_string(typeOfObject) + ".png";
	if (typeOfObject >= 7 && typeOfObject <= 8)
		rockBody.path = "Game/worldSprites/BirchGrove/hill" + std::to_string(typeOfObject - 6) + ".png";
	if (typeOfObject >= 9 && typeOfObject <= 13)
		rockBody.path = "Game/worldSprites/DarkWoods/rock" + std::to_string(typeOfObject - 8) + ".png";

	additionalSprites.push_back(rockBody);

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