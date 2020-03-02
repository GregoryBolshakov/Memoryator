#include "empty_object.h"


empty_object::empty_object(std::string objectName, Vector2f centerPosition) : world_object(std::move(objectName), centerPosition)
{
	tag = entity_tag::emptyObject;
}


empty_object::~empty_object()
= default;

std::vector<sprite_chain_element*> empty_object::prepareSprites(long long elapsedTime)
{
    return {};
}

int empty_object::getSpriteNumber()
{
	return 0;
}

Vector2f empty_object::getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return {-1, -1};
}

int empty_object::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void empty_object::setTextureSize(Vector2f textureSize)
{
	return;
}

Vector2f empty_object::calculateTextureOffset()
{
	return {0, 0};
}

void empty_object::initPedestal()
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
