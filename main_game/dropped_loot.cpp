//#include "dropped_loot.h"
//
//#include "helper.h"
//
//dropped_loot::dropped_loot(std::string objectName, sf::Vector2f centerPosition, int typeOfObject, int count) : picked_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 5;
//	this->type_of_object_ = typeOfObject;
//	to_save_name_ = "drop";
//	dropped_loot::setType(typeOfObject);
//	radius_ = sprite_pack::icon_size.x / 2;
//	tag = entity_tag::droppedLoot;
//	this->count = count;
//}
//
//void dropped_loot::setType(int typeOfObject)
//{
//	if (typeOfObject == -1)
//		return;
//
//	this->type_of_object_ = typeOfObject;
//	id = entity_tag(typeOfObject);
//	conditional_size_units_ = sprite_pack::icon_size;
//}
//
//Vector2f dropped_loot::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//	return { texture_box_.width / 2.0f, texture_box_.height / 2.0f };
//}
//
//void dropped_loot::init_pedestal()
//{
//	focus1_ = position_;
//	focus2_ = position_;
//	ellipse_size_multipliers[0] = { 0 };
//}
//
//Vector2f dropped_loot::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition)
//{
//	return { -1, -1 };
//}
//
//int dropped_loot::get_build_type(sf::Vector2f ounPos, sf::Vector2f otherPos)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> dropped_loot::prepare_sprites(long long elapsedTime)
//{
//	sprite_chain_element* body = new sprite_chain_element(pack_tag::empty, pack_part::full, direction::DOWN, this->is_selected, position_, conditional_size_units_, texture_box_offset_);	
//
//	switch (type_of_object_)
//	{
//	case 401:
//		body->set_draw_info(pack_tag::icons, pack_part::flowers, direction::DOWN, body->number + 1);	
//		break;
//	case 402:
//		body->set_draw_info(pack_tag::icons, pack_part::flowers, direction::DOWN, body->number + 9);
//		break;
//	case 201:
//		body->set_draw_info(pack_tag::inventory, pack_part::bag1, direction::DOWN, body->number + 1);
//		break;
//	case 202:
//		body->set_draw_info(pack_tag::icons, pack_part::craftObjects, direction::DOWN, body->number + 5);
//		break;
//	case 219:
//		body->set_draw_info(pack_tag::icons, pack_part::notCraftObjects, direction::DOWN, 1);
//		break;
//	case 102:
//		body->set_draw_info(pack_tag::icons, pack_part::mobs, direction::DOWN, 3);
//		break;
//	default:
//		body->set_draw_info(pack_tag::inventory, pack_part::areas, direction::DOWN, 1);
//		break;
//	}
//
//	return { body };
//
//
//	
//
//}