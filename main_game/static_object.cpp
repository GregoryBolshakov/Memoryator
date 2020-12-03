#include "static_object.h"

static_object::static_object(std::string name, const sf::Vector2f position, const int kind) : world_object(std::move(name), position, kind)
{
}

static_object::~static_object()
= default;
