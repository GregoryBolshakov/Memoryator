#include "ground.h"

#include "helper.h"

ground::ground(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : static_object(std::move(objectName), centerPosition)
{
	varietyOfTypes = 3;
	this->typeOfObject = typeOfObject;
	isBackground = true;
	toSaveName = "ground";
	ground::setType(typeOfObject);
	tag = Tag::ground;
}

void ground::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = { 1000, 1000 };
	this->zCoord = typeOfObject * 10 + 1;
}

Vector2f ground::calculateTextureOffset()
{
	return {0, 0};
}

Vector2f ground::getBuildPosition(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

int ground::getBuildType(Vector2f, Vector2f)
{
	return 1;
}

std::vector<sprite_chain_element*> ground::prepareSprites(long long)
{
	auto body = new sprite_chain_element(PackTag::darkWoods, PackPart::ground, Direction::DOWN, 1, position, conditionalSizeUnits, Vector2f(textureBoxOffset));
	
	body->zCoord = zCoord;
	body->isBackground = true;

	if (typeOfObject == 1)
		body->packTag = PackTag::swampyTrees;
	if (typeOfObject == 2)
		body->packTag = PackTag::darkWoods;
	if (typeOfObject == 3)
		body->packTag = PackTag::birchGrove;

    return { body };
}