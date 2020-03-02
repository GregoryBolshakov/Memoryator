#include "bush.h"

#include "Helper.h"

bush::bush(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(std::move(objectName), centerPosition)
{
	varietyOfTypes = 15; // BrichGrove: 1-8; SwampyTrees: 9-15
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "bush";
	bush::setType(typeOfObject);
	tag = Tag::bush;
}

void bush::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = { 173, 254 };
	if (typeOfObject == 2)
		conditionalSizeUnits = { 152, 163 };
	if (typeOfObject == 3)
		conditionalSizeUnits = { 272, 429 };
	if (typeOfObject == 4)
		conditionalSizeUnits = { 283, 259 };
	if (typeOfObject == 5)
		conditionalSizeUnits = { 349, 284 };
	if (typeOfObject == 6)
		conditionalSizeUnits = { 363, 286 };
	if (typeOfObject == 7)
		conditionalSizeUnits = { 231, 424 };
	if (typeOfObject == 8)
		conditionalSizeUnits = { 405, 315 };
	if (typeOfObject == 9)
		conditionalSizeUnits = { 642, 311 };
	if (typeOfObject == 10)
		conditionalSizeUnits = { 289, 279 };
	if (typeOfObject == 11)
		conditionalSizeUnits = { 396, 409 };
	if (typeOfObject == 12)
		conditionalSizeUnits = { 385, 285 };
	if (typeOfObject == 13)
		conditionalSizeUnits = { 450, 283 };
	if (typeOfObject == 14)
		conditionalSizeUnits = { 462, 290 };
	if (typeOfObject == 15)
		conditionalSizeUnits = { 189, 188 };
}

Vector2f bush::calculateTextureOffset()
{
	switch (typeOfObject)
	{
	case 1:
		return { textureBox.width * 0.551f, textureBox.height * 0.01f };
	case 2:
		return { textureBox.width * 0.59f, textureBox.height * 0.01f };
	case 3:
		return { textureBox.width * 0.5f, textureBox.height * 0.929f };
	case 4:
		return { textureBox.width * 0.49f, textureBox.height * 0.624f };
	case 5:
		return { textureBox.width * 0.506f, textureBox.height * 0.632f };
	case 6:
		return { textureBox.width * 0.495f, textureBox.height * 0.734f };
	case 7:
		return { textureBox.width * 0.551f, textureBox.height * 0.01f };
	case 8:
		return { textureBox.width * 0.59f, textureBox.height * 0.01f };
	case 9:
		return { textureBox.width * 0.553f, textureBox.height * 0.908f };
	case 10:
		return { textureBox.width * 0.525f, textureBox.height * 0.9f };
	case 11:
		return { textureBox.width * 0.578f, textureBox.height * 0.931f };
	case 12:
		return { textureBox.width * 0.506f, textureBox.height * 0.853f };
	case 13:
		return { textureBox.width * 0.542f, textureBox.height * 0.83f };
	case 14:
		return { textureBox.width * 0.519f, textureBox.height * 0.863f };
	case 15:
		return { textureBox.width * 0.495f, textureBox.height * 0.734f };
	default:
		return { textureBox.width * 0.409f, textureBox.height * 0.945f };
	}
}

void bush::initPedestal()
{
	switch (typeOfObject)
	{
		case 1:
		case 2:
			focus1 = Vector2f(position.x, position.y);
			focus2 = Vector2f(position.x, position.y);
			ellipseSizeMultipliers[0] = { 0 };
			break;
		case 3:
			focus1 = Vector2f(position.x - textureBox.width * 0.224f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.224f, position.y);
			ellipseSizeMultipliers[0] = { 1.29f };
			break;
		case 4:
			focus1 = Vector2f(position.x - textureBox.width * 0.311f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.311f, position.y);
			ellipseSizeMultipliers[0] = { 1.307f };
			break;
		case 5:
			focus1 = Vector2f(position.x - textureBox.width * 0.32f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.32f, position.y);
			ellipseSizeMultipliers[0] = { 1.3f };
			break;
		case 6:
			focus1 = Vector2f(position.x - textureBox.width * 0.319f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.319f, position.y);
			ellipseSizeMultipliers[0] = { 1.17f };
			break;
		case 7:
		case 8:
			focus1 = Vector2f(position.x, position.y);
			focus2 = Vector2f(position.x, position.y);
			ellipseSizeMultipliers[0] = { 0 };
			break;
		case 9:
			focus1 = Vector2f(position.x - textureBox.width * 0.119f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.119f, position.y);
			ellipseSizeMultipliers[0] = { 1.29f };
			break;
		case 10:
			focus1 = Vector2f(position.x - textureBox.width * 0.196f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.196f, position.y);
			ellipseSizeMultipliers[0] = { 1.31f };
			break;
		case 11:
			focus1 = Vector2f(position.x - textureBox.width * 0.123f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.123f, position.y);
			ellipseSizeMultipliers[0] = { 1.26f };
			break;
		case 12:
			focus1 = Vector2f(position.x - textureBox.width * 0.174f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.174f, position.y);
			ellipseSizeMultipliers[0] = { 1.18f };
			break;
		case 13:
			focus1 = Vector2f(position.x - textureBox.width * 0.165f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.165f, position.y);
			ellipseSizeMultipliers[0] = { 1.24f };
			break;
		case 14:
			focus1 = Vector2f(position.x - textureBox.width * 0.238f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.238f, position.y);
			ellipseSizeMultipliers[0] = { 1.09f };
			break;
		case 15:
			focus1 = Vector2f(position.x - textureBox.width * 0.289f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.289f, position.y);
			ellipseSizeMultipliers[0] = { 1.29f };
			break;
		default:
			focus1 = Vector2f(position.x - textureBox.width * 0.341f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.341f, position.y);
			ellipseSizeMultipliers[0] = { 1.18f };
			break;
	}
	initMicroBlocks();
}

Vector2f bush::getBuildPosition(std::vector<WorldObject*>, float, Vector2f)
{
	return { -1, -1 };
}

int bush::getBuildType(Vector2f, Vector2f)
{
	return 1;
}

std::vector<sprite_chain_element*> bush::prepareSprites(long long)
{
	auto body = new sprite_chain_element(PackTag::darkWoods, PackPart::bush, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset), color, mirrored);

	if (typeOfObject >= 1 && typeOfObject <= 8)
		body->packTag = PackTag::birchGrove;
	if (typeOfObject >= 9 && typeOfObject <= 15)
	{
		body->packTag = PackTag::swampyTrees;
		body->number -= 8;
	}

	return { body };
}