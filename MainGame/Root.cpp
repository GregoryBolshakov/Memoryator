#include "Root.h"

#include "Helper.h"

Root::Root(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : TerrainObject(std::move(objectName), centerPosition)
{
	varietyOfTypes = 5; // 1-5
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "root";
	Root::setType(typeOfObject);
	tag = Tag::root;
}

void Root::setType(const int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = { 544, 138 };
	if (typeOfObject == 2)
		conditionalSizeUnits = { 468, 208 };
	if (typeOfObject == 3)
		conditionalSizeUnits = { 333, 228 };
	if (typeOfObject == 4)
		conditionalSizeUnits = { 406, 137 };
	if (typeOfObject == 5)
		conditionalSizeUnits = { 339, 132 };
}

Vector2f Root::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;

	switch (typeOfObject)
	{	
	case 1:
		return { textureBox.width * 0.698f, textureBox.height * 0.47f };
	case 2:
		return { textureBox.width * 0.313f, textureBox.height * 0.671f };
	case 3:
		return { textureBox.width * 0.429f, textureBox.height * 0.508f };
	case 4:
		return { textureBox.width * 0.485f, textureBox.height * 0.01f };
	case 5:
		return { textureBox.width * 0.551f, textureBox.height * 0.01f };
	default:
		return { textureBox.width * 0.409f, textureBox.height * 0.01f };
	}
}

void Root::initPedestal()
{
	switch (typeOfObject)
	{
	case 1:
		focus1 = Vector2f(position.x - textureBox.width * 0.229f, position.y - textureBox.height * 0.151f);
		focus2 = Vector2f(position.x + textureBox.width * 0.229f, position.y - textureBox.height * 0.151f);
		ellipseSizeMultipliers[0] = { 1.06f };
		break;
	case 2:
		focus1 = Vector2f(position.x - textureBox.width * 0.288f, position.y - textureBox.height * 0.09f);
		focus2 = Vector2f(position.x + textureBox.width * 0.288f, position.y - textureBox.height * 0.09f);
		ellipseSizeMultipliers[0] = { 1.07f };
		break;
	case 3:
		focus1 = Vector2f(position.x - textureBox.width * 0.367f, position.y - textureBox.height * 0.065f);
		focus2 = Vector2f(position.x + textureBox.width * 0.367f, position.y - textureBox.height * 0.065f);
		ellipseSizeMultipliers[0] = { 1.17f };
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

Vector2f Root::getBuildPosition(std::vector<WorldObject*>, float, Vector2f)
{
	return { -1, -1 };
}

int Root::getBuildType(Vector2f, Vector2f)
{
	return 1;
}

std::vector<SpriteChainElement*> Root::prepareSprites(long long)
{
	const auto body = new SpriteChainElement(PackTag::swampyTrees, PackPart::root, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset), color, mirrored);

	return { body };
}