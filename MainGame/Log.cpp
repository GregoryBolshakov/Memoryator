#include "Log.h"
#include "Helper.h"

Log::Log(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 3;
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "log";
	setType(typeOfObject);
	tag = Tag::log;
}

void Log::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(687, 179);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(507, 231);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(509, 246);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i(660, 312);
}

Vector2i Log::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	if (typeOfObject == 1)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.5));
	if (typeOfObject == 2)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.5));
	if (typeOfObject == 3)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.5));
	if (typeOfObject == 4)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.5));

	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.25));
}

void Log::initPedestal()
{
	focus1 = Vector2f(position.x - textureBox.width / 3, position.y);
	focus2 = Vector2f(position.x + textureBox.width / 3, position.y);
	ellipseSize = float((focus2.x - focus1.x) * 1.15);
	initMicroBlocks();
}

Vector2f Log::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Log::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Log::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement stumpBody;
	stumpBody.size = Vector2f(conditionalSizeUnits);
	stumpBody.offset = Vector2f(textureBoxOffset);

	animationLength = 1;
	stumpBody.path = "Game/worldSprites/BirchGrove/log" + std::to_string(typeOfObject) + ".png";

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