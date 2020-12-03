//#include "roof.h"
//
//#include "helper.h"
//
//roof::roof(std::string objectName, const sf::Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 4;
//	this->type_ = typeOfObject;
//	strength = 0;
//	radius_ = 50;
//	animation_speed_ = 10;
//	to_save_name_ = "roof";
//	roof::setType(typeOfObject);
//	tag = entity_tag::roof;
//}
//
//void roof::setType(const int typeOfObject)
//{
//	this->type_ = typeOfObject;
//	if (typeOfObject == 1)
//		size_ = { 248, 373 };
//	if (typeOfObject == 2)
//		size_ = { 254, 555 };
//	if (typeOfObject == 3)
//		size_ = { 255, 485 };
//	if (typeOfObject == 4)
//		size_ = {239, 474};
//}
//
//Vector2f roof::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//
//	switch (type_)
//	{
//	case 1:
//		return { texture_box_.width * 0.501f, texture_box_.height * 0.993f };
//	case 2:
//		return { texture_box_.width * 0.655f, texture_box_.height * 0.934f };
//	case 3:
//		return { texture_box_.width * 0.561f, texture_box_.height * 0.96f };
//	case 4:
//		return { texture_box_.width * 0.245f, texture_box_.height * 0.944f };
//	default:
//		return { texture_box_.width * 0.506f, texture_box_.height * 0.632f };
//	}
//}
//
//void roof::init_pedestal()
//{
//	switch (type_)
//	{
//	case 1:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.152f, position_.y - texture_box_.height * 0.021f);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.152f, position_.y - texture_box_.height * 0.021f);
//		ellipse_size_multipliers[0] = { 1.5f };
//		break;
//	case 2:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.143f, position_.y - texture_box_.height * 0.021f);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.143f, position_.y - texture_box_.height * 0.021f);
//		ellipse_size_multipliers[0] = { 1.55f };
//		break;
//	case 3:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.158f, position_.y - texture_box_.height * 0.037f);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.158f, position_.y - texture_box_.height * 0.037f);
//		ellipse_size_multipliers[0] = { 1.62f };
//		break;
//	case 4:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.176f, position_.y - texture_box_.height * 0.032f);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.176f, position_.y - texture_box_.height * 0.032f);
//		ellipse_size_multipliers[0] = { 1.55f };
//		break;
//	default:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.32f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.32f, position_.y);
//		ellipse_size_multipliers[0] = { 1.3f };
//		break;
//	}
//	init_route_blocks();
//}
//
//Vector2f roof::get_build_position(std::vector<world_object*>, float, sf::Vector2f)
//{
//	return { -1, -1 };
//}
//
//int roof::get_build_type(sf::Vector2f, sf::Vector2f)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> roof::prepare_sprites(long long)
//{
//	const auto body = new sprite_chain_element(pack_tag::darkWoods, pack_part::roof, direction::DOWN, type_, position_, size_, sf::Vector2f(offset_), color, mirrored_);
//
//	return { body };
//}