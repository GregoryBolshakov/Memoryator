#include "empty_object.h"
#include "sprite_chain_element.h"

empty_object::empty_object(std::string objectName, sf::Vector2f centerPosition) : world_object(std::move(objectName), centerPosition)
{
	tag = entity_tag::empty_object;
}


empty_object::~empty_object()
= default;

std::vector<unique_ptr<sprite_chain_element>> empty_object::prepare_sprites(long long elapsedTime)
{
    return {};
}

void empty_object::init_pedestal()
{
	return;
}

void setTextureSize(sf::Vector2f textureSize)
{
	return;
}

sf::Vector2f calculateTextureOffset()
{
	return {0, 0};
}

void initPedestal()
{
	return;
}
