#include "Rock.h"
#include "Helper.h"

Rock::Rock(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 18; // BirchGrove: 1-8; DarkWoods: 9-13; SwampyTrees: 14-18
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "rock";
	setType(typeOfObject);
	tag = Tag::rock;
}

void Rock::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(395, 231);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(579, 294);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i (343, 95);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i (236, 106);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i (269, 116);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2i (349, 113);
	if (typeOfObject == 7)
		conditionalSizeUnits = Vector2i (278, 204);
	if (typeOfObject == 8)
		conditionalSizeUnits = Vector2i (417, 113);	
	if (typeOfObject == 9)
		conditionalSizeUnits = Vector2i(428, 259);
	if (typeOfObject == 10)
		conditionalSizeUnits = Vector2i(474, 463);
	if (typeOfObject == 11)
		conditionalSizeUnits = Vector2i(314, 116);
	if (typeOfObject == 12)
		conditionalSizeUnits = Vector2i(371, 205);
	if (typeOfObject == 13)
		conditionalSizeUnits = Vector2i(259, 144);
	if (typeOfObject == 14)
		conditionalSizeUnits = Vector2i(278, 190);
	if (typeOfObject == 15)
		conditionalSizeUnits = Vector2i(300, 175);
	if (typeOfObject == 16)
		conditionalSizeUnits = Vector2i(173, 154);
	if (typeOfObject == 17)
		conditionalSizeUnits = Vector2i(406, 208);
	if (typeOfObject == 18)
		conditionalSizeUnits = Vector2i(254, 173);

	radius = std::max(conditionalSizeUnits.x, conditionalSizeUnits.y) / 2;
}

Vector2i Rock::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	switch (typeOfObject)
	{
	case 1:
		return Vector2i(textureBox.width * 0.551f, int(textureBox.height * 0.01f));
	case 2:
		return Vector2i(textureBox.width * 0.59f, int(textureBox.height * 0.01f));
	case 3:
		return Vector2i(textureBox.width * 0.488f, int(textureBox.height * 0.664f));
	case 4:
		return Vector2i(textureBox.width * 0.49f, int(textureBox.height * 0.624f));
	case 5:
		return Vector2i(textureBox.width * 0.506f, int(textureBox.height * 0.632f));
	case 6:
		return Vector2i(textureBox.width * 0.539f, int(textureBox.height * 0.637f));
	case 7:
		return Vector2i(textureBox.width * 0.58f, int(textureBox.height * 0.764f));
	case 8:
		return Vector2i(textureBox.width * 0.5f, int(textureBox.height * 0.673f));
	case 9:
		return Vector2i(textureBox.width * 0.455f, int(textureBox.height * 0.68f));
	case 10:
		return Vector2i(textureBox.width * 0.512f, int(textureBox.height * 0.717f));
	case 11:
		return Vector2i(textureBox.width * 0.488f, int(textureBox.height * 0.772f));
	case 12:
		return Vector2i(textureBox.width * 0.478f, int(textureBox.height * 0.633f));
	case 13:
		return Vector2i(textureBox.width * 0.491f, int(textureBox.height * 0.569f));
	case 14:
		return Vector2i(textureBox.width * 0.5f, int(textureBox.height * 0.673f));
	case 15:
		return Vector2i(textureBox.width * 0.491f, int(textureBox.height * 0.634f));
	case 16:
		return Vector2i(textureBox.width * 0.515f, int(textureBox.height * 0.809f));
	case 17:
		return Vector2i(textureBox.width * 0.559f, int(textureBox.height * 0.68f));
	case 18:
		return Vector2i(textureBox.width * 0.478f, int(textureBox.height * 0.633f));
	default:
		return Vector2i(textureBox.width * 0.409f, int(textureBox.height * 0.945f));
	}
}

