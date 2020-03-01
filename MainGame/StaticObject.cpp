#include "StaticObject.h"

StaticObject::StaticObject(std::string objectName, Vector2f centerPosition) : WorldObject(std::move(objectName), centerPosition)
{
}

StaticObject::~StaticObject()
= default;
