#include "Yarrow.h"
#include "Helper.h"

Yarrow::Yarrow(std::string objectName, Vector2f centerPosition, int typeOfObject) : PickedObject(objectName, centerPosition)
{
	varietyOfTypes = 5;
	this->typeOfObject = typeOfObject;
	toSaveName = "yarrow";
	setType(typeOfObject);
	radius = 50;
	tag = Tag::yarrow;

	count = 1;
	id = Tag::yarrow;
}

void Yarrow::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(234, 358);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(294, 410);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(315, 351);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i(320, 337);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i(212, 531);
}

Vector2i Yarrow::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i (textureBox.width / 2, int(textureBox.height / 1.2));
}

void Yarrow::initPedestal()
{
	focus1 = position;
	focus2 = position;
	ellipseSizeMultipliers[0] = { 0 };
	ellipseSize = 0;
	initMicroBlocks();
}

Vector2f Yarrow::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Yarrow::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Yarrow::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement yarrowBody;
	yarrowBody.path = "Game/worldSprites/terrainObjects/yarrow/yarrow" + std::to_string(typeOfObject) + ".png";
	yarrowBody.size = Vector2f(conditionalSizeUnits);
	yarrowBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(yarrowBody);
}