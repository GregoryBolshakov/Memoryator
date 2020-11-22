//#include "grass.h"
//
//#include "helper.h"
//
//grass::grass(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : static_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 30; // BirchGrove: 1-8; DarkWoods: 9-21; SwampyTrees: 22-30
//	this->type_of_object_ = typeOfObject;
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
//	this->type_of_object_ = typeOfObject;
//	if (typeOfObject == 1)
//		conditional_size_units_ = { 86, 71 };
//	if (typeOfObject == 2)
//		conditional_size_units_ = { 146, 182 };
//	if (typeOfObject == 3)
//		conditional_size_units_ = { 205, 214 };
//	if (typeOfObject == 4)
//		conditional_size_units_ = { 205, 137 };
//	if (typeOfObject == 5)
//		conditional_size_units_ = { 108, 189 };
//	if (typeOfObject == 6)
//		conditional_size_units_ = { 116, 211 };
//	if (typeOfObject == 7)
//		conditional_size_units_ = { 141, 240 };
//	if (typeOfObject == 8)
//		conditional_size_units_ = { 220, 264 };
//	if (typeOfObject == 9)
//		conditional_size_units_ = { 251, 120 };
//	if (typeOfObject == 10)
//		conditional_size_units_ = { 222, 343 };
//	if (typeOfObject == 11)
//		conditional_size_units_ = { 357, 330 };
//	if (typeOfObject == 12)
//		conditional_size_units_ = { 386, 428 };
//	if (typeOfObject == 13)
//		conditional_size_units_ = { 425, 291 };
//	if (typeOfObject == 14)
//		conditional_size_units_ = { 87, 183 };
//	if (typeOfObject == 15)
//		conditional_size_units_ = { 113, 202 };
//	if (typeOfObject == 16)
//		conditional_size_units_ = { 67, 158 };
//	if (typeOfObject == 17)
//		conditional_size_units_ = { 119, 171 };
//	if (typeOfObject == 18)
//		conditional_size_units_ = { 178, 170 };
//	if (typeOfObject == 19)
//		conditional_size_units_ = { 188, 129 };
//	if (typeOfObject == 20)
//		conditional_size_units_ = { 136, 171 };
//	if (typeOfObject == 21)
//		conditional_size_units_ = { 171, 288 };
//	if (typeOfObject == 22)
//		conditional_size_units_ = { 491, 209 };
//	if (typeOfObject == 23)
//		conditional_size_units_ = { 166, 124 };
//	if (typeOfObject == 24)
//		conditional_size_units_ = { 78, 255 };
//	if (typeOfObject == 25)
//		conditional_size_units_ = { 171, 173 };
//	if (typeOfObject == 26)
//		conditional_size_units_ = { 123, 137 };
//	if (typeOfObject == 27)
//		conditional_size_units_ = { 133, 136 };
//	if (typeOfObject == 28)
//		conditional_size_units_ = { 91, 112 };
//	if (typeOfObject == 29)
//		conditional_size_units_ = { 119, 125 };
//	if (typeOfObject == 30)
//		conditional_size_units_ = { 134, 84 };
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
//Vector2f grass::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
//{
//	return { -1, -1 };
//}
//
//int grass::get_build_type(Vector2f ounPos, Vector2f otherPos)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> grass::prepare_sprites(long long elapsedTime)
//{
//	std::vector<sprite_chain_element*> result = {};
//	sprite_chain_element* body = new sprite_chain_element(pack_tag::darkWoods, pack_part::plant, direction::DOWN, type_of_object_, position_, conditional_size_units_, Vector2f(texture_box_offset_));
//	body->unscaled = true;
//
//	if (type_of_object_ >= 1 && type_of_object_ <= 8)
//		body->pack_tag = pack_tag::birchGrove;
//	if (type_of_object_ >= 9 && type_of_object_ <= 21)
//	{
//		body->pack_tag = pack_tag::darkWoods;
//		body->number -= 8;
//	}
//	if (type_of_object_ >= 22 && type_of_object_ <= 30)
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