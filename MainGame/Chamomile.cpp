#include "chamomile.h"

#include "helper.h"

chamomile::chamomile(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : picked_object(std::move(objectName), centerPosition)
{
	varietyOfTypes = 5;
	this->typeOfObject = typeOfObject;
	toSaveName = "chamomile";
	chamomile::setType(typeOfObject);
	tag = entity_tag::chamomile;

	count = 1;
	id = entity_tag::chamomile;	
}

void chamomile::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = { 297, 263 };
	if (typeOfObject == 2)
		conditionalSizeUnits = { 279, 281 };
	if (typeOfObject == 3)
		conditionalSizeUnits = { 313, 264 };
	if (typeOfObject == 4)
		conditionalSizeUnits = { 223, 267 };
	if (typeOfObject == 5)
		conditionalSizeUnits = { 203, 206 };
}

Vector2f chamomile::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2.0f, textureBox.height / 1.2f };
}

void chamomile::initPedestal()
{
	focus1 = position;
	focus2 = position;
	ellipseSizeMultipliers[0] = { 0 };
	initMicroBlocks();
}

Vector2f chamomile::getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int chamomile::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<sprite_chain_element*> chamomile::prepareSprites(long long elapsedTime)
{
    return {};
    /*additionalSprites.clear();
	spriteChainElement body;
	body->path = "Game/worldSprites/terrainObjects/chamomile/chamomile" + std::to_string(typeOfObject) + ".png";
	body->size = Vector2f(conditionalSizeUnits);
	body->offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(body);*/
}