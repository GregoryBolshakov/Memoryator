//#include "bush.h"
//
//#include "helper.h"
//
//bush::bush(std::string objectName, sf::Vector2f centerPosition, int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 15; // BrichGrove: 1-8; SwampyTrees: 9-15
//	this->type_ = typeOfObject;
//	strength = 0;
//	radius_ = 50;
//	animation_speed_ = 10;
//	to_save_name_ = "bush";
//	bush::setType(typeOfObject);
//	tag = entity_tag::bush;
//}
//
//void bush::setType(int typeOfObject)
//{
//	this->type_ = typeOfObject;
//	if (typeOfObject == 1)
//		size_ = { 173, 254 };
//	if (typeOfObject == 2)
//		size_ = { 152, 163 };
//	if (typeOfObject == 3)
//		size_ = { 272, 429 };
//	if (typeOfObject == 4)
//		size_ = { 283, 259 };
//	if (typeOfObject == 5)
//		size_ = { 349, 284 };
//	if (typeOfObject == 6)
//		size_ = { 363, 286 };
//	if (typeOfObject == 7)
//		size_ = { 231, 424 };
//	if (typeOfObject == 8)
//		size_ = { 405, 315 };
//	if (typeOfObject == 9)
//		size_ = { 642, 311 };
//	if (typeOfObject == 10)
//		size_ = { 289, 279 };
//	if (typeOfObject == 11)
//		size_ = { 396, 409 };
//	if (typeOfObject == 12)
//		size_ = { 385, 285 };
//	if (typeOfObject == 13)
//		size_ = { 450, 283 };
//	if (typeOfObject == 14)
//		size_ = { 462, 290 };
//	if (typeOfObject == 15)
//		size_ = { 189, 188 };
//}
//
//Vector2f bush::calculate_texture_offset()
//{
//	switch (type_)
//	{
//	case 1:
//		return { texture_box_.width * 0.551f, texture_box_.height * 0.01f };
//	case 2:
//		return { texture_box_.width * 0.59f, texture_box_.height * 0.01f };
//	case 3:
//		return { texture_box_.width * 0.5f, texture_box_.height * 0.929f };
//	case 4:
//		return { texture_box_.width * 0.49f, texture_box_.height * 0.624f };
//	case 5:
//		return { texture_box_.width * 0.506f, texture_box_.height * 0.632f };
//	case 6:
//		return { texture_box_.width * 0.495f, texture_box_.height * 0.734f };
//	case 7:
//		return { texture_box_.width * 0.551f, texture_box_.height * 0.01f };
//	case 8:
//		return { texture_box_.width * 0.59f, texture_box_.height * 0.01f };
//	case 9:
//		return { texture_box_.width * 0.553f, texture_box_.height * 0.908f };
//	case 10:
//		return { texture_box_.width * 0.525f, texture_box_.height * 0.9f };
//	case 11:
//		return { texture_box_.width * 0.578f, texture_box_.height * 0.931f };
//	case 12:
//		return { texture_box_.width * 0.506f, texture_box_.height * 0.853f };
//	case 13:
//		return { texture_box_.width * 0.542f, texture_box_.height * 0.83f };
//	case 14:
//		return { texture_box_.width * 0.519f, texture_box_.height * 0.863f };
//	case 15:
//		return { texture_box_.width * 0.495f, texture_box_.height * 0.734f };
//	default:
//		return { texture_box_.width * 0.409f, texture_box_.height * 0.945f };
//	}
//}
//
//void bush::init_pedestal()
//{
//	switch (type_)
//	{
//		case 1:
//		case 2:
//			focus1_ = sf::Vector2f(position_.x, position_.y);
//			focus2_ = sf::Vector2f(position_.x, position_.y);
//			ellipse_size_multipliers[0] = { 0 };
//			break;
//		case 3:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.224f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.224f, position_.y);
//			ellipse_size_multipliers[0] = { 1.29f };
//			break;
//		case 4:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.311f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.311f, position_.y);
//			ellipse_size_multipliers[0] = { 1.307f };
//			break;
//		case 5:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.32f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.32f, position_.y);
//			ellipse_size_multipliers[0] = { 1.3f };
//			break;
//		case 6:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.319f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.319f, position_.y);
//			ellipse_size_multipliers[0] = { 1.17f };
//			break;
//		case 7:
//		case 8:
//			focus1_ = sf::Vector2f(position_.x, position_.y);
//			focus2_ = sf::Vector2f(position_.x, position_.y);
//			ellipse_size_multipliers[0] = { 0 };
//			break;
//		case 9:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.119f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.119f, position_.y);
//			ellipse_size_multipliers[0] = { 1.29f };
//			break;
//		case 10:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.196f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.196f, position_.y);
//			ellipse_size_multipliers[0] = { 1.31f };
//			break;
//		case 11:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.123f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.123f, position_.y);
//			ellipse_size_multipliers[0] = { 1.26f };
//			break;
//		case 12:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.174f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.174f, position_.y);
//			ellipse_size_multipliers[0] = { 1.18f };
//			break;
//		case 13:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.165f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.165f, position_.y);
//			ellipse_size_multipliers[0] = { 1.24f };
//			break;
//		case 14:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.238f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.238f, position_.y);
//			ellipse_size_multipliers[0] = { 1.09f };
//			break;
//		case 15:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.289f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.289f, position_.y);
//			ellipse_size_multipliers[0] = { 1.29f };
//			break;
//		default:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.341f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.341f, position_.y);
//			ellipse_size_multipliers[0] = { 1.18f };
//			break;
//	}
//	init_route_blocks();
//}
//
//Vector2f bush::get_build_position(std::vector<world_object*>, float, sf::Vector2f)
//{
//	return { -1, -1 };
//}
//
//int bush::get_build_type(sf::Vector2f, sf::Vector2f)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> bush::prepare_sprites(long long)
//{
//	auto body = new sprite_chain_element(pack_tag::darkWoods, pack_part::bush, direction::DOWN, type_, position_, size_, sf::Vector2f(offset_), color, mirrored_);
//
//	if (type_ >= 1 && type_ <= 8)
//		body->pack_tag = pack_tag::birchGrove;
//	if (type_ >= 9 && type_ <= 15)
//	{
//		body->pack_tag = pack_tag::swampyTrees;
//		body->number -= 8;
//	}
//
//	return { body };
//}