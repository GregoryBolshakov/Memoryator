//#include "grass.h"
//
//#include "helper.h"
//
//grass::grass(std::string objectName, const sf::Vector2f centerPosition, const int typeOfObject) : static_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 30; // BirchGrove: 1-8; DarkWoods: 9-21; SwampyTrees: 22-30
//	this->type_ = typeOfObject;
//	this->intangible = true;
//	to_save_name_ = "grass";
//	grass::setType(typeOfObject);
//	tag = entity_tag::grass;
//	mirrored_ = bool(rand() % 2);
//}
//
//void grass::setType(int typeOfObject)
//{
//	if (typeOfObject == -1)
//		return;
//
//	this->type_ = typeOfObject;
//	if (typeOfObject == 1)
//		size_ = { 86, 71 };
//	if (typeOfObject == 2)
//		size_ = { 146, 182 };
//	if (typeOfObject == 3)
//		size_ = { 205, 214 };
//	if (typeOfObject == 4)
//		size_ = { 205, 137 };
//	if (typeOfObject == 5)
//		size_ = { 108, 189 };
//	if (typeOfObject == 6)
//		size_ = { 116, 211 };
//	if (typeOfObject == 7)
//		size_ = { 141, 240 };
//	if (typeOfObject == 8)
//		size_ = { 220, 264 };
//	if (typeOfObject == 9)
//		size_ = { 251, 120 };
//	if (typeOfObject == 10)
//		size_ = { 222, 343 };
//	if (typeOfObject == 11)
//		size_ = { 357, 330 };
//	if (typeOfObject == 12)
//		size_ = { 386, 428 };
//	if (typeOfObject == 13)
//		size_ = { 425, 291 };
//	if (typeOfObject == 14)
//		size_ = { 87, 183 };
//	if (typeOfObject == 15)
//		size_ = { 113, 202 };
//	if (typeOfObject == 16)
//		size_ = { 67, 158 };
//	if (typeOfObject == 17)
//		size_ = { 119, 171 };
//	if (typeOfObject == 18)
//		size_ = { 178, 170 };
//	if (typeOfObject == 19)
//		size_ = { 188, 129 };
//	if (typeOfObject == 20)
//		size_ = { 136, 171 };
//	if (typeOfObject == 21)
//		size_ = { 171, 288 };
//	if (typeOfObject == 22)
//		size_ = { 491, 209 };
//	if (typeOfObject == 23)
//		size_ = { 166, 124 };
//	if (typeOfObject == 24)
//		size_ = { 78, 255 };
//	if (typeOfObject == 25)
//		size_ = { 171, 173 };
//	if (typeOfObject == 26)
//		size_ = { 123, 137 };
//	if (typeOfObject == 27)
//		size_ = { 133, 136 };
//	if (typeOfObject == 28)
//		size_ = { 91, 112 };
//	if (typeOfObject == 29)
//		size_ = { 119, 125 };
//	if (typeOfObject == 30)
//		size_ = { 134, 84 };
//}
//
//Vector2f grass::calculate_texture_offset()
//{	
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//	
//	return { texture_box_.width / 2.0f, texture_box_.height / 1.2f };
//}
//
//Vector2f grass::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition)
//{
//	return { -1, -1 };
//}
//
//int grass::get_build_type(sf::Vector2f ounPos, sf::Vector2f otherPos)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> grass::prepare_sprites(long long elapsedTime)
//{
//	std::vector<sprite_chain_element*> result = {};
//	sprite_chain_element* body = new sprite_chain_element(pack_tag::darkWoods, pack_part::plant, direction::DOWN, type_, position_, size_, sf::Vector2f(offset_));
//	body->isometric = true;
//
//	if (type_ >= 1 && type_ <= 8)
//		body->pack_tag = pack_tag::birchGrove;
//	if (type_ >= 9 && type_ <= 21)
//	{
//		body->pack_tag = pack_tag::darkWoods;
//		body->number -= 8;
//	}
//	if (type_ >= 22 && type_ <= 30)
//	{
//		body->pack_tag = pack_tag::swampyTrees;
//		body->number -= 21;
//	}
//	result.push_back(body);
//
//	set_unscaled(result);
//
//	return result;
//}