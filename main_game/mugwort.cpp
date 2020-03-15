#include "mugwort.h"

#include "helper.h"

mugwort::mugwort(std::string objectName, Vector2f centerPosition, int typeOfObject) : picked_object(std::move(objectName), centerPosition)
{
	variety_of_types_ = 3;
	this->type_of_object_ = typeOfObject;
	to_save_name_ = "mugwort";
	mugwort::setType(typeOfObject);
	radius_ = 50;
	tag = entity_tag::mugwort;
	count = 1;
	id = entity_tag::yarrow;
}

void mugwort::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->type_of_object_ = typeOfObject;
	if (typeOfObject == 1)
		conditional_size_units_ = { 308, 371 };
	if (typeOfObject == 2)
		conditional_size_units_ = { 303, 325 };
	if (typeOfObject == 3)
		conditional_size_units_ = { 250, 271 };
}

Vector2f mugwort::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return { texture_box_.width / 2.0f, texture_box_.height / 1.2f };
}

void mugwort::init_pedestal()
{
	focus1_ = position_;
	focus2_ = position_;
	ellipse_size_multipliers[0] = { 0 };
	init_micro_blocks();
}

Vector2f mugwort::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int mugwort::get_build_type(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<sprite_chain_element*> mugwort::prepare_sprites(long long elapsedTime)
{
    return {};
	/*additionalSprites.clear();
	spriteChainElement mugwortBody;
	mugwortBody.path = "Game/worldSprites/terrainObjects/mugwort/mugwort" + std::to_string(typeOfObject) + ".png";
	mugwortBody.size = Vector2f(conditionalSizeUnits);
	mugwortBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(mugwortBody);*/
}