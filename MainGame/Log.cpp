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
}

Vector2i Log::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	switch (typeOfObject)
	{
	case 1:
		return Vector2i(textureBox.width * 0.492f, int(textureBox.height * 0.726f));
	case 2:
		return Vector2i(textureBox.width * 0.479f, int(textureBox.height * 0.735f));
	case 3:
		return Vector2i(textureBox.width * 0.502f, int(textureBox.height * 0.584f));
	default:
		return Vector2i(textureBox.width * 0.485f, int(textureBox.height * 0.591f));
	}
}

void Log::initPedestal()
{
	switch (typeOfObject)
	{
	case 1:
		focus1 = Vector2f(position.x - textureBox.width * 0.383f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.383f, position.y);
		ellipseSizeMultipliers[0] = { 1.04 };
		break;
	case 2:
		focus1 = Vector2f(position.x - textureBox.width * 0.399f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.399f, position.y);
		ellipseSizeMultipliers[0] = { 1.07 };
		break;
	case 3:
		focus1 = Vector2f(position.x - textureBox.width * 0.396f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.396f, position.y);
		ellipseSizeMultipliers[0] = { 1.12 };
		break;
	default:
		focus1 = Vector2f(position.x - textureBox.width * 0.311f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.311f, position.y);
		ellipseSizeMultipliers[0] = { 1.307 };
		break;
	}
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