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

	switch (typeOfObject)
	{
	case 1:
		return Vector2i(textureBox.width * 0.501f, int(textureBox.height * 0.993f));
	case 2:
		return Vector2i(textureBox.width * 0.655f, int(textureBox.height * 0.934f));
	case 3:
		return Vector2i(textureBox.width * 0.561f, int(textureBox.height * 0.96f));
	case 4:
		return Vector2i(textureBox.width * 0.245f, int(textureBox.height * 0.944f));
	default:
		return Vector2i(textureBox.width * 0.506f, int(textureBox.height * 0.632f));
	}
}

void Roof::initPedestal()
{
	switch (typeOfObject)
	{
	case 1:
		focus1 = Vector2f(position.x - textureBox.width * 0.152f, position.y - textureBox.height * 0.021f);
		focus2 = Vector2f(position.x + textureBox.width * 0.152f, position.y - textureBox.height * 0.021f);
		ellipseSizeMultipliers[0] = { 1.5 };
		break;
	case 2:
		focus1 = Vector2f(position.x - textureBox.width * 0.143f, position.y - textureBox.height * 0.021f);
		focus2 = Vector2f(position.x + textureBox.width * 0.143f, position.y - textureBox.height * 0.021f);
		ellipseSizeMultipliers[0] = { 1.55 };
		break;
	case 3:
		focus1 = Vector2f(position.x - textureBox.width * 0.158f, position.y - textureBox.height * 0.037f);
		focus2 = Vector2f(position.x + textureBox.width * 0.158f, position.y - textureBox.height * 0.037f);
		ellipseSizeMultipliers[0] = { 1.62 };
		break;
	case 4:
		focus1 = Vector2f(position.x - textureBox.width * 0.176f, position.y - textureBox.height * 0.032f);
		focus2 = Vector2f(position.x + textureBox.width * 0.176f, position.y - textureBox.height * 0.032f);
		ellipseSizeMultipliers[0] = { 1.55 };
		break;
	default:
		focus1 = Vector2f(position.x - textureBox.width * 0.32f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.32f, position.y);
		ellipseSizeMultipliers[0] = { 1.3 };
		break;
	}
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

std::vector<SpriteChainElement> Roof::prepareSprites(long long elapsedTime)
{
    return {};
	/*additionalSprites.clear();
	spriteChainElement body;
	body.size = Vector2f(conditionalSizeUnits);
	body.offset = Vector2f(textureBoxOffset);
	body.antiTransparent = true;

	animationLength = 1;
	body.path = "Game/worldSprites/DarkWoods/roof" + std::to_string(typeOfObject) + ".png";

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
	}*/
}