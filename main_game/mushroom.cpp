//#include "mushroom.h"
//
//#include "helper.h"
//
//mushroom::mushroom(std::string objectName, const sf::Vector2f centerPosition, const int typeOfObject) : static_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 16; // BirchGrove: 1-3; DarkWoods: 4-12; SwampyTrees: 13-16
//	this->type_ = typeOfObject;
//	this->intangible = true;
//	to_save_name_ = "mushroom";
//	mushroom::setType(typeOfObject);
//	tag = entity_tag::mushroom;
//	mirrored_ = bool(rand() % 2);
//}
//
//void mushroom::setType(int typeOfObject)
//{
//	if (typeOfObject == -1)
//		return;
//
//	this->type_ = typeOfObject;
//	if (typeOfObject == 1)
//		size_ = { 72, 140 };
//	if (typeOfObject == 2)
//		size_ = { 67, 178 };
//	if (typeOfObject == 3)
//		size_ = { 82, 94 };
//	if (typeOfObject == 4)
//		size_ = { 143, 203 };
//	if (typeOfObject == 5)
//		size_ = { 120, 134 };
//	if (typeOfObject == 6)
//		size_ = { 173, 161 };
//	if (typeOfObject == 7)
//		size_ = { 70, 134 };
//	if (typeOfObject == 8)
//		size_ = { 91, 81 };
//	if (typeOfObject == 9)
//		size_ = { 94, 78 };
//	if (typeOfObject == 10)
//		size_ = { 103, 137 };
//	if (typeOfObject == 11)
//		size_ = { 89, 71 };
//	if (typeOfObject == 12)
//		size_ = { 144, 122 };
//	if (typeOfObject == 13)
//		size_ = { 54, 60 };
//	if (typeOfObject == 14)
//		size_ = { 80, 74 };
//	if (typeOfObject == 15)
//		size_ = { 80, 80 };
//	if (typeOfObject == 16)
//		size_ = { 77, 53 };
//}
//
//Vector2f mushroom::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//
//	if (type_ == 0)
//		return { 0, 0 };
//
//	return { texture_box_.width / 2.0f, texture_box_.height / 2.0f };
//}
//
//Vector2f mushroom::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition)
//{
//	return { -1, -1 };
//}
//
//int mushroom::get_build_type(sf::Vector2f ounPos, sf::Vector2f otherPos)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> mushroom::prepare_sprites(long long elapsedTime)
//{
//	sprite_chain_element* body = new sprite_chain_element(pack_tag::darkWoods, pack_part::mushroom, direction::DOWN, type_, position_, size_, sf::Vector2f(offset_));
//	body->isometric = true;
//
//	if (type_ >= 1 && type_ <= 3)
//		body->pack_tag = pack_tag::birchGrove;
//	if (type_ >= 4 && type_ <= 12)
//	{
//		body->pack_tag = pack_tag::darkWoods;
//		body->number -= 3;
//	}
//	if (type_ >= 13 && type_ <= 16)
//	{
//		body->pack_tag = pack_tag::swampyTrees;
//		body->number -= 12;
//	}
//
//	return { body };
//}