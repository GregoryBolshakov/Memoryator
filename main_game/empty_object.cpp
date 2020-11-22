#include "empty_object.h"


empty_object::empty_object(std::string objectName, Vector2f centerPosition) : world_object(std::move(objectName), centerPosition)
{
	tag = entity_tag::emptyObject;
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

Vector2f empty_object::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return {-1, -1};
}

int empty_object::get_build_type(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void empty_object::set_texture_size(Vector2f textureSize)
{
	return;
}

Vector2f empty_object::calculate_texture_offset()
{
	return {0, 0};
}

void empty_object::init_pedestal()
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
