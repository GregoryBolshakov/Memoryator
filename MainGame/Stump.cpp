#include "Stump.h"
#include "Helper.h"

Stump::Stump(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 13; // BirchGrove: 1-4; DarkWoods: 5-10; SwampyTrees: 11-13
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "stump";
	setType(typeOfObject);
	tag = Tag::stump;
}

void Stump::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = { 412, 256 };
	if (typeOfObject == 2)
		conditionalSizeUnits = { 507, 337 };
	if (typeOfObject == 3)
		conditionalSizeUnits = { 647, 312 };
	if (typeOfObject == 4)
		conditionalSizeUnits = { 462, 269 };
	if (typeOfObject == 5)
		conditionalSizeUnits = { 243, 203 };
	if (typeOfObject == 6)
		conditionalSizeUnits = { 236, 315 };
	if (typeOfObject == 7)
		conditionalSizeUnits = { 286, 576 };
	if (typeOfObject == 8)
		conditionalSizeUnits = { 354, 570 };
	if (typeOfObject == 9)
		conditionalSizeUnits = { 335, 266 };
	if (typeOfObject == 10)
		conditionalSizeUnits = { 186, 412 };
	if (typeOfObject == 11)
		conditionalSizeUnits = { 250, 663 };
	if (typeOfObject == 12)
		conditionalSizeUnits = { 526, 441 };
	if (typeOfObject == 13)
		conditionalSizeUnits = { 267, 434 };
}

Vector2f Stump::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;

	switch (typeOfObject)
	{
	case 1:
		return { textureBox.width * 0.551f, textureBox.height * 0.645f };
	case 2:
		return { textureBox.width * 0.542f, textureBox.height * 0.64f };
	case 3:
		return { textureBox.width * 0.518f, textureBox.height * 0.446f };
	case 4:
		return { textureBox.width * 0.485f, textureBox.height * 0.591f };
	case 5:
		return { textureBox.width * 0.551f, textureBox.height * 0.771f };
	case 6:
		return { textureBox.width * 0.468f, textureBox.height * 0.796f };
	case 7:
		return { textureBox.width * 0.497f, textureBox.height * 0.859f };
	case 8:
		return { textureBox.width * 0.525f, textureBox.height * 0.889f };
	case 9:
		return { textureBox.width * 0.501f, textureBox.height * 0.728f };
	case 10:
		return { textureBox.width * 0.46f, textureBox.height * 0.83f };
	case 11:
		return { textureBox.width * 0.467f, textureBox.height * 0.868f };
	case 12:
		return { textureBox.width * 0.554f, textureBox.height * 0.758f };
	case 13:
		return { textureBox.width * 0.46f, textureBox.height * 0.83f };
	default:
		return { textureBox.width * 0.409f, textureBox.height * 0.945f };
	}	
}

void Stump::initPedestal()
{
	switch (typeOfObject)
	{
	case 1:
		focus1 = Vector2f(position.x - textureBox.width * 0.167f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.167f, position.y);
		ellipseSizeMultipliers[0] = { 1.4 };
		break;
	case 2:
		focus1 = Vector2f(position.x - textureBox.width * 0.353f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.353f, position.y);
		ellipseSizeMultipliers[0] = { 1.15 };
		break;
	case 3:
		focus1 = Vector2f(position.x - textureBox.width * 0.248f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.248f, position.y);
		ellipseSizeMultipliers[0] = { 1.25 };
		break;
	case 4:
		focus1 = Vector2f(position.x - textureBox.width * 0.311f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.311f, position.y);
		ellipseSizeMultipliers[0] = { 1.307 };
		break;
	case 5:
		focus1 = Vector2f(position.x - textureBox.width * 0.167f, position.y - textureBox.height * 0.091f);
		focus2 = Vector2f(position.x + textureBox.width * 0.167f, position.y - textureBox.height * 0.091f);
		ellipseSizeMultipliers[0] = { 1.64 };
		break;
	case 6:
		focus1 = Vector2f(position.x - textureBox.width * 0.327f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.327f, position.y);
		ellipseSizeMultipliers[0] = { 1.34 };
		break;
	case 7:
		focus1 = Vector2f(position.x - textureBox.width * 0.268f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.268f, position.y);
		ellipseSizeMultipliers[0] = { 1.388 };
		break;
	case 8:
		focus1 = Vector2f(position.x - textureBox.width * 0.142f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.142f, position.y);
		ellipseSizeMultipliers[0] = { 1.62 };
		break;
	case 9:
		focus1 = Vector2f(position.x - textureBox.width * 0.314f, position.y - textureBox.height * 0.07f);
		focus2 = Vector2f(position.x + textureBox.width * 0.314f, position.y - textureBox.height * 0.07f);
		ellipseSizeMultipliers[0] = { 1.24 };
		break;
	case 10:
		focus1 = Vector2f(position.x - textureBox.width * 0.335f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.335f, position.y);
		ellipseSizeMultipliers[0] = { 1.43};
		break;
	case 11:
		focus1 = Vector2f(position.x - textureBox.width * 0.236f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.236f, position.y);
		ellipseSizeMultipliers[0] = { 1.62 };
		break;
	case 12:
		focus1 = Vector2f(position.x - textureBox.width * 0.223f, position.y - textureBox.height * 0.062f);
		focus2 = Vector2f(position.x + textureBox.width * 0.223f, position.y - textureBox.height * 0.062f);
		ellipseSizeMultipliers[0] = { 1.19 };
		break;
	case 13:
		focus1 = Vector2f(position.x - textureBox.width * 0.335f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.335f, position.y);
		ellipseSizeMultipliers[0] = { 1.43 };
		break;
	default:
		focus1 = Vector2f(position.x - textureBox.width * 0.111f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.111f, position.y);
		ellipseSizeMultipliers[0] = { 1.78 };
		break;
	}
	initMicroBlocks();
}

Vector2f Stump::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Stump::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<SpriteChainElement*> Stump::prepareSprites(long long elapsedTime)
{
	SpriteChainElement* body = new SpriteChainElement(PackTag::darkWoods, PackPart::stump, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset), color, mirrored);

	if (typeOfObject >= 1 && typeOfObject <= 4)
		body->packTag = PackTag::birchGrove;
	if (typeOfObject >= 5 && typeOfObject <= 10)
	{
		body->packTag = PackTag::darkWoods;
		body->number -= 4;
	}
	if (typeOfObject >= 11 && typeOfObject <= 13)
	{
		body->packTag = PackTag::swampyTrees;
		body->number -= 10;
	}

	return { body };
}