void Rock::initPedestal()
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
		focus1 = Vector2f(position.x - textureBox.width * 0.342f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.342f, position.y);
		ellipseSizeMultipliers[0] = { 1.16 };
		break;
	case 4:
		focus1 = Vector2f(position.x - textureBox.width * 0.311f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.311f, position.y);
		ellipseSizeMultipliers[0] = { 1.15 };
		break;
	case 5:
		focus1 = Vector2f(position.x - textureBox.width * 0.32f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.32f, position.y);
		ellipseSizeMultipliers[0] = { 1.3};
		break;
	case 6:
		focus1 = Vector2f(position.x - textureBox.width * 0.36f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.36f, position.y);
		ellipseSizeMultipliers[0] = { 1.1 };
		break;
	case 7:
		focus1 = Vector2f(position.x - textureBox.width * 0.293f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.293f, position.y);
		ellipseSizeMultipliers[0] = { 1.21 };
		break;
	case 8:
		focus1 = Vector2f(position.x - textureBox.width * 0.38f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.38f, position.y);
		ellipseSizeMultipliers[0] = { 1.11 };
		break;
	case 9:
		focus1 = Vector2f(position.x - textureBox.width * 0.315f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.315f, position.y);
		ellipseSizeMultipliers[0] = { 1.23 };
		break;
	case 10:
		focus1 = Vector2f(position.x - textureBox.width * 0.302f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.302f, position.y);
		ellipseSizeMultipliers[0] = { 1.26 };
		break;
	case 11:
		focus1 = Vector2f(position.x - textureBox.width * 0.229f, position.y - textureBox.height * 0.092f);
		focus2 = Vector2f(position.x + textureBox.width * 0.229f, position.y - textureBox.height * 0.092f);
		ellipseSizeMultipliers[0] = { 1.64 };
		break;
	case 12:
		focus1 = Vector2f(position.x - textureBox.width * 0.386f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.386f, position.y);
		ellipseSizeMultipliers[0] = { 1.15 };
		break;
	case 13:
		focus1 = Vector2f(position.x - textureBox.width * 0.273f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.273f, position.y);
		ellipseSizeMultipliers[0] = { 1.64 };
		break;
	case 14:
		focus1 = Vector2f(position.x - textureBox.width * 0.315f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.315f, position.y);
		ellipseSizeMultipliers[0] = { 1.23 };
		break;
	case 15:
		focus1 = Vector2f(position.x - textureBox.width * 0.354f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.354f, position.y);
		ellipseSizeMultipliers[0] = { 1.14 };
		break;
	case 16:
		focus1 = Vector2f(position.x - textureBox.width * 0.378f, position.y - textureBox.height * 0.101f);
		focus2 = Vector2f(position.x + textureBox.width * 0.378f, position.y - textureBox.height * 0.101f);
		ellipseSizeMultipliers[0] = { 1.3 };
		break;
	case 17:
		focus1 = Vector2f(position.x - textureBox.width * 0.386f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.386f, position.y);
		ellipseSizeMultipliers[0] = { 1.15 };
		break;
	case 18:
		focus1 = Vector2f(position.x - textureBox.width * 0.38f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.38f, position.y);
		ellipseSizeMultipliers[0] = { 1.08 };
		break;
	default:
		focus1 = Vector2f(position.x - textureBox.width * 0.111f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.111f, position.y);
		ellipseSizeMultipliers[0] = { 1.78 };
		break;
	}
	initMicroBlocks();
}

Vector2f Rock::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Rock::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<SpriteChainElement> Rock::prepareSprites(long long elapsedTime)
{
	SpriteChainElement body(PackTag::darkWoods, PackPart::rock, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset));

	if (typeOfObject >= 1 && typeOfObject <= 8)
		body.packTag = PackTag::birchGrove;
	if (typeOfObject >= 9 && typeOfObject <= 13)
	{
		body.packTag = PackTag::darkWoods;
		body.number -= 8;
	}
	if (typeOfObject >= 14 && typeOfObject <= 18)
	{
		body.packTag = PackTag::swampyTrees;
		body.number -= 13;
	}

	return { body };
}