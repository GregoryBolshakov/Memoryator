#include "lake.h"

#include "Helper.h"

lake::lake(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
{
	varietyOfTypes = 4; // SwampyTrees: 1-4
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "lake";
	isMultiEllipse = true;
	lake::setType(typeOfObject);
	tag = Tag::lake;
}

void lake::setType(const int typeOfObject)
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

Vector2f lake::calculateTextureOffset()
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

void lake::initPedestal()
{
	std::pair<Vector2f, Vector2f> microEllipse;
	switch (typeOfObject)
	{
	case 1:
		focus1 = Vector2f(position.x, position.y);
		focus2 = Vector2f(position.x, position.y);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.49f, position.y + textureBox.height * 0.384f);
		microEllipse.second = Vector2f(position.x - textureBox.width * 0.0158f, position.y + textureBox.height * 0.384f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.184f, position.y + textureBox.height * 0.269f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.096f, position.y + textureBox.height * 0.269f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.102f, position.y + textureBox.height * 0.428f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.0549f, position.y + textureBox.height * 0.428f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.0222f, position.y + textureBox.height * 0.159f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.352f, position.y + textureBox.height * 0.159f);
		internalEllipses.push_back(microEllipse);
		ellipseSizeMultipliers = { 1.06f, 1.13f, 1.26f, 1.07f, 1.16f };
		break;
	case 2:
		focus1 = Vector2f(position.x, position.y);
		focus2 = Vector2f(position.x, position.y);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.452f, position.y - textureBox.height * 0.0536f);
		microEllipse.second = Vector2f(position.x - textureBox.width * 0.117f, position.y - textureBox.height * 0.0536f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.154f, position.y + textureBox.height * 0.202f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.141f, position.y + textureBox.height * 0.202f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.166f, position.y + textureBox.height * 0.179f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.438f, position.y + textureBox.height * 0.179f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.17f, position.y - textureBox.height * 0.244f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.114f, position.y - textureBox.height * 0.244f);
		internalEllipses.push_back(microEllipse);
		ellipseSizeMultipliers = { 1.45f, 1.37f, 1.38f, 1.39f };
		break;
	case 3:
		focus1 = Vector2f(position.x, position.y);
		focus2 = Vector2f(position.x, position.y);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.463f, position.y + textureBox.height * 0.601f);
		microEllipse.second = Vector2f(position.x - textureBox.width * 0.108f, position.y + textureBox.height * 0.601f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.145f, position.y + textureBox.height * 0.759f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.15f, position.y + textureBox.height * 0.759f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.175f, position.y + textureBox.height * 0.242f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.392f, position.y + textureBox.height * 0.242f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.16f, position.y + textureBox.height * 0.426f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.155f, position.y + textureBox.height * 0.426f);
		internalEllipses.push_back(microEllipse);
		ellipseSizeMultipliers = { 1.21f, 1.11f, 1.27f, 1.3f };
		break;
	case 4:
		focus1 = Vector2f(position.x, position.y);
		focus2 = Vector2f(position.x, position.y);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.413f, position.y + textureBox.height * 1.04f);
		microEllipse.second = Vector2f(position.x - textureBox.width * 0.066f, position.y + textureBox.height * 1.04f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x - textureBox.width * 0.046f, position.y + textureBox.height * 0.558f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.399f, position.y + textureBox.height * 0.558f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.027f, position.y + textureBox.height * 0.305f);
		microEllipse.second = Vector2f(position.x + textureBox.width * 0.24f, position.y + textureBox.height * 0.305f);
		internalEllipses.push_back(microEllipse);

		microEllipse.first = Vector2f(position.x + textureBox.width * 0.313f, position.y + textureBox.height * 0.782f);
		microEllipse.second = Vector2f(position.x - textureBox.width * 0.428f, position.y + textureBox.height * 0.782f);
		internalEllipses.push_back(microEllipse);
		ellipseSizeMultipliers = { 1.19f, 1.21f, 1.53f, 1.04f };
		break;
	}
	initMicroBlocks();
}

Vector2f lake::getBuildPosition(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

int lake::getBuildType(Vector2f, Vector2f)
{
	return 1;
}

std::vector<sprite_chain_element*> lake::prepareSprites(long long)
{
	std::vector<sprite_chain_element*> result = {};
	const auto body = new sprite_chain_element(PackTag::swampyTrees, PackPart::lake, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset));

	result.push_back(body);

	setUnscaled(result);

	return result ;
}