//#include "rock.h"
//
//#include "helper.h"
//
//rock::rock(std::string objectName, const sf::Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 18; // BirchGrove: 1-8; DarkWoods: 9-13; SwampyTrees: 14-18
//	this->type_ = typeOfObject;
//	strength = 0;
//	radius_ = 50;
//	animation_speed_ = 10;
//	to_save_name_ = "rock";
//	rock::setType(typeOfObject);
//	tag = entity_tag::rock;
//}
//
//void rock::setType(const int typeOfObject)
//{
//	this->type_ = typeOfObject;
//	if (typeOfObject == 1)
//		size_ = { 395, 231 };
//	if (typeOfObject == 2)
//		size_ = { 579, 294 };
//	if (typeOfObject == 3)
//		size_ = { 343, 95 };
//	if (typeOfObject == 4)
//		size_ = { 236, 106 };
//	if (typeOfObject == 5)
//		size_ = { 269, 116 };
//	if (typeOfObject == 6)
//		size_ = { 349, 113 };
//	if (typeOfObject == 7)
//		size_ = { 278, 204 };
//	if (typeOfObject == 8)
//		size_ = { 417, 113 };
//	if (typeOfObject == 9)
//		size_ = { 428, 259 };
//	if (typeOfObject == 10)
//		size_ = { 474, 463 };
//	if (typeOfObject == 11)
//		size_ = { 314, 116 };
//	if (typeOfObject == 12)
//		size_ = { 371, 205 };
//	if (typeOfObject == 13)
//		size_ = { 259, 144 };
//	if (typeOfObject == 14)
//		size_ = { 278, 190 };
//	if (typeOfObject == 15)
//		size_ = { 300, 175 };
//	if (typeOfObject == 16)
//		size_ = { 173, 154 };
//	if (typeOfObject == 17)
//		size_ = { 406, 208 };
//	if (typeOfObject == 18)
//		size_ = { 254, 173 };
//
//	radius_ = std::max(size_.x, size_.y) / 2;
//}
//
//Vector2f rock::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//
//	switch (type_)
//	{
//	case 1:
//		return { texture_box_.width * 0.58f, texture_box_.height * 0.764f };
//	case 2:
//		return { texture_box_.width * 0.5f, texture_box_.height * 0.673f };
//	case 3:
//		return { texture_box_.width * 0.551f, texture_box_.height * 0.01f };
//	case 4:
//		return { texture_box_.width * 0.59f, texture_box_.height * 0.01f };
//	case 5:
//		return { texture_box_.width * 0.488f, texture_box_.height * 0.664f };
//	case 6:
//		return { texture_box_.width * 0.49f, texture_box_.height * 0.624f };
//	case 7:
//		return { texture_box_.width * 0.506f, texture_box_.height * 0.632f };
//	case 8:
//		return { texture_box_.width * 0.539f, texture_box_.height * 0.637f };
//	case 9:
//		return { texture_box_.width * 0.455f, texture_box_.height * 0.68f };
//	case 10:
//		return { texture_box_.width * 0.512f, texture_box_.height * 0.717f };
//	case 11:
//		return { texture_box_.width * 0.488f, texture_box_.height * 0.772f };
//	case 12:
//		return { texture_box_.width * 0.478f, texture_box_.height * 0.633f };
//	case 13:
//		return { texture_box_.width * 0.491f, texture_box_.height * 0.569f };
//	case 14:
//		return { texture_box_.width * 0.5f, texture_box_.height * 0.673f };
//	case 15:
//		return { texture_box_.width * 0.491f, texture_box_.height * 0.634f };
//	case 16:
//		return { texture_box_.width * 0.515f, texture_box_.height * 0.809f };
//	case 17:
//		return { texture_box_.width * 0.559f, texture_box_.height * 0.68f };
//	case 18:
//		return { texture_box_.width * 0.478f, texture_box_.height * 0.633f };
//	default:
//		return { texture_box_.width * 0.409f, texture_box_.height * 0.945f };
//	}
//}
//
//void rock::init_pedestal()
//{
//	switch (type_)
//	{
//	case 1:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.293f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.293f, position_.y);
//		ellipse_size_multipliers[0] = { 1.21f };
//		break;
//	case 2:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.38f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.38f, position_.y);
//		ellipse_size_multipliers[0] = { 1.11f };
//		break;
//	case 3:
//		focus1_ = sf::Vector2f(position_.x, position_.y);
//		focus2_ = sf::Vector2f(position_.x, position_.y);
//		ellipse_size_multipliers[0] = { 0 };
//		break;
//	case 4:
//		focus1_ = sf::Vector2f(position_.x, position_.y);
//		focus2_ = sf::Vector2f(position_.x, position_.y);
//		ellipse_size_multipliers[0] = { 0 };
//		break;
//	case 5:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.342f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.342f, position_.y);
//		ellipse_size_multipliers[0] = { 1.16f };
//		break;
//	case 6:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.311f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.311f, position_.y);
//		ellipse_size_multipliers[0] = { 1.15f };
//		break;
//	case 7:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.32f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.32f, position_.y);
//		ellipse_size_multipliers[0] = { 1.3f };
//		break;
//	case 8:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.36f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.36f, position_.y);
//		ellipse_size_multipliers[0] = { 1.1f };
//		break;	
//	case 9:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.315f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.315f, position_.y);
//		ellipse_size_multipliers[0] = { 1.23f };
//		break;
//	case 10:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.302f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.302f, position_.y);
//		ellipse_size_multipliers[0] = { 1.26f };
//		break;
//	case 11:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.229f, position_.y - texture_box_.height * 0.092f);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.229f, position_.y - texture_box_.height * 0.092f);
//		ellipse_size_multipliers[0] = { 1.64f };
//		break;
//	case 12:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.386f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.386f, position_.y);
//		ellipse_size_multipliers[0] = { 1.15f };
//		break;
//	case 13:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.273f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.273f, position_.y);
//		ellipse_size_multipliers[0] = { 1.64f };
//		break;
//	case 14:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.315f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.315f, position_.y);
//		ellipse_size_multipliers[0] = { 1.23f };
//		break;
//	case 15:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.354f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.354f, position_.y);
//		ellipse_size_multipliers[0] = { 1.14f };
//		break;
//	case 16:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.378f, position_.y - texture_box_.height * 0.101f);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.378f, position_.y - texture_box_.height * 0.101f);
//		ellipse_size_multipliers[0] = { 1.3f };
//		break;
//	case 17:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.386f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.386f, position_.y);
//		ellipse_size_multipliers[0] = { 1.15f };
//		break;
//	case 18:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.38f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.38f, position_.y);
//		ellipse_size_multipliers[0] = { 1.08f };
//		break;
//	default:
//		focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.111f, position_.y);
//		focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.111f, position_.y);
//		ellipse_size_multipliers[0] = { 1.78f };
//		break;
//	}
//	init_route_blocks();
//}
//
//Vector2f rock::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition)
//{
//	return { -1, -1 };
//}
//
//int rock::get_build_type(sf::Vector2f ounPos, sf::Vector2f otherPos)
//{
//	return 1;
//}
//
//std::vector<sprite_chain_element*> rock::prepare_sprites(long long elapsedTime)
//{
//	auto body = new sprite_chain_element(pack_tag::darkWoods, pack_part::rock, direction::DOWN, type_, position_, size_, sf::Vector2f(offset_), color, mirrored_);
//
//	if (type_ >= 1 && type_ <= 8)
//		body->pack_tag = pack_tag::birchGrove;
//	if (type_ >= 9 && type_ <= 13)
//	{
//		body->pack_tag = pack_tag::darkWoods;
//		body->number -= 8;
//	}
//	if (type_ >= 14 && type_ <= 18)
//	{
//		body->pack_tag = pack_tag::swampyTrees;
//		body->number -= 13;
//	}
//
//	set_unscaled({ body });
//	
//	return { body };
//}