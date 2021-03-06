//#include "fence.h"
//
//#include "helper.h"
//
//fence::fence(std::string objectName, const sf::Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
//{
//	variety_of_types_ = 3;
//	this->type_ = typeOfObject;
//	radius_ = 120;
//	to_save_name_ = "fence";
//	fence::setType(typeOfObject);
//	is_dots_adjusted = true;
//	tag = entity_tag::fence;
//}
//
//void fence::setType(const int typeOfObject)
//{
//	this->type_ = typeOfObject;
//	switch (typeOfObject)
//	{
//		case 1:
//		case 2:
//		{
//			this->size_ = { 240, 230 };
//			break;
//		}
//		case 3:
//		case 4:
//		{
//			this->size_ = { 95, 350 };
//			break;
//		}
//		default:
//		{
//			this->size_ = { 300, 300 };
//			break;
//		}
//	}
//}
//
//Vector2f fence::calculate_texture_offset()
//{
//	texture_box_.width = texture_box_.width * get_scale_ratio().x;
//	texture_box_.height = texture_box_.height * get_scale_ratio().y;
//	switch (type_)
//	{
//		case 1:		
//		case 2:
//			return { texture_box_.width / 2.0f, texture_box_.height / 1.0f };
//		case 3:
//			return { texture_box_.width / 2.0f, texture_box_.height / 1.5f };
//		case 4:
//		default:
//			return { texture_box_.width / 2.0f, texture_box_.height / 1.0f };
//	}
//}
//
//void fence::init_pedestal()
//{
//	switch (type_)
//	{
//		case 1:
//		case 2:
//		{
//			dot1_ = sf::Vector2f(position_.x - texture_box_.width / 3.2f, position_.y);
//			dot2_ = sf::Vector2f(position_.x + texture_box_.width / 1.8f, position_.y);
//			break;
//		}
//		case 3:
//		{
//			dot1_ = sf::Vector2f(position_.x, position_.y - texture_box_.height / 9.0f);
//			dot2_ = sf::Vector2f(position_.x, position_.y + texture_box_.height / 2.8f);
//			break;
//		}
//		case 4:
//		{
//			dot1_ = sf::Vector2f(position_.x - texture_box_.width / 4.0f, position_.y/* - textureBox.height / 7*/);
//			dot2_ = sf::Vector2f(position_.x - texture_box_.width / 4.0f, position_.y + texture_box_.height / 2.0f);
//			break;
//		}
//		default:
//		{
//			dot1_ = sf::Vector2f(position_.x - texture_box_.width / 2.0f, position_.y);
//			dot2_ = sf::Vector2f(position_.x + texture_box_.width / 2.0f, position_.y);
//			break;
//		}
//	}
//
//	//radius = sqrt(pow(dot1.x - dot2.x, 2) + pow(dot1.y + dot2.y, 2)) / 2 - 10;
//}
//
//Vector2f fence::get_build_position(std::vector<world_object*> visibleItems, const float scaleFactor, const sf::Vector2f cameraPosition)
//{
//	const auto mousePos = sf::Vector2f (sf::Mouse::getPosition());
//	const auto mouseWorldPos = sf::Vector2f ((mousePos.x - world_metrics::window_size.x / 2 + cameraPosition.x * scaleFactor) / scaleFactor,
//	                                     (mousePos.y - world_metrics::window_size.y / 2 + cameraPosition.y * scaleFactor) / scaleFactor);
//
//	const auto dot1 = sf::Vector2f ((this->dot1_.x - this->position_.x) + mouseWorldPos.x, (this->dot1_.y - this->position_.y) + mouseWorldPos.y);
//	const auto dot2 = sf::Vector2f ((this->dot2_.x - this->position_.x) + mouseWorldPos.x, (this->dot2_.y - this->position_.y) + mouseWorldPos.y);
//
//	for (auto&item : visibleItems)
//	{
//		if (item->tag == entity_tag::fence)
//		{
//			const auto object = dynamic_cast<fence*>(item);
//
//			auto const dist1 = sqrt(pow(dot1.x - object->get_dot2().x, 2) + pow(dot1.y - object->get_dot2().y, 2));
//			auto const dist2 = sqrt(pow(dot2.x - object->get_dot1().x, 2) + pow(dot2.y - object->get_dot1().y, 2));
//			auto const dist3 = sqrt(pow(dot1.x - object->get_dot1().x, 2) + pow(dot1.y - object->get_dot1().y, 2));
//			auto const dist4 = sqrt(pow(dot2.x - object->get_dot2().x, 2) + pow(dot2.y - object->get_dot2().y, 2));
//
//			auto ownDot = sf::Vector2f (-1, -1);
//			auto objDot = sf::Vector2f (-1, -1);
//
//			if (dist1 <= dist2 && dist1 <= dist3 && dist1 <= dist4 && dist1 < 100)
//			{
//				ownDot = sf::Vector2f (this->position_.x - this->dot1_.x, this->position_.y - this->dot1_.y);
//				objDot = sf::Vector2f (object->get_dot2());				
//			}
//			else if (dist2 <= dist1 && dist2 <= dist3 && dist2 <= dist4 && dist2 < 100)
//			{
//				ownDot = sf::Vector2f (this->position_.x - this->dot2_.x, this->position_.y - this->dot2_.y);
//				objDot = sf::Vector2f (object->get_dot1());				
//			}
//			else if (dist3 <= dist1 && dist3 <= dist2 && dist3 <= dist4 && dist3 < 100)
//			{
//				ownDot = sf::Vector2f (this->position_.x - this->dot1_.x, this->position_.y - this->dot1_.y);
//				objDot = sf::Vector2f (object->get_dot1());				
//			}
//			else if (dist4 <= dist1 && dist4 <= dist2 && dist4 <= dist3 && dist4 < 100)
//			{
//				ownDot = sf::Vector2f (this->position_.x - this->dot2_.x, this->position_.y - this->dot2_.y);
//				objDot = sf::Vector2f (object->get_dot2());				
//			}	
//
//			if (ownDot != sf::Vector2f (-1, -1) && objDot != sf::Vector2f (-1, -1))
//			{			
//				current_dot_ = sf::Vector2f (objDot);
//				return sf::Vector2f (objDot.x + ownDot.x, objDot.y + ownDot.y);
//			}
//		}
//	}
//	return { -1, -1 };
//}
//
//int fence::get_build_type(const sf::Vector2f ounPos, const sf::Vector2f otherPos)
//{
//	if (otherPos != sf::Vector2f (-1, -1))
//	{
//		const auto side = helper::getSide(ounPos, otherPos);
//		if (side == up)
//			return 3;
//		if (side == down)
//			return 3;
//		if (side == left)
//			return 1;
//		if (side == right)
//			return 2;
//	}
//
//	return 1;
//}
//
//std::vector<sprite_chain_element*> fence::prepare_sprites(long long elapsedTime)
//{
//    return {};
//	/*additionalSprites.clear();
//	spriteChainElement fullSprite;
//	fullSprite.path = "Game/worldSprites/terrainObjects/fence/fence" + std::to_string(typeOfObject) + ".png";
//	fullSprite.size = sf::Vector2f(conditionalSizeUnits);
//	fullSprite.offset = sf::Vector2f(textureBoxOffset);
//	additionalSprites.push_back(fullSprite);*/
//}