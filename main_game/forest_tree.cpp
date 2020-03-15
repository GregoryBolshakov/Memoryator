#include "forest_tree.h"

#include "helper.h"

forest_tree::forest_tree(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
{
	variety_of_types_ = 18; // BirchGrove: 1-7; DarkWoods: 8-13; SwampyTrees: 14-18
	this->type_of_object_ = typeOfObject;
	strength = 0;
	radius_ = 50;
	animation_speed_ = 10;
	to_save_name_ = "ForestTree";
	forest_tree::setType(typeOfObject);
	tag = entity_tag::tree;
}

void forest_tree::setType(const int typeOfObject)
{
	this->type_of_object_ = typeOfObject;
	if (typeOfObject == 1)
		conditional_size_units_ = { 396, 1090 };
	if (typeOfObject == 2)
		conditional_size_units_ = { 536, 1150 };
	if (typeOfObject == 3)
		conditional_size_units_ = { 244, 799 };
	if (typeOfObject == 4)
		conditional_size_units_ = { 623, 875 };
	if (typeOfObject == 5)
		conditional_size_units_ = { 644, 1235 };
	if (typeOfObject == 6)
		conditional_size_units_ = { 681, 1027 };
	if (typeOfObject == 7)
		conditional_size_units_ = { 616, 1169 };
	if (typeOfObject == 8)
		conditional_size_units_ = { 457, 1152 };
	if (typeOfObject == 9)
		conditional_size_units_ = { 527, 1188 };
	if (typeOfObject == 10)
		conditional_size_units_ = { 786, 1296 };
	if (typeOfObject == 11)
		conditional_size_units_ = { 806, 1292 };
	if (typeOfObject == 12)
		conditional_size_units_ = { 795, 1190 };
	if (typeOfObject == 13)
		conditional_size_units_ = { 1221, 386 };
	if (typeOfObject == 14)
		conditional_size_units_ = { 952, 962 };
	if (typeOfObject == 15)
		conditional_size_units_ = { 565, 1261 };
	if (typeOfObject == 16)
		conditional_size_units_ = { 914, 1108 };
	if (typeOfObject == 17)
		conditional_size_units_ = { 652, 1359 };
	if (typeOfObject == 18)
		conditional_size_units_ = { 667, 717 };

	//const auto extension = float(90 + rand() % 20) / 100.0f; // in percents
	//conditionalSizeUnits.x *= extension; conditionalSizeUnits.y *= extension;
}

Vector2f forest_tree::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;

	switch (type_of_object_)
	{
		case 1:
			return { texture_box_.width * 0.257f, texture_box_.height * 0.932f };
		case 2:
			return { texture_box_.width * 0.45f, texture_box_.height * 0.96f };
		case 3:
			return { texture_box_.width * 0.55f, texture_box_.height * 0.96f };
		case 4:
			return { texture_box_.width * 0.409f, texture_box_.height * 0.945f };
		case 5:
			return { texture_box_.width * 0.41f, texture_box_.height * 0.935f };
		case 6:
			return { texture_box_.width * 0.536f, texture_box_.height * 0.94f };
		case 7:
			return { texture_box_.width * 0.526f, texture_box_.height * 0.936f };
		case 8:
			return { texture_box_.width * 0.560f, texture_box_.height * 0.96f };
		case 9:
		case 10:
			return { texture_box_.width * 0.467f, texture_box_.height * 0.94f };
		case 11:
			return { texture_box_.width * 0.554f, texture_box_.height * 0.95f };
		case 12:
			return { texture_box_.width * 0.327f, texture_box_.height * 0.93f };
		case 13:
			return { texture_box_.width * 0.574f, texture_box_.height * 0.508f };
		case 14:
			return { texture_box_.width * 0.521f, texture_box_.height * 0.883f };
		case 15:
			return { texture_box_.width * 0.509f, texture_box_.height * 0.857f };
		case 16:
			return { texture_box_.width * 0.537f, texture_box_.height * 0.83f };
		case 17:
			return { texture_box_.width * 0.349f, texture_box_.height * 0.861f };
		case 18:
			return { texture_box_.width * 0.439f, texture_box_.height * 0.857f };
		default:
			return { texture_box_.width * 0.45f, texture_box_.height * 0.96f };
	}
}

