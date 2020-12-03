//#include "totem.h"
//
//#include "helper.h"
//
//
//totem::totem(std::string objectName, const sf::Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 1;
//	this->type_ = typeOfObject;
//	strength = 0;
//	radius_ = 20;
//	to_save_name_ = "totem";
//	totem::setType(typeOfObject);
//	tag = entity_tag::totem;
//}
//
//void totem::setType(const int typeOfObject)
//{
//	this->type_ = typeOfObject;
//	if (typeOfObject == 1)
//		size_ = { 154, 544 };
//	if (typeOfObject == 2)
//		size_ = { 154, 544 };
//	if (typeOfObject == 3)
//		size_ = { 568, 292 };
//	if (typeOfObject == 4)
//		size_ = { 336, 240 };
//	if (typeOfObject == 5)
//		size_ = { 280, 212 };
//	if (typeOfObject == 6)
//		size_ = { 272, 196 };
//
//	radius_ = std::max(size_.x, size_.y) / 2;
//}
//
//Vector2f totem::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//
//	if (type_ == 1)
//		return { texture_box_.width / 2.0f, texture_box_.height / 1.15f };
//	if (type_ == 2)
//		return { texture_box_.width / 2.0f, texture_box_.height / 1.15f };
//	if (type_ == 3)
//		return { texture_box_.width / 2.0f, texture_box_.height / 1.5f };
//	if (type_ == 4)
//		return { texture_box_.width / 2.0f, texture_box_.height / 1.5f };
//	if (type_ == 5)
//		return { texture_box_.width / 2.0f, texture_box_.height / 1.5f };
//	if (type_ == 6)
//		return { texture_box_.width / 2.0f, texture_box_.height / 1.25f };
//
//	throw std::logic_error("Invalid type of object.");
//}
//
//void totem::init_pedestal()
//{
//	//if (typeOfObject == 1)
//	{
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width / 4, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width / 4, position_.y);
//		ellipse_size_multipliers[0] = { 1.8f };
//	}	
//	init_route_blocks();
//}
//
//Vector2f totem::get_build_position(std::vector<world_object*>, float, sf::Vector2f)
//{
//	return { -1, -1 };
//}
//
//int totem::get_build_type(sf::Vector2f, sf::Vector2f)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> totem::prepare_sprites(long long)
//{
//	const auto body = new sprite_chain_element(pack_tag::craftObjects, pack_part::totem, direction::DOWN, type_, position_, size_, sf::Vector2f(offset_), color, mirrored_);
//
//	return { body };
//}