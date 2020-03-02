#include "static_object.h"

static_object::static_object(std::string objectName, Vector2f centerPosition) : world_object(std::move(objectName), centerPosition)
{
}

static_object::~static_object()
= default;
