//#include "mushroom.h"
//
//#include "helper.h"
//
//mushroom::mushroom(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : static_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 16; // BirchGrove: 1-3; DarkWoods: 4-12; SwampyTrees: 13-16
//	this->type_of_object_ = typeOfObject;
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
//	this->type_of_object_ = typeOfObject;
//	if (typeOfObject == 1)
//		conditional_size_units_ = { 72, 140 };
//	if (typeOfObject == 2)
//		conditional_size_units_ = { 67, 178 };
//	if (typeOfObject == 3)
//		conditional_size_units_ = { 82, 94 };
//	if (typeOfObject == 4)
//		conditional_size_units_ = { 143, 203 };
//	if (typeOfObject == 5)
//		conditional_size_units_ = { 120, 134 };
//	if (typeOfObject == 6)
//		conditional_size_units_ = { 173, 161 };
//	if (typeOfObject == 7)
//		conditional_size_units_ = { 70, 134 };
//	if (typeOfObject == 8)
//		conditional_size_units_ = { 91, 81 };
//	if (typeOfObject == 9)
//		conditional_size_units_ = { 94, 78 };
//	if (typeOfObject == 10)
//		conditional_size_units_ = { 103, 137 };
//	if (typeOfObject == 11)
//		conditional_size_units_ = { 89, 71 };
//	if (typeOfObject == 12)
//		conditional_size_units_ = { 144, 122 };
//	if (typeOfObject == 13)
//		conditional_size_units_ = { 54, 60 };
//	if (typeOfObject == 14)
//		conditional_size_units_ = { 80, 74 };
//	if (typeOfObject == 15)
//		conditional_size_units_ = { 80, 80 };
//	if (typeOfObject == 16)
//		conditional_size_units_ = { 77, 53 };
//}
//
//Vector2f mushroom::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//
//	if (type_of_object_ == 0)
//		return { 0, 0 };
//
//	return { texture_box_.width / 2.0f, texture_box_.height / 2.0f };
//}
//
//Vector2f mushroom::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
//{
//	return { -1, -1 };
//}
//
//int mushroom::get_build_type(Vector2f ounPos, Vector2f otherPos)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> mushroom::prepare_sprites(long long elapsedTime)
//{
//	sprite_chain_element* body = new sprite_chain_element(pack_tag::darkWoods, pack_part::mushroom, direction::DOWN, type_of_object_, position_, conditional_size_units_, Vector2f(texture_box_offset_));
//	body->unscaled = true;
//
//	if (type_of_object_ >= 1 && type_of_object_ <= 3)
//		body->pack_tag = pack_tag::birchGrove;
//	if (type_of_object_ >= 4 && type_of_object_ <= 12)
//	{
//		body->pack_tag = pack_tag::darkWoods;
//		body->number -= 3;
//	}
//	if (type_of_object_ >= 13 && type_of_object_ <= 16)
//	{
//		body->pack_tag = pack_tag::swampyTrees;
//		body->number -= 12;
//	}
//
//	return { body };
//}