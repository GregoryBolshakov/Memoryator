#include "Lake.h"
#include "Helper.h"

Lake::Lake(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 4; // SwampyTrees: 1-4
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "lake";
	isMultiEllipse = true;
	setType(typeOfObject);
	tag = Tag::lake;
}

void Lake::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = { 1072, 476 };
	if (typeOfObject == 2)
		conditionalSizeUnits = { 482, 279 };
	if (typeOfObject == 3)
		conditionalSizeUnits = { 839, 315 };
	if (typeOfObject == 4)
		conditionalSizeUnits = { 473, 388 };
}

Vector2f Lake::calculateTextureOffset()
{
	switch (typeOfObject)
	{
	case 1:
		return { textureBox.width * 0.55f, textureBox.height * 0.382f };
	case 2:
		return { textureBox.width * 0.503f, textureBox.height * 0.504f };
	case 3:
		return { textureBox.width * 0.543f, textureBox.height * 0.0325f };
	case 4:
		return { textureBox.width * 0.473f, textureBox.height * -0.141f };
	default:
		return { textureBox.width * 0.409f, textureBox.height * 0.945f };
	}
}

void Lake::initPedestal()
{
	std::pair<Vector2f, Vector2f> microEllipse;
	switch (typeOfObject)
	{
	case 1:
		focus1 = Vector2f(position.x, position.y);
		focus2 = Vector2f(position.x, position.y);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.49, position.y + textureBox.height * 0.384);
		microEllipse.second = Vector2f(position.x - textureBox.width * 0.0158, position.y + textureBox.height * 0.384);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.184, position.y + textureBox.height * 0.269);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.096, position.y + textureBox.height * 0.269);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.102, position.y + textureBox.height * 0.428);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.0549, position.y + textureBox.height * 0.428);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.0222, position.y + textureBox.height * 0.159);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.352, position.y + textureBox.height * 0.159);
		internalEllipses.push_back(microEllipse);
		ellipseSizeMultipliers = { 1.06, 1.13, 1.26, 1.07, 1.16 };
		break;
	case 2:
		focus1 = Vector2f(position.x, position.y);
		focus2 = Vector2f(position.x, position.y);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.452, position.y - textureBox.height * 0.0536);
		microEllipse.second = Vector2f(position.x - textureBox.width * 0.117, position.y - textureBox.height * 0.0536);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.154, position.y + textureBox.height * 0.202);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.141, position.y + textureBox.height * 0.202);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.166, position.y + textureBox.height * 0.179);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.438, position.y + textureBox.height * 0.179);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.17, position.y - textureBox.height * 0.244);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.114, position.y - textureBox.height * 0.244);
		internalEllipses.push_back(microEllipse);
		ellipseSizeMultipliers = { 1.45, 1.37, 1.38, 1.39 };
		break;
	case 3:
		focus1 = Vector2f(position.x, position.y);
		focus2 = Vector2f(position.x, position.y);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.463, position.y + textureBox.height * 0.601);
		microEllipse.second = Vector2f(position.x - textureBox.width * 0.108, position.y + textureBox.height * 0.601);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.145, position.y + textureBox.height * 0.759);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.15, position.y + textureBox.height * 0.759);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.175, position.y + textureBox.height * 0.242);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.392, position.y + textureBox.height * 0.242);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.16, position.y + textureBox.height * 0.426);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.155, position.y + textureBox.height * 0.426);
		internalEllipses.push_back(microEllipse);
		ellipseSizeMultipliers = { 1.21, 1.11, 1.27, 1.3 };
		break;
	case 4:
		focus1 = Vector2f(position.x, position.y);
		focus2 = Vector2f(position.x, position.y);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.413, position.y + textureBox.height * 1.04);
		microEllipse.second = Vector2f(position.x - textureBox.width * 0.066, position.y + textureBox.height * 1.04);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.046, position.y + textureBox.height * 0.558);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.399, position.y + textureBox.height * 0.558);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.027, position.y + textureBox.height * 0.305);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.24, position.y + textureBox.height * 0.305);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.313, position.y + textureBox.height * 0.782);
		microEllipse.second = Vector2f(position.x - textureBox.width * 0.428, position.y + textureBox.height * 0.782);
		internalEllipses.push_back(microEllipse);
		ellipseSizeMultipliers = { 1.19, 1.21, 1.53, 1.04 };
		break;
	}
	initMicroBlocks();
}

Vector2f Lake::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Lake::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<SpriteChainElement*> Lake::prepareSprites(long long elapsedTime)
{
	std::vector<SpriteChainElement*> result = {};
	SpriteChainElement* body = new SpriteChainElement(PackTag::swampyTrees, PackPart::lake, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset));

	result.push_back(body);

	setUnscaled(result);

	return result ;
}