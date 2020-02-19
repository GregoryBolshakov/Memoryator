#include "Ground.h"
#include "Helper.h"

Ground::Ground(std::string objectName, Vector2f centerPosition, int typeOfObject) : StaticObject(objectName, centerPosition)
{
	varietyOfTypes = 3;
	this->typeOfObject = typeOfObject;
	isBackground = true;
	toSaveName = "ground";
	setType(typeOfObject);
	tag = Tag::ground;
}

void Ground::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = { 1000, 1000 };
	this->zCoord = typeOfObject * 10 + 1;
}

Vector2f Ground::calculateTextureOffset()
{
	return {0, 0};
}

Vector2f Ground::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Ground::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<SpriteChainElement*> Ground::prepareSprites(long long elapsedTime)
{
    SpriteChainElement* body = new SpriteChainElement(PackTag::darkWoods, PackPart::ground, Direction::DOWN, 1, position, conditionalSizeUnits, Vector2f(textureBoxOffset));
	
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