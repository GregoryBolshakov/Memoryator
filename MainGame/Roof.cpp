#include "roof.h"

#include "Helper.h"

roof::roof(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
{
	varietyOfTypes = 4;
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "roof";
	roof::setType(typeOfObject);
	tag = Tag::roof;
}

void roof::setType(const int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = { 248, 373 };
	if (typeOfObject == 2)
		conditionalSizeUnits = { 254, 555 };
	if (typeOfObject == 3)
		conditionalSizeUnits = { 255, 485 };
	if (typeOfObject == 4)
		conditionalSizeUnits = {239, 474};
}

Vector2f roof::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;

	switch (typeOfObject)
	{
	case 1:
		return { textureBox.width * 0.501f, textureBox.height * 0.993f };
	case 2:
		return { textureBox.width * 0.655f, textureBox.height * 0.934f };
	case 3:
		return { textureBox.width * 0.561f, textureBox.height * 0.96f };
	case 4:
		return { textureBox.width * 0.245f, textureBox.height * 0.944f };
	default:
		return { textureBox.width * 0.506f, textureBox.height * 0.632f };
	}
}

void roof::initPedestal()
{
	switch (typeOfObject)
	{
	case 1:
		focus1 = Vector2f(position.x - textureBox.width * 0.152f, position.y - textureBox.height * 0.021f);
		focus2 = Vector2f(position.x + textureBox.width * 0.152f, position.y - textureBox.height * 0.021f);
		ellipseSizeMultipliers[0] = { 1.5f };
		break;
	case 2:
		focus1 = Vector2f(position.x - textureBox.width * 0.143f, position.y - textureBox.height * 0.021f);
		focus2 = Vector2f(position.x + textureBox.width * 0.143f, position.y - textureBox.height * 0.021f);
		ellipseSizeMultipliers[0] = { 1.55f };
		break;
	case 3:
		focus1 = Vector2f(position.x - textureBox.width * 0.158f, position.y - textureBox.height * 0.037f);
		focus2 = Vector2f(position.x + textureBox.width * 0.158f, position.y - textureBox.height * 0.037f);
		ellipseSizeMultipliers[0] = { 1.62f };
		break;
	case 4:
		focus1 = Vector2f(position.x - textureBox.width * 0.176f, position.y - textureBox.height * 0.032f);
		focus2 = Vector2f(position.x + textureBox.width * 0.176f, position.y - textureBox.height * 0.032f);
		ellipseSizeMultipliers[0] = { 1.55f };
		break;
	default:
		focus1 = Vector2f(position.x - textureBox.width * 0.32f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.32f, position.y);
		ellipseSizeMultipliers[0] = { 1.3f };
		break;
	}
	initMicroBlocks();
}

Vector2f roof::getBuildPosition(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

int roof::getBuildType(Vector2f, Vector2f)
{
	return 1;
}

std::vector<sprite_chain_element*> roof::prepareSprites(long long)
{
	const auto body = new sprite_chain_element(PackTag::darkWoods, PackPart::roof, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset), color, mirrored);

	return { body };
}