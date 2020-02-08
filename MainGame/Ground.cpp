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
	this->conditionalSizeUnits = Vector2i (1000, 1000);
	this->zCoord = typeOfObject * 10 + 5;
}

Vector2i Ground::calculateTextureOffset()
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

std::vector<SpriteChainElement> Ground::prepareSprites(long long elapsedTime)
{
    SpriteChainElement body(PackTag::darkWoods, PackPart::ground, Direction::DOWN, 1, position, conditionalSizeUnits, Vector2f(textureBoxOffset));
    return {body};
	/*additionalSprites.clear();
	SpriteChainElement body;
    body.packPart = PackPart::ground;
    body.direction = Direction::DOWN;
    body.number = 1;

	if (typeOfObject == 3)
		body.packTag = PackTag::birchGrove;
	if (typeOfObject == 2)
		body.packTag = PackTag::darkWoods;
	if (typeOfObject == 1)
		body.packTag = PackTag::swampyTrees;

	body.size = Vector2f(conditionalSizeUnits);
	body.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(body);*/
}