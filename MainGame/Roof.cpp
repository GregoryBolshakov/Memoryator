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
		conditionalSizeUnits = Vector2i(360, 800);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(338, 684);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(368, 700);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i(360, 536);
}

Vector2i Roof::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.2));
}

void Roof::initPedestal()
{
	focus1 = Vector2f(position.x - textureBox.width / 2.5, position.y);
	focus2 = Vector2f(position.x + textureBox.width / 2.5, position.y);
	ellipseSize = float((focus2.x - focus1.x) * 1.1);
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