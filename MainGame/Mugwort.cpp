#include "Mugwort.h"

#include "Helper.h"

Mugwort::Mugwort(std::string objectName, Vector2f centerPosition, int typeOfObject) : PickedObject(std::move(objectName), centerPosition)
{
	varietyOfTypes = 3;
	this->typeOfObject = typeOfObject;
	toSaveName = "mugwort";
	Mugwort::setType(typeOfObject);
	radius = 50;
	tag = Tag::mugwort;
	count = 1;
	id = Tag::yarrow;
}

void Mugwort::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = { 308, 371 };
	if (typeOfObject == 2)
		conditionalSizeUnits = { 303, 325 };
	if (typeOfObject == 3)
		conditionalSizeUnits = { 250, 271 };
}

Vector2f Mugwort::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2.0f, textureBox.height / 1.2f };
}

void Mugwort::initPedestal()
{
	focus1 = position;
	focus2 = position;
	ellipseSizeMultipliers[0] = { 0 };
	initMicroBlocks();
}

Vector2f Mugwort::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Mugwort::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<sprite_chain_element*> Mugwort::prepareSprites(long long elapsedTime)
{
    return {};
	/*additionalSprites.clear();
	spriteChainElement mugwortBody;
	mugwortBody.path = "Game/worldSprites/terrainObjects/mugwort/mugwort" + std::to_string(typeOfObject) + ".png";
	mugwortBody.size = Vector2f(conditionalSizeUnits);
	mugwortBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(mugwortBody);*/
}