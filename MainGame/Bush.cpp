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
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.5));
}

void Bush::initPedestal()
{
	focus1 = Vector2f(position.x - textureBox.width / 2, position.y);
	focus2 = Vector2f(position.x + textureBox.width / 3, position.y);
	ellipseSize = float((focus2.x - focus1.x) * 1.2);
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