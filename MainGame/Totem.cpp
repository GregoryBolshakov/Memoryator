#include "totem.h"

#include "Helper.h"


totem::totem(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : TerrainObject(std::move(objectName), centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 20;
	toSaveName = "totem";
	totem::setType(typeOfObject);
	tag = Tag::totem;
}

void totem::setType(const int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = { 154, 544 };
	if (typeOfObject == 2)
		conditionalSizeUnits = { 154, 544 };
	if (typeOfObject == 3)
		conditionalSizeUnits = { 568, 292 };
	if (typeOfObject == 4)
		conditionalSizeUnits = { 336, 240 };
	if (typeOfObject == 5)
		conditionalSizeUnits = { 280, 212 };
	if (typeOfObject == 6)
		conditionalSizeUnits = { 272, 196 };

	radius = std::max(conditionalSizeUnits.x, conditionalSizeUnits.y) / 2;
}

Vector2f totem::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;

	if (typeOfObject == 1)
		return { textureBox.width / 2.0f, textureBox.height / 1.15f };
	if (typeOfObject == 2)
		return { textureBox.width / 2.0f, textureBox.height / 1.15f };
	if (typeOfObject == 3)
		return { textureBox.width / 2.0f, textureBox.height / 1.5f };
	if (typeOfObject == 4)
		return { textureBox.width / 2.0f, textureBox.height / 1.5f };
	if (typeOfObject == 5)
		return { textureBox.width / 2.0f, textureBox.height / 1.5f };
	if (typeOfObject == 6)
		return { textureBox.width / 2.0f, textureBox.height / 1.25f };

	throw std::logic_error("Invalid type of object.");
}

void totem::initPedestal()
{
	//if (typeOfObject == 1)
	{
		focus1 = Vector2f(position.x - textureBox.width / 4, position.y);
		focus2 = Vector2f(position.x + textureBox.width / 4, position.y);
		ellipseSizeMultipliers[0] = { 1.8f };
	}	
	initMicroBlocks();
}

Vector2f totem::getBuildPosition(std::vector<WorldObject*>, float, Vector2f)
{
	return { -1, -1 };
}

int totem::getBuildType(Vector2f, Vector2f)
{
	return 1;
}

std::vector<sprite_chain_element*> totem::prepareSprites(long long)
{
	const auto body = new sprite_chain_element(PackTag::craftObjects, PackPart::totem, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset), color, mirrored);

	return { body };
}