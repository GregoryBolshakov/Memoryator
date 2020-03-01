#include "Yarrow.h"
#include "Helper.h"

Yarrow::Yarrow(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : PickedObject(std::move(objectName), centerPosition)
{
	varietyOfTypes = 5;
	this->typeOfObject = typeOfObject;
	toSaveName = "yarrow";
	Yarrow::setType(typeOfObject);
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
		conditionalSizeUnits = { 234, 358 };
	if (typeOfObject == 2)
		conditionalSizeUnits = { 294, 410 };
	if (typeOfObject == 3)
		conditionalSizeUnits = { 315, 351 };
	if (typeOfObject == 4)
		conditionalSizeUnits = { 320, 337 };
	if (typeOfObject == 5)
		conditionalSizeUnits = { 212, 531 };
}

Vector2f Yarrow::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2.0f, textureBox.height / 1.2f };
}

void Yarrow::initPedestal()
{
	focus1 = position;
	focus2 = position;
	ellipseSizeMultipliers[0] = { 0 };
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

std::vector<SpriteChainElement*> Yarrow::prepareSprites(long long elapsedTime)
{
    return {};
	/*additionalSprites.clear();
	spriteChainElement yarrowBody;
	yarrowBody.path = "Game/worldSprites/terrainObjects/yarrow/yarrow" + std::to_string(typeOfObject) + ".png";
	yarrowBody.size = Vector2f(conditionalSizeUnits);
	yarrowBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(yarrowBody);*/
}