#include "EmptyObject.h"



EmptyObject::EmptyObject(std::string objectName, Vector2f centerPosition) : WorldObject(objectName, centerPosition)
{
	tag = Tag::emptyObject;
}


EmptyObject::~EmptyObject()
{
}

std::vector<SpriteChainElement*> EmptyObject::prepareSprites(long long elapsedTime)
{
    return {};
}

int EmptyObject::getSpriteNumber()
{
	return 0;
}

Vector2f EmptyObject::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return {-1, -1};
}

int EmptyObject::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void EmptyObject::setTextureSize(Vector2f textureSize)
{
	return;
}

Vector2f EmptyObject::calculateTextureOffset()
{
	return {0, 0};
}

void EmptyObject::initPedestal()
{
	return;
}

void setTextureSize(Vector2f textureSize)
{
	return;
}

Vector2f calculateTextureOffset()
{
	return {0, 0};
}

void initPedestal()
{
	return;
}
