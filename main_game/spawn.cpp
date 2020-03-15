#include "spawn.h"

#include "helper.h"

spawn::spawn(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
{
	variety_of_types_ = 1;
	this->type_of_object_ = typeOfObject;
	radius_ = 200;
	to_save_name_ = "spawn";
	spawn::setType(typeOfObject);
}

void spawn::setType(const int typeOfObject)
{
	this->type_of_object_ = typeOfObject;
	this->conditional_size_units_ = { 1000, 1000 };
}

Vector2f spawn::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return { texture_box_.width / 2.0f, texture_box_.height / 1.6f };
}

void spawn::init_pedestal()
{
	if (type_of_object_ == 1)
	{
		focus1_ = Vector2f (texture_box_.left + floor(texture_box_.width / 3.8f), position_.y);
		focus2_ = Vector2f (texture_box_.left + texture_box_.width - floor(texture_box_.width / 3.8f), position_.y);
	}
	if (type_of_object_ == 2)
	{
		focus1_ = Vector2f (texture_box_.left + floor(texture_box_.width / 2.3f), position_.y);
		focus2_ = Vector2f (texture_box_.left + texture_box_.width - floor(texture_box_.width / 3.3f), position_.y);	
	}
	if (type_of_object_ == 3)
	{
		focus1_ = Vector2f (texture_box_.left + floor(texture_box_.width / 3.5f), position_.y + texture_box_.height / 20.0f);
		focus2_ = Vector2f (texture_box_.left + texture_box_.width - floor(texture_box_.width / 2.0f), position_.y + texture_box_.height / 100.0f);
	}
	ellipse_size_multipliers = { 1.08f, 1.2f, 1.3f };
}

Vector2f spawn::get_build_position(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

int spawn::get_build_type(Vector2f, Vector2f)
{
	return 1;
}

std::vector<sprite_chain_element*> spawn::prepare_sprites(long long)
{
    return {};
	/*additionalSprites.clear();
	spriteChainElement spawnBody;
	spawnBody.path = "Game/worldSprites/terrainObjects/brazier/brazier" + std::to_string(typeOfObject) + ".png";
	spawnBody.size = Vector2f(conditionalSizeUnits);
	spawnBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(spawnBody);*/
}