void forest_tree::init_pedestal()
{
	std::pair<Vector2f, Vector2f> microEllipse;
	switch (type_of_object_)
	{
		case 1:		
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.158f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.158f, position_.y);
			ellipse_size_multipliers[0] = { 1.44f };
			break;		
		case 2:		
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.07f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.07f, position_.y);
			ellipse_size_multipliers[0] = { 1.58f };
			break;		
		case 3:		
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.184f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.184f, position_.y);
			ellipse_size_multipliers[0] = { 1.42f };
			break;		
		case 4:		
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.082f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.082f, position_.y);
			ellipse_size_multipliers[0] = { 1.42f };
			break;		
		case 5:		
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.088f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.088f, position_.y);
			ellipse_size_multipliers[0] = { 1.57f };
			break;		
		case 6:		
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.084f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.084f, position_.y);
			ellipse_size_multipliers[0] = { 1.31f };
			break;		
		case 7:		
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.131f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.131f, position_.y);
			ellipse_size_multipliers[0] = { 1.28f };
			break;		
		case 8:		
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.118f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.118f, position_.y);
			ellipse_size_multipliers[0] = { 1.42f };
			break;
		case 9:		
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.124f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.124f, position_.y);
			ellipse_size_multipliers[0] = { 1.21f };
			break;		
		case 10:		
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.127f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.127f, position_.y);
			ellipse_size_multipliers[0] = { 1.25f };
			break;		
		case 11:		
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.163f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.163f, position_.y);
			ellipse_size_multipliers[0] = { 1.25f };
			break;		
		case 12:		
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.137f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.137f, position_.y);
			ellipse_size_multipliers[0] = { 1.25f };
			break;		
		case 13:
			this->is_multi_ellipse = true;
			focus1_ = Vector2f(position_.x, position_.y);
			focus2_ = Vector2f(position_.x, position_.y);

			microEllipse.first = Vector2f(position_.x - texture_box_.width * 0.42f, position_.y + texture_box_.height * 0.247f);
			microEllipse.second = Vector2f(position_.x - texture_box_.width * 0.178f, position_.y + texture_box_.height * 0.247f);
			internal_ellipses.push_back(microEllipse);

			microEllipse.first = Vector2f(position_.x - texture_box_.width * 0.235f, position_.y + texture_box_.height * 0.171f);
			microEllipse.second = Vector2f(position_.x - texture_box_.width * 0.026f, position_.y + texture_box_.height * 0.171f);
			internal_ellipses.push_back(microEllipse);

			microEllipse.first = Vector2f(position_.x - texture_box_.width * 0.044f, position_.y + texture_box_.height * 0.047f);
			microEllipse.second = Vector2f(position_.x + texture_box_.width * 0.132f, position_.y + texture_box_.height * 0.047f);
			internal_ellipses.push_back(microEllipse);

			microEllipse.first = Vector2f(position_.x + texture_box_.width * 0.14f, position_.y - texture_box_.height * 0.012f);
			microEllipse.second = Vector2f(position_.x + texture_box_.width * 0.385f, position_.y - texture_box_.height * 0.012f);
			internal_ellipses.push_back(microEllipse);
			ellipse_size_multipliers = { 1.03f, 1.09f, 1.27f, 1.12f };
			break;
		case 14:
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.079f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.079f, position_.y);
			ellipse_size_multipliers[0] = { 1.42f };
			break;
		case 15:
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.09f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.09f, position_.y);
			ellipse_size_multipliers[0] = { 1.55f };
			break;
		case 16:
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.066f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.066f, position_.y);
			ellipse_size_multipliers[0] = { 1.46f };
			break;
		case 17:
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.162f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.162f, position_.y);
			ellipse_size_multipliers[0] = { 1.25f };
			break;
		case 18:
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.109f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.109f, position_.y);
			ellipse_size_multipliers[0] = { 1.25f };
			break;
		default:		
			focus1_ = Vector2f(position_.x - texture_box_.width * 0.106f, position_.y);
			focus2_ = Vector2f(position_.x + texture_box_.width * 0.106f, position_.y);
			ellipse_size_multipliers[0] = { 1.82f };
			break;		
	}
	init_micro_blocks();
}

Vector2f forest_tree::get_build_position(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

Vector2f forest_tree::getOwlBase() const
{
	return { position_.x, position_.y - conditional_size_units_.y / 2 };
}

int forest_tree::get_build_type(Vector2f, Vector2f)
{
	return 1;
}

std::vector<sprite_chain_element*> forest_tree::prepare_sprites(const long long elapsedTime)
{
	auto body = new sprite_chain_element(pack_tag::darkWoods, pack_part::tree, direction::DOWN, type_of_object_, position_, conditional_size_units_, Vector2f(texture_box_offset_), color, mirrored_);

	if (type_of_object_ >= 1 && type_of_object_ <= 7)
		body->pack_tag = pack_tag::birchGrove;
	if (type_of_object_ >= 8 && type_of_object_ <= 13)
	{
		body->pack_tag = pack_tag::darkWoods;
		body->number -= 7;
	}
	if (type_of_object_ >= 14 && type_of_object_ <= 18)
	{
		body->pack_tag = pack_tag::swampyTrees;
		body->number -= 13;
	}

	if (state_ == absorbed)
	{
		body->animation_length = 15;
		color.a = 255 - current_sprite_[0] * 255 / body->animation_length;
	}

	time_for_new_sprite_ += elapsedTime;

	if (time_for_new_sprite_ >= long(1e6 / animation_speed_))
	{
		time_for_new_sprite_ = 0;

		if (++current_sprite_[0] > body->animation_length)
		{
			if (state_ == absorbed)
			{
				state_ = common;
				delete_promise_on();
			}
			current_sprite_[0] = 1;
		}
	}

	return { body };
}