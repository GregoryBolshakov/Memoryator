//#include "yarrow.h"
//
//#include "helper.h"
//
//yarrow::yarrow(std::string objectName, const sf::Vector2f centerPosition, const int typeOfObject) : picked_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 5;
//	this->type_ = typeOfObject;
//	to_save_name_ = "yarrow";
//	yarrow::setType(typeOfObject);
//	radius_ = 50;
//	tag = entity_tag::yarrow;
//
//	count = 1;
//	id = entity_tag::yarrow;
//}
//
//void yarrow::setType(int typeOfObject)
//{
//	if (typeOfObject == -1)
//		return;
//
//	this->type_ = typeOfObject;
//	if (typeOfObject == 1)
//		size_ = { 234, 358 };
//	if (typeOfObject == 2)
//		size_ = { 294, 410 };
//	if (typeOfObject == 3)
//		size_ = { 315, 351 };
//	if (typeOfObject == 4)
//		size_ = { 320, 337 };
//	if (typeOfObject == 5)
//		size_ = { 212, 531 };
//}
//
//Vector2f yarrow::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//	return { texture_box_.width / 2.0f, texture_box_.height / 1.2f };
//}
//
//void yarrow::init_pedestal()
//{
//	focus1_ = position_;
//	focus2_ = position_;
//	ellipse_size_multipliers[0] = { 0 };
//	init_route_blocks();
//}
//
//Vector2f yarrow::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition)
//{
//	return { -1, -1 };
//}
//
//int yarrow::get_build_type(sf::Vector2f ounPos, sf::Vector2f otherPos)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> yarrow::prepare_sprites(long long elapsedTime)
//{
//    return {};
//	/*additionalSprites.clear();
//	spriteChainElement yarrowBody;
//	yarrowBody.path = "Game/worldSprites/terrainObjects/yarrow/yarrow" + std::to_string(typeOfObject) + ".png";
//	yarrowBody.size = sf::Vector2f(conditionalSizeUnits);
//	yarrowBody.offset = sf::Vector2f(textureBoxOffset);
//	additionalSprites.push_back(yarrowBody);*/
//}