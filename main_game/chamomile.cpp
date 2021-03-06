//#include "chamomile.h"
//
//#include "helper.h"
//
//chamomile::chamomile(std::string objectName, const sf::Vector2f centerPosition, const int typeOfObject) : picked_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 5;
//	this->type_ = typeOfObject;
//	to_save_name_ = "chamomile";
//	chamomile::setType(typeOfObject);
//	tag = entity_tag::chamomile;
//
//	count = 1;
//	id = entity_tag::chamomile;	
//}
//
//void chamomile::setType(int typeOfObject)
//{
//	if (typeOfObject == -1)
//		return;
//
//	this->type_ = typeOfObject;
//	if (typeOfObject == 1)
//		size_ = { 297, 263 };
//	if (typeOfObject == 2)
//		size_ = { 279, 281 };
//	if (typeOfObject == 3)
//		size_ = { 313, 264 };
//	if (typeOfObject == 4)
//		size_ = { 223, 267 };
//	if (typeOfObject == 5)
//		size_ = { 203, 206 };
//}
//
//Vector2f chamomile::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//	return { texture_box_.width / 2.0f, texture_box_.height / 1.2f };
//}
//
//void chamomile::init_pedestal()
//{
//	focus1_ = position_;
//	focus2_ = position_;
//	ellipse_size_multipliers[0] = { 0 };
//	init_route_blocks();
//}
//
//Vector2f chamomile::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition)
//{
//	return { -1, -1 };
//}
//
//int chamomile::get_build_type(sf::Vector2f ounPos, sf::Vector2f otherPos)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> chamomile::prepare_sprites(long long elapsedTime)
//{
//    return {};
//    /*additionalSprites.clear();
//	spriteChainElement body;
//	body->path = "Game/worldSprites/terrainObjects/chamomile/chamomile" + std::to_string(typeOfObject) + ".png";
//	body->size = sf::Vector2f(conditionalSizeUnits);
//	body->offset = sf::Vector2f(textureBoxOffset);
//	additionalSprites.push_back(body);*/
//}