#include "fern.h"

#include "Helper.h"

fern::fern(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : picked_object(std::move(objectName), centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	toSaveName = "fern";
	fern::setType(typeOfObject);
	tag = Tag::fern;

	count = 1;
	id = Tag::fern;
}

void fern::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = { 330, 370 };
}

Vector2f fern::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2.0f, textureBox.height / 1.7f };
}

void fern::initPedestal()
{
	focus1 = position;
	focus2 = position;
	ellipseSizeMultipliers[0] = { 0 };
	initMicroBlocks();
}

Vector2f fern::getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

Vector2f fern::getEnterPosition() const
{
	return { position.x, position.y + 1 };
}

int fern::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<sprite_chain_element*> fern::prepareSprites(long long elapsedTime)
{
    return {};
	/*additionalSprites.clear();
	spriteChainElement body;
	body->path = "Game/worldSprites/terrainObjects/fern/fern" + std::to_string(typeOfObject) + ".png";
	body->size = Vector2f(conditionalSizeUnits);
	body->offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(body);*/
}