#include "Roof.h"
#include "Helper.h"

Roof::Roof(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 4;
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "roof";
	setType(typeOfObject);
	tag = Tag::roof;
}

void Roof::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(248, 373);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(254, 555);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(255, 485);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i(239, 474);
}

Vector2i Roof::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	return Vector2i(textureBox.width * 0.293, int(textureBox.height * 0.914));
}

void Roof::initPedestal()
{
	focus1 = Vector2f(position.x - textureBox.width * 0.287, position.y);
	focus2 = Vector2f(position.x + textureBox.width * 0.287, position.y);
	ellipseSizeMultipliers[0] = 1.1;
	initMicroBlocks();
}

Vector2f Roof::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Roof::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Roof::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement stumpBody;
	stumpBody.size = Vector2f(conditionalSizeUnits);
	stumpBody.offset = Vector2f(textureBoxOffset);

	animationLength = 1;
	stumpBody.path = "Game/worldSprites/DarkWoods/roof" + std::to_string(typeOfObject) + ".png";

	additionalSprites.push_back(stumpBody);

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