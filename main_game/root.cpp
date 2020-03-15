#include "root.h"

#include "helper.h"

root::root(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
{
	variety_of_types_ = 5; // 1-5
	this->type_of_object_ = typeOfObject;
	strength = 0;
	radius_ = 50;
	animation_speed_ = 10;
	to_save_name_ = "root";
	root::setType(typeOfObject);
	tag = entity_tag::root;
}

void root::setType(const int typeOfObject)
{
	this->type_of_object_ = typeOfObject;
	if (typeOfObject == 1)
		conditional_size_units_ = { 544, 138 };
	if (typeOfObject == 2)
		conditional_size_units_ = { 468, 208 };
	if (typeOfObject == 3)
		conditional_size_units_ = { 333, 228 };
	if (typeOfObject == 4)
		conditional_size_units_ = { 406, 137 };
	if (typeOfObject == 5)
		conditional_size_units_ = { 339, 132 };
}

Vector2f root::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;

	switch (type_of_object_)
	{	
	case 1:
		return { texture_box_.width * 0.698f, texture_box_.height * 0.47f };
	case 2:
		return { texture_box_.width * 0.313f, texture_box_.height * 0.671f };
	case 3:
		return { texture_box_.width * 0.429f, texture_box_.height * 0.508f };
	case 4:
		return { texture_box_.width * 0.485f, texture_box_.height * 0.01f };
	case 5:
		return { texture_box_.width * 0.551f, texture_box_.height * 0.01f };
	default:
		return { texture_box_.width * 0.409f, texture_box_.height * 0.01f };
	}
}

void root::init_pedestal()
{
	switch (type_of_object_)
	{
	case 1:
		focus1_ = Vector2f(position_.x - texture_box_.width * 0.229f, position_.y - texture_box_.height * 0.151f);
		focus2_ = Vector2f(position_.x + texture_box_.width * 0.229f, position_.y - texture_box_.height * 0.151f);
		ellipse_size_multipliers[0] = { 1.06f };
		break;
	case 2:
		focus1_ = Vector2f(position_.x - texture_box_.width * 0.288f, position_.y - texture_box_.height * 0.09f);
		focus2_ = Vector2f(position_.x + texture_box_.width * 0.288f, position_.y - texture_box_.height * 0.09f);
		ellipse_size_multipliers[0] = { 1.07f };
		break;
	case 3:
		focus1_ = Vector2f(position_.x - texture_box_.width * 0.367f, position_.y - texture_box_.height * 0.065f);
		focus2_ = Vector2f(position_.x + texture_box_.width * 0.367f, position_.y - texture_box_.height * 0.065f);
		ellipse_size_multipliers[0] = { 1.17f };
		break;
	case 4:
		focus1_ = Vector2f(position_.x - texture_box_.width * 0.311f, position_.y);
		focus2_ = Vector2f(position_.x + texture_box_.width * 0.311f, position_.y);
		ellipse_size_multipliers[0] = { 0 };
		break;
	case 5:
		focus1_ = Vector2f(position_.x - texture_box_.width * 0.167f, position_.y - texture_box_.height * 0.091f);
		focus2_ = Vector2f(position_.x + texture_box_.width * 0.167f, position_.y - texture_box_.height * 0.091f);
		ellipse_size_multipliers[0] = { 0 };
		break;
	default:
		focus1_ = Vector2f(position_.x - texture_box_.width * 0.111f, position_.y);
		focus2_ = Vector2f(position_.x + texture_box_.width * 0.111f, position_.y);
		ellipse_size_multipliers[0] = { 0 };
		break;
	}
	init_micro_blocks();
}

Vector2f root::get_build_position(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

int root::get_build_type(Vector2f, Vector2f)
{
	return 1;
}

std::vector<sprite_chain_element*> root::prepare_sprites(long long)
{
	const auto body = new sprite_chain_element(pack_tag::swampyTrees, pack_part::root, direction::DOWN, type_of_object_, position_, conditional_size_units_, Vector2f(texture_box_offset_), color, mirrored_);

	return { body };
}