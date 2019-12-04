#include "Rock.h"
#include "Helper.h"

Rock::Rock(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 6;
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
		conditionalSizeUnits = Vector2i(434, 266);
	if (typeOfObject == 8)
		conditionalSizeUnits = Vector2i(475, 437);
	if (typeOfObject == 9)
		conditionalSizeUnits = Vector2i(269, 125);
	if (typeOfObject == 10)
		conditionalSizeUnits = Vector2i(238, 139);
	if (typeOfObject == 11)
		conditionalSizeUnits = Vector2i(319, 311);

	radius = std::max(conditionalSizeUnits.x, conditionalSizeUnits.y) / 2;
}

Vector2i Rock::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i (textureBox.width / 2, int(textureBox.height / 1.5));
}

void Rock::initPedestal()
{
	focus1 = Vector2f (position.x - textureBox.width / 4, position.y);
	focus2 = Vector2f (position.x + textureBox.width / 4, position.y);
	ellipseSize = float((focus2.x - focus1.x) * 1.6);
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
	if (typeOfObject >= 7 && typeOfObject <= 11)
		rockBody.path = "Game/worldSprites/DarkWoods/rock" + std::to_string(typeOfObject - 6) + ".png";

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