#include "ground_connection.h"

#include "helper.h"

ground_connection::ground_connection(std::string object_name, const Vector2f center_position, const int type_of_object) : static_object(std::move(object_name), center_position)
{
	variety_of_types_ = 12; // SwampyTrees: 1-4; DarkWoods: 5-8; BirchGrove: 9-12
	this->type_of_object_ = type_of_object;
	is_background = true;
	to_save_name_ = "groundConnection";
	ground_connection::setType(type_of_object);
	tag = entity_tag::groundConnection;
}

void ground_connection::setType(const int type_of_object)
{
	if (type_of_object == -1)
		return;

	this->type_of_object_ = type_of_object;
	if (type_of_object % 4 == 1)
	{
		this->conditional_size_units_ = { 1000, 250 };
	}
	else
		if (type_of_object % 4 == 2)
		{
			this->conditional_size_units_ = { 1000, 250 };
		}
		else
			if (type_of_object % 4 == 3)
			{
				this->conditional_size_units_ = { 250, 1000 };
			}
			else
				if (type_of_object % 4 == 0)
				{
					this->conditional_size_units_ = { 250, 1000 };
				}

	if (type_of_object >= 1 && type_of_object <= 4)
		this->z_coordinate_ = 10;
	else
		if (type_of_object >= 5 && type_of_object <= 8)
			this->z_coordinate_ = 20;
		else
			if (type_of_object >= 9 && type_of_object <= 12)
				this->z_coordinate_ = 30;
			else
				if (type_of_object >= 13 && type_of_object <= 16)
					this->z_coordinate_ = 40;
				else
					if (type_of_object >= 17 && type_of_object <= 20)
						this->z_coordinate_ = 50;
}

Vector2f ground_connection::calculate_texture_offset()
{
	if (type_of_object_ % 4 == 1)
		return { 0, texture_box_.height - 5 };
	if (type_of_object_ % 4 == 2)
		return { 0, 5 };
	if (type_of_object_ % 4 == 3)
		return { texture_box_.width - 5, 0 };
	if (type_of_object_ % 4 == 0)
		return { 5, 0 };
	return { 0, 0 };
}

Vector2f ground_connection::get_build_position(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

int ground_connection::get_build_type(Vector2f, Vector2f)
{
	return 1;
}

std::vector<unique_ptr<sprite_chain_element>> ground_connection::prepare_sprites(long long)
{
	std::vector<unique_ptr<sprite_chain_element>> result;

	if (type_of_object_ >= 1 && type_of_object_ <= 4)
	{
		result.emplace_back(std::move(make_unique<sprite_chain_element>()));
		return result;
	}

	int spriteType = type_of_object_ % 4 + 1;
	if (spriteType == 1)
		spriteType = 5;

	auto body = make_unique<sprite_chain_element>(pack_tag::darkWoods, pack_part::ground, direction::DOWN, spriteType, position_, conditional_size_units_, Vector2f(texture_box_offset_));
	body->z_coordinate = z_coordinate_;

	//if (typeOfObject >= 1 && typeOfObject <= 4)
		//body->packTag = PackTag::darkWoods;
	if (type_of_object_ >= 5 && type_of_object_ <= 8)
		body->pack_tag = pack_tag::darkWoods;
	if (type_of_object_ >= 9 && type_of_object_ <= 12)
		body->pack_tag = pack_tag::birchGrove;

	body->is_background = true;

	result.emplace_back(std::move(body));
	return result;
}