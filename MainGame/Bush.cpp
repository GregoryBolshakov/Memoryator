#include "Bush.h"
#include "Helper.h"

Bush::Bush(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 8;
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "bush";
	setType(typeOfObject);
	tag = Tag::bush;
}

void Bush::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(173, 254);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(152, 163);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(272, 429);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i(283, 259);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i(349, 284);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2i(363, 286);
	if (typeOfObject == 7)
		conditionalSizeUnits = Vector2i(231, 424);
	if (typeOfObject == 8)
		conditionalSizeUnits = Vector2i(405, 315);
}

Vector2i Bush::calculateTextureOffset()
{
	switch (typeOfObject)
	{
	case 1:
		return Vector2i(textureBox.width * 0.551f, int(textureBox.height * 0.01f));
	case 2:
		return Vector2i(textureBox.width * 0.59f, int(textureBox.height * 0.01f));
	case 3:
		return Vector2i(textureBox.width * 0.553f, int(textureBox.height * 0.908f));
	case 4:
		return Vector2i(textureBox.width * 0.49f, int(textureBox.height * 0.624f));
	case 5:
		return Vector2i(textureBox.width * 0.506f, int(textureBox.height * 0.632f));
	case 6:
		return Vector2i(textureBox.width * 0.495f, int(textureBox.height * 0.734f));
	default:
		return Vector2i(textureBox.width * 0.409f, int(textureBox.height * 0.945f));
	}
}

void Bush::initPedestal()
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
			focus1 = Vector2f(position.x - textureBox.width * 0.289f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.289f, position.y);
			ellipseSizeMultipliers[0] = { 1.29 };
			break;
		case 4:
			focus1 = Vector2f(position.x - textureBox.width * 0.311f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.311f, position.y);
			ellipseSizeMultipliers[0] = { 1.307 };
			break;
		case 5:
			focus1 = Vector2f(position.x - textureBox.width * 0.32f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.32f, position.y);
			ellipseSizeMultipliers[0] = { 1.3 };
			break;
		case 6:
			focus1 = Vector2f(position.x - textureBox.width * 0.319f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.319f, position.y);
			ellipseSizeMultipliers[0] = { 1.17 };
			break;
		default:
			focus1 = Vector2f(position.x - textureBox.width * 0.341f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.341f, position.y);
			ellipseSizeMultipliers[0] = { 1.18 };
			break;
	}
	initMicroBlocks();
}

Vector2f Bush::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Bush::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Bush::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement stumpBody;
	stumpBody.size = Vector2f(conditionalSizeUnits);
	stumpBody.offset = Vector2f(textureBoxOffset);

	animationLength = 1;
	stumpBody.path = "Game/worldSprites/BirchGrove/bush" + std::to_string(typeOfObject) + ".png";

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