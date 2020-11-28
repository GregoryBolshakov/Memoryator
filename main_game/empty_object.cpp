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

int empty_object::get_sprite_number()
{
	return 0;
}

sf::Vector2f empty_object::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition)
{
	return {-1, -1};
}

int empty_object::get_build_type(sf::Vector2f ounPos, sf::Vector2f otherPos)
{
	return 1;
}

void empty_object::set_texture_size(sf::Vector2f textureSize)
{
	return;
}

sf::Vector2f empty_object::calculate_texture_offset()
{
	return {0, 0};
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
