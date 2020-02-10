#include "Bush.h"
#include "Helper.h"

Bush::Bush(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 15; // BrichGrove: 1-8; SwampyTrees: 9-15
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "bush";
	setType(typeOfObject);
	tag = Tag::bush;
}

void Bush::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(173, 254);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(152, 163);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(272, 429);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i(283, 259);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i(349, 284);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2i(363, 286);
	if (typeOfObject == 7)
		conditionalSizeUnits = Vector2i(231, 424);
	if (typeOfObject == 8)
		conditionalSizeUnits = Vector2i(405, 315);
	if (typeOfObject == 9)
		conditionalSizeUnits = Vector2i(642, 311);
	if (typeOfObject == 10)
		conditionalSizeUnits = Vector2i(289, 279);
	if (typeOfObject == 11)
		conditionalSizeUnits = Vector2i(396, 409);
	if (typeOfObject == 12)
		conditionalSizeUnits = Vector2i(385, 285);
	if (typeOfObject == 13)
		conditionalSizeUnits = Vector2i(450, 283);
	if (typeOfObject == 14)
		conditionalSizeUnits = Vector2i(462, 290);
	if (typeOfObject == 15)
		conditionalSizeUnits = Vector2i(189, 188);
}

Vector2i Bush::calculateTextureOffset()
{
	switch (typeOfObject)
	{
	case 1:
		return Vector2i(textureBox.width * 0.551f, int(textureBox.height * 0.01f));
	case 2:
		return Vector2i(textureBox.width * 0.59f, int(textureBox.height * 0.01f));
	case 3:
		return Vector2i(textureBox.width * 0.5f, int(textureBox.height * 0.929f));
	case 4:
		return Vector2i(textureBox.width * 0.49f, int(textureBox.height * 0.624f));
	case 5:
		return Vector2i(textureBox.width * 0.506f, int(textureBox.height * 0.632f));
	case 6:
		return Vector2i(textureBox.width * 0.495f, int(textureBox.height * 0.734f));
	case 7:
		return Vector2i(textureBox.width * 0.551f, int(textureBox.height * 0.01f));
	case 8:
		return Vector2i(textureBox.width * 0.59f, int(textureBox.height * 0.01f));
	case 9:
		return Vector2i(textureBox.width * 0.553f, int(textureBox.height * 0.908f));
	case 10:
		return Vector2i(textureBox.width * 0.525f, int(textureBox.height * 0.9f));
	case 11:
		return Vector2i(textureBox.width * 0.578f, int(textureBox.height * 0.931f));
	case 12:
		return Vector2i(textureBox.width * 0.506f, int(textureBox.height * 0.853f));
	case 13:
		return Vector2i(textureBox.width * 0.542f, int(textureBox.height * 0.83f));
	case 14:
		return Vector2i(textureBox.width * 0.519f, int(textureBox.height * 0.863f));
	case 15:
		return Vector2i(textureBox.width * 0.495f, int(textureBox.height * 0.734f));
	default:
		return Vector2i(textureBox.width * 0.409f, int(textureBox.height * 0.945f));
	}
}

void Bush::initPedestal()
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
			focus1 = Vector2f(position.x - textureBox.width * 0.224f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.224f, position.y);
			ellipseSizeMultipliers[0] = { 1.29 };
			break;
		case 4:
			focus1 = Vector2f(position.x - textureBox.width * 0.311f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.311f, position.y);
			ellipseSizeMultipliers[0] = { 1.307 };
			break;
		case 5:
			focus1 = Vector2f(position.x - textureBox.width * 0.32f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.32f, position.y);
			ellipseSizeMultipliers[0] = { 1.3 };
			break;
		case 6:
			focus1 = Vector2f(position.x - textureBox.width * 0.319f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.319f, position.y);
			ellipseSizeMultipliers[0] = { 1.17 };
			break;
		case 7:
			focus1 = Vector2f(position.x, position.y);
			focus2 = Vector2f(position.x, position.y);
			ellipseSizeMultipliers[0] = { 0 };
			break;
		case 8:
			focus1 = Vector2f(position.x, position.y);
			focus2 = Vector2f(position.x, position.y);
			ellipseSizeMultipliers[0] = { 0 };
			break;
		case 9:
			focus1 = Vector2f(position.x - textureBox.width * 0.119f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.119f, position.y);
			ellipseSizeMultipliers[0] = { 1.29 };
			break;
		case 10:
			focus1 = Vector2f(position.x - textureBox.width * 0.196f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.196f, position.y);
			ellipseSizeMultipliers[0] = { 1.31 };
			break;
		case 11:
			focus1 = Vector2f(position.x - textureBox.width * 0.123f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.123f, position.y);
			ellipseSizeMultipliers[0] = { 1.26 };
			break;
		case 12:
			focus1 = Vector2f(position.x - textureBox.width * 0.174f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.174f, position.y);
			ellipseSizeMultipliers[0] = { 1.18 };
			break;
		case 13:
			focus1 = Vector2f(position.x - textureBox.width * 0.165f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.165f, position.y);
			ellipseSizeMultipliers[0] = { 1.24 };
			break;
		case 14:
			focus1 = Vector2f(position.x - textureBox.width * 0.238f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.238f, position.y);
			ellipseSizeMultipliers[0] = { 1.09 };
			break;
		case 15:
			focus1 = Vector2f(position.x - textureBox.width * 0.289f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.289f, position.y);
			ellipseSizeMultipliers[0] = { 1.29 };
			break;
		default:
			focus1 = Vector2f(position.x - textureBox.width * 0.341f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.341f, position.y);
			ellipseSizeMultipliers[0] = { 1.18 };
			break;
	}
	initMicroBlocks();
}

Vector2f Bush::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Bush::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<SpriteChainElement> Bush::prepareSprites(long long elapsedTime)
{
	SpriteChainElement body(PackTag::darkWoods, PackPart::bush, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset));

	if (typeOfObject >= 1 && typeOfObject <= 8)
		body.packTag = PackTag::birchGrove;
	if (typeOfObject >= 9 && typeOfObject <= 15)
	{
		body.packTag = PackTag::swampyTrees;
		body.number -= 8;
	}

	return { body };
}