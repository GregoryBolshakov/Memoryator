//#include "fern.h"
//
//#include "helper.h"
//
//fern::fern(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : picked_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 1;
//	this->type_of_object_ = typeOfObject;
//	to_save_name_ = "fern";
//	fern::setType(typeOfObject);
//	tag = entity_tag::fern;
//
//	count = 1;
//	id = entity_tag::fern;
//}
//
//void fern::setType(int typeOfObject)
//{
//	if (typeOfObject == -1)
//		return;
//
//	this->type_of_object_ = typeOfObject;
//	if (typeOfObject == 1)
//		conditional_size_units_ = { 330, 370 };
//}
//
//Vector2f fern::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//	return { texture_box_.width / 2.0f, texture_box_.height / 1.7f };
//}
//
//void fern::init_pedestal()
//{
//	focus1_ = position_;
//	focus2_ = position_;
//	ellipse_size_multipliers[0] = { 0 };
//	init_micro_blocks();
//}
//
//Vector2f fern::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
//{
//	return { -1, -1 };
//}
//
//Vector2f fern::getEnterPosition() const
//{
//	return { position_.x, position_.y + 1 };
//}
//
//int fern::get_build_type(Vector2f ounPos, Vector2f otherPos)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> fern::prepare_sprites(long long elapsedTime)
//{
//    return {};
//	/*additionalSprites.clear();
//	spriteChainElement body;
//	body->path = "Game/worldSprites/terrainObjects/fern/fern" + std::to_string(typeOfObject) + ".png";
//	body->size = Vector2f(conditionalSizeUnits);
//	body->offset = Vector2f(textureBoxOffset);
//	additionalSprites.push_back(body);*/
//}