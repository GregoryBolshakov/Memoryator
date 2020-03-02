#include "yarrow.h"

#include "helper.h"

yarrow::yarrow(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : picked_object(std::move(objectName), centerPosition)
{
	varietyOfTypes = 5;
	this->typeOfObject = typeOfObject;
	toSaveName = "yarrow";
	yarrow::setType(typeOfObject);
	radius = 50;
	tag = Tag::yarrow;

	count = 1;
	id = Tag::yarrow;
}

void yarrow::setType(int typeOfObject)
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

Vector2f yarrow::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2.0f, textureBox.height / 1.2f };
}

void yarrow::initPedestal()
{
	focus1 = position;
	focus2 = position;
	ellipseSizeMultipliers[0] = { 0 };
	initMicroBlocks();
}

Vector2f yarrow::getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int yarrow::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<sprite_chain_element*> yarrow::prepareSprites(long long elapsedTime)
{
    return {};
	/*additionalSprites.clear();
	spriteChainElement yarrowBody;
	yarrowBody.path = "Game/worldSprites/terrainObjects/yarrow/yarrow" + std::to_string(typeOfObject) + ".png";
	yarrowBody.size = Vector2f(conditionalSizeUnits);
	yarrowBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(yarrowBody);*/
}