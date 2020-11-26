//#include "lake.h"
//
//#include "helper.h"
//
//lake::lake(std::string objectName, const sf::Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 4; // SwampyTrees: 1-4
//	this->type_of_object_ = typeOfObject;
//	strength = 0;
//	radius_ = 50;
//	animation_speed_ = 10;
//	to_save_name_ = "lake";
//	is_multi_ellipse = true;
//	lake::setType(typeOfObject);
//	tag = entity_tag::lake;
//}
//
//void lake::setType(const int typeOfObject)
//{
//	this->type_of_object_ = typeOfObject;
//	if (typeOfObject == 1)
//		conditional_size_units_ = { 1072, 476 };
//	if (typeOfObject == 2)
//		conditional_size_units_ = { 482, 279 };
//	if (typeOfObject == 3)
//		conditional_size_units_ = { 839, 315 };
//	if (typeOfObject == 4)
//		conditional_size_units_ = { 473, 388 };
//}
//
//Vector2f lake::calculate_texture_offset()
//{
//	switch (type_of_object_)
//	{
//	case 1:
//		return { texture_box_.width * 0.55f, texture_box_.height * 0.382f };
//	case 2:
//		return { texture_box_.width * 0.503f, texture_box_.height * 0.504f };
//	case 3:
//		return { texture_box_.width * 0.543f, texture_box_.height * 0.0325f };
//	case 4:
//		return { texture_box_.width * 0.473f, texture_box_.height * -0.141f };
//	default:
//		return { texture_box_.width * 0.409f, texture_box_.height * 0.945f };
//	}
//}
//
//void lake::init_pedestal()
//{
//	std::pair<sf::Vector2f, sf::Vector2f> microEllipse;
//	switch (type_of_object_)
//	{
//	case 1:
//		focus1_ = sf::Vector2f(position_.x, position_.y);
//		focus2_ = sf::Vector2f(position_.x, position_.y);
//
//		microEllipse.first = sf::Vector2f(position_.x - texture_box_.width * 0.49f, position_.y + texture_box_.height * 0.384f);
//		microEllipse.second = sf::Vector2f(position_.x - texture_box_.width * 0.0158f, position_.y + texture_box_.height * 0.384f);
//		internal_ellipses.push_back(microEllipse);
//
//		microEllipse.first = sf::Vector2f(position_.x - texture_box_.width * 0.184f, position_.y + texture_box_.height * 0.269f);
//		microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.096f, position_.y + texture_box_.height * 0.269f);
//		internal_ellipses.push_back(microEllipse);
//
//		microEllipse.first = sf::Vector2f(position_.x - texture_box_.width * 0.102f, position_.y + texture_box_.height * 0.428f);
//		microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.0549f, position_.y + texture_box_.height * 0.428f);
//		internal_ellipses.push_back(microEllipse);
//
//		microEllipse.first = sf::Vector2f(position_.x + texture_box_.width * 0.0222f, position_.y + texture_box_.height * 0.159f);
//		microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.352f, position_.y + texture_box_.height * 0.159f);
//		internal_ellipses.push_back(microEllipse);
//		ellipse_size_multipliers = { 1.06f, 1.13f, 1.26f, 1.07f, 1.16f };
//		break;
//	case 2:
//		focus1_ = sf::Vector2f(position_.x, position_.y);
//		focus2_ = sf::Vector2f(position_.x, position_.y);
//
//		microEllipse.first = sf::Vector2f(position_.x - texture_box_.width * 0.452f, position_.y - texture_box_.height * 0.0536f);
//		microEllipse.second = sf::Vector2f(position_.x - texture_box_.width * 0.117f, position_.y - texture_box_.height * 0.0536f);
//		internal_ellipses.push_back(microEllipse);
//
//		microEllipse.first = sf::Vector2f(position_.x - texture_box_.width * 0.154f, position_.y + texture_box_.height * 0.202f);
//		microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.141f, position_.y + texture_box_.height * 0.202f);
//		internal_ellipses.push_back(microEllipse);
//
//		microEllipse.first = sf::Vector2f(position_.x + texture_box_.width * 0.166f, position_.y + texture_box_.height * 0.179f);
//		microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.438f, position_.y + texture_box_.height * 0.179f);
//		internal_ellipses.push_back(microEllipse);
//
//		microEllipse.first = sf::Vector2f(position_.x - texture_box_.width * 0.17f, position_.y - texture_box_.height * 0.244f);
//		microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.114f, position_.y - texture_box_.height * 0.244f);
//		internal_ellipses.push_back(microEllipse);
//		ellipse_size_multipliers = { 1.45f, 1.37f, 1.38f, 1.39f };
//		break;
//	case 3:
//		focus1_ = sf::Vector2f(position_.x, position_.y);
//		focus2_ = sf::Vector2f(position_.x, position_.y);
//
//		microEllipse.first = sf::Vector2f(position_.x - texture_box_.width * 0.463f, position_.y + texture_box_.height * 0.601f);
//		microEllipse.second = sf::Vector2f(position_.x - texture_box_.width * 0.108f, position_.y + texture_box_.height * 0.601f);
//		internal_ellipses.push_back(microEllipse);
//
//		microEllipse.first = sf::Vector2f(position_.x - texture_box_.width * 0.145f, position_.y + texture_box_.height * 0.759f);
//		microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.15f, position_.y + texture_box_.height * 0.759f);
//		internal_ellipses.push_back(microEllipse);
//
//		microEllipse.first = sf::Vector2f(position_.x + texture_box_.width * 0.175f, position_.y + texture_box_.height * 0.242f);
//		microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.392f, position_.y + texture_box_.height * 0.242f);
//		internal_ellipses.push_back(microEllipse);
//
//		microEllipse.first = sf::Vector2f(position_.x - texture_box_.width * 0.16f, position_.y + texture_box_.height * 0.426f);
//		microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.155f, position_.y + texture_box_.height * 0.426f);
//		internal_ellipses.push_back(microEllipse);
//		ellipse_size_multipliers = { 1.21f, 1.11f, 1.27f, 1.3f };
//		break;
//	case 4:
//		focus1_ = sf::Vector2f(position_.x, position_.y);
//		focus2_ = sf::Vector2f(position_.x, position_.y);
//
//		microEllipse.first = sf::Vector2f(position_.x - texture_box_.width * 0.413f, position_.y + texture_box_.height * 1.04f);
//		microEllipse.second = sf::Vector2f(position_.x - texture_box_.width * 0.066f, position_.y + texture_box_.height * 1.04f);
//		internal_ellipses.push_back(microEllipse);
//
//		microEllipse.first = sf::Vector2f(position_.x - texture_box_.width * 0.046f, position_.y + texture_box_.height * 0.558f);
//		microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.399f, position_.y + texture_box_.height * 0.558f);
//		internal_ellipses.push_back(microEllipse);
//
//		microEllipse.first = sf::Vector2f(position_.x + texture_box_.width * 0.027f, position_.y + texture_box_.height * 0.305f);
//		microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.24f, position_.y + texture_box_.height * 0.305f);
//		internal_ellipses.push_back(microEllipse);
//
//		microEllipse.first = sf::Vector2f(position_.x + texture_box_.width * 0.313f, position_.y + texture_box_.height * 0.782f);
//		microEllipse.second = sf::Vector2f(position_.x - texture_box_.width * 0.428f, position_.y + texture_box_.height * 0.782f);
//		internal_ellipses.push_back(microEllipse);
//		ellipse_size_multipliers = { 1.19f, 1.21f, 1.53f, 1.04f };
//		break;
//	}
//	init_route_blocks();
//}
//
//Vector2f lake::get_build_position(std::vector<world_object*>, float, sf::Vector2f)
//{
//	return { -1, -1 };
//}
//
//int lake::get_build_type(sf::Vector2f, sf::Vector2f)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> lake::prepare_sprites(long long)
//{
//	std::vector<sprite_chain_element*> result = {};
//	const auto body = new sprite_chain_element(pack_tag::swampyTrees, pack_part::lake, direction::DOWN, type_of_object_, position_, conditional_size_units_, sf::Vector2f(texture_box_offset_));
//
//	result.push_back(body);
//
//	set_unscaled(result);
//
//	return result ;
//}