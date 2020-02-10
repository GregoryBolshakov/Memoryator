#include "Root.h"
#include "Helper.h"

Root::Root(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 5; // 1-5
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "root";
	setType(typeOfObject);
	tag = Tag::root;
}

void Root::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(544, 138);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(468, 208);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(333, 228);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i(406, 137);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i(339, 132);
}

Vector2i Root::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	switch (typeOfObject)
	{	
	case 1:
		return Vector2i(textureBox.width * 0.698f, int(textureBox.height * 0.47f));
	case 2:
		return Vector2i(textureBox.width * 0.313f, int(textureBox.height * 0.671f));
	case 3:
		return Vector2i(textureBox.width * 0.429f, int(textureBox.height * 0.508f));
	case 4:
		return Vector2i(textureBox.width * 0.485f, int(textureBox.height * 0.01f));
	case 5:
		return Vector2i(textureBox.width * 0.551f, int(textureBox.height * 0.01f));
	default:
		return Vector2i(textureBox.width * 0.409f, int(textureBox.height * 0.01f));
	}
}

void Root::initPedestal()
{
	switch (typeOfObject)
	{
	case 1:
		focus1 = Vector2f(position.x - textureBox.width * 0.229f, position.y - textureBox.height * 0.151f);
		focus2 = Vector2f(position.x + textureBox.width * 0.229f, position.y - textureBox.height * 0.151f);
		ellipseSizeMultipliers[0] = { 1.06 };
		break;
	case 2:
		focus1 = Vector2f(position.x - textureBox.width * 0.288f, position.y - textureBox.height * 0.09f);
		focus2 = Vector2f(position.x + textureBox.width * 0.288f, position.y - textureBox.height * 0.09f);
		ellipseSizeMultipliers[0] = { 1.07 };
		break;
	case 3:
		focus1 = Vector2f(position.x - textureBox.width * 0.367f, position.y - textureBox.height * 0.065f);
		focus2 = Vector2f(position.x + textureBox.width * 0.367f, position.y - textureBox.height * 0.065f);
		ellipseSizeMultipliers[0] = { 1.17 };
		break;
	case 4:
		focus1 = Vector2f(position.x - textureBox.width * 0.311f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.311f, position.y);
		ellipseSizeMultipliers[0] = { 0 };
		break;
	case 5:
		focus1 = Vector2f(position.x - textureBox.width * 0.167f, position.y - textureBox.height * 0.091f);
		focus2 = Vector2f(position.x + textureBox.width * 0.167f, position.y - textureBox.height * 0.091f);
		ellipseSizeMultipliers[0] = { 0 };
		break;
	default:
		focus1 = Vector2f(position.x - textureBox.width * 0.111f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.111f, position.y);
		ellipseSizeMultipliers[0] = { 0 };
		break;
	}
	initMicroBlocks();
}

Vector2f Root::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Root::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<SpriteChainElement> Root::prepareSprites(long long elapsedTime)
{
	SpriteChainElement body(PackTag::swampyTrees, PackPart::root, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset));

	return { body };
}