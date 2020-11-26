//#include "stump.h"
//
//#include "helper.h"
//
//stump::stump(std::string objectName, const sf::Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 13; // BirchGrove: 1-4; DarkWoods: 5-10; SwampyTrees: 11-13
//	this->type_of_object_ = typeOfObject;
//	strength = 0;
//	radius_ = 50;
//	animation_speed_ = 10;
//	to_save_name_ = "stump";
//	stump::setType(typeOfObject);
//	tag = entity_tag::stump;
//}
//
//void stump::setType(const int typeOfObject)
//{
//	this->type_of_object_ = typeOfObject;
//	if (typeOfObject == 1)
//		conditional_size_units_ = { 412, 256 };
//	if (typeOfObject == 2)
//		conditional_size_units_ = { 507, 337 };
//	if (typeOfObject == 3)
//		conditional_size_units_ = { 647, 312 };
//	if (typeOfObject == 4)
//		conditional_size_units_ = { 462, 269 };
//	if (typeOfObject == 5)
//		conditional_size_units_ = { 243, 203 };
//	if (typeOfObject == 6)
//		conditional_size_units_ = { 236, 315 };
//	if (typeOfObject == 7)
//		conditional_size_units_ = { 286, 576 };
//	if (typeOfObject == 8)
//		conditional_size_units_ = { 354, 570 };
//	if (typeOfObject == 9)
//		conditional_size_units_ = { 335, 266 };
//	if (typeOfObject == 10)
//		conditional_size_units_ = { 186, 412 };
//	if (typeOfObject == 11)
//		conditional_size_units_ = { 250, 663 };
//	if (typeOfObject == 12)
//		conditional_size_units_ = { 526, 441 };
//	if (typeOfObject == 13)
//		conditional_size_units_ = { 267, 434 };
//}
//
//Vector2f stump::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//
//	switch (type_of_object_)
//	{
//	case 1:
//		return { texture_box_.width * 0.551f, texture_box_.height * 0.645f };
//	case 2:
//		return { texture_box_.width * 0.542f, texture_box_.height * 0.64f };
//	case 3:
//		return { texture_box_.width * 0.518f, texture_box_.height * 0.446f };
//	case 4:
//		return { texture_box_.width * 0.485f, texture_box_.height * 0.591f };
//	case 5:
//		return { texture_box_.width * 0.551f, texture_box_.height * 0.771f };
//	case 6:
//		return { texture_box_.width * 0.468f, texture_box_.height * 0.796f };
//	case 7:
//		return { texture_box_.width * 0.497f, texture_box_.height * 0.859f };
//	case 8:
//		return { texture_box_.width * 0.525f, texture_box_.height * 0.889f };
//	case 9:
//		return { texture_box_.width * 0.501f, texture_box_.height * 0.728f };
//	case 10:
//		return { texture_box_.width * 0.46f, texture_box_.height * 0.83f };
//	case 11:
//		return { texture_box_.width * 0.467f, texture_box_.height * 0.868f };
//	case 12:
//		return { texture_box_.width * 0.554f, texture_box_.height * 0.758f };
//	case 13:
//		return { texture_box_.width * 0.46f, texture_box_.height * 0.83f };
//	default:
//		return { texture_box_.width * 0.409f, texture_box_.height * 0.945f };
//	}	
//}
//
//void stump::init_pedestal()
//{
//	switch (type_of_object_)
//	{
//	case 1:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.167f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.167f, position_.y);
//		ellipse_size_multipliers[0] = { 1.4f };
//		break;
//	case 2:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.353f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.353f, position_.y);
//		ellipse_size_multipliers[0] = { 1.15f };
//		break;
//	case 3:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.248f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.248f, position_.y);
//		ellipse_size_multipliers[0] = { 1.25f };
//		break;
//	case 4:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.311f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.311f, position_.y);
//		ellipse_size_multipliers[0] = { 1.307f };
//		break;
//	case 5:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.167f, position_.y - texture_box_.height * 0.091f);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.167f, position_.y - texture_box_.height * 0.091f);
//		ellipse_size_multipliers[0] = { 1.64f };
//		break;
//	case 6:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.327f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.327f, position_.y);
//		ellipse_size_multipliers[0] = { 1.34f };
//		break;
//	case 7:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.268f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.268f, position_.y);
//		ellipse_size_multipliers[0] = { 1.388f };
//		break;
//	case 8:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.142f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.142f, position_.y);
//		ellipse_size_multipliers[0] = { 1.62f };
//		break;
//	case 9:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.314f, position_.y - texture_box_.height * 0.07f);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.314f, position_.y - texture_box_.height * 0.07f);
//		ellipse_size_multipliers[0] = { 1.24f };
//		break;
//	case 10:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.335f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.335f, position_.y);
//		ellipse_size_multipliers[0] = { 1.43f };
//		break;
//	case 11:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.236f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.236f, position_.y);
//		ellipse_size_multipliers[0] = { 1.62f };
//		break;
//	case 12:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.223f, position_.y - texture_box_.height * 0.062f);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.223f, position_.y - texture_box_.height * 0.062f);
//		ellipse_size_multipliers[0] = { 1.19f };
//		break;
//	case 13:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.335f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.335f, position_.y);
//		ellipse_size_multipliers[0] = { 1.43f };
//		break;
//	default:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.111f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.111f, position_.y);
//		ellipse_size_multipliers[0] = { 1.78f };
//		break;
//	}
//	init_route_blocks();
//}
//
//Vector2f stump::get_build_position(std::vector<world_object*>, float, sf::Vector2f)
//{
//	return { -1, -1 };
//}
//
//int stump::get_build_type(sf::Vector2f, sf::Vector2f)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> stump::prepare_sprites(long long)
//{
//	auto body = new sprite_chain_element(pack_tag::darkWoods, pack_part::stump, direction::DOWN, type_of_object_, position_, conditional_size_units_, sf::Vector2f(texture_box_offset_), color, mirrored_);
//
//	if (type_of_object_ >= 1 && type_of_object_ <= 4)
//		body->pack_tag = pack_tag::birchGrove;
//	if (type_of_object_ >= 5 && type_of_object_ <= 10)
//	{
//		body->pack_tag = pack_tag::darkWoods;
//		body->number -= 4;
//	}
//	if (type_of_object_ >= 11 && type_of_object_ <= 13)
//	{
//		body->pack_tag = pack_tag::swampyTrees;
//		body->number -= 10;
//	}
//
//	set_unscaled({ body });
//	
//	return { body };
//}