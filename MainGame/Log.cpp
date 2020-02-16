#include "Log.h"
#include "Helper.h"

Log::Log(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 3;
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "log";
	setType(typeOfObject);
	tag = Tag::log;
}

void Log::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(687, 179);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(507, 231);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(509, 246);
}

Vector2i Log::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	switch (typeOfObject)
	{
	case 1:
		return Vector2i(textureBox.width * 0.492f, int(textureBox.height * 0.721f));
	case 2:
		return Vector2i(textureBox.width * 0.479f, int(textureBox.height * 0.735f));
	case 3:
		return Vector2i(textureBox.width * 0.502f, int(textureBox.height * 0.584f));
	default:
		return Vector2i(textureBox.width * 0.485f, int(textureBox.height * 0.591f));
	}
}

void Log::initPedestal()
{
	std::pair<Vector2f, Vector2f> microEllipse;
	switch (typeOfObject)
	{
	case 1:
		this->isMultiellipse = true;
		focus1 = Vector2f(position.x, position.y);
		focus2 = Vector2f(position.x, position.y);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.42, position.y + textureBox.height * 0.15);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.073, position.y - textureBox.height * 0.26);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.107, position.y - textureBox.height * 0.197);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.169, position.y - textureBox.height * 0.197);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.033, position.y - textureBox.height * 0.2);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.407, position.y + textureBox.height * 0.103);
		internalEllipses.push_back(microEllipse);
		ellipseSizeMultipliers = { 1.03, 1.11, 1.05 };
		break;
	case 2:
		focus1 = Vector2f(position.x - textureBox.width * 0.399f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.399f, position.y);
		ellipseSizeMultipliers[0] = { 1.07 };
		break;
	case 3:
		focus1 = Vector2f(position.x - textureBox.width * 0.396f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.396f, position.y);
		ellipseSizeMultipliers[0] = { 1.12 };
		break;
	default:
		focus1 = Vector2f(position.x - textureBox.width * 0.311f, position.y);
		focus2 = Vector2f(position.x + textureBox.width * 0.311f, position.y);
		ellipseSizeMultipliers[0] = { 1.307 };
		break;
	}
	initMicroBlocks();
}

Vector2f Log::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Log::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<SpriteChainElement> Log::prepareSprites(long long elapsedTime)
{
	SpriteChainElement body(PackTag::birchGrove, PackPart::log, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset), color, mirrored);	

	return { body };
}