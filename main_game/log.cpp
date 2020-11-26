//#include "log.h"
//
//#include "helper.h"
//
//namespace mr
//{
//	log::log(std::string objectName, const sf::Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
//	{
//		variety_of_types_ = 3;
//		this->type_of_object_ = typeOfObject;
//		strength = 0;
//		radius_ = 50;
//		animation_speed_ = 10;
//		to_save_name_ = "log";
//		log::setType(typeOfObject);
//		tag = entity_tag::log;
//	}
//
//	void log::setType(const int typeOfObject)
//	{
//		this->type_of_object_ = typeOfObject;
//		if (typeOfObject == 1)
//			conditional_size_units_ = { 687, 179 };
//		if (typeOfObject == 2)
//			conditional_size_units_ = { 507, 231 };
//		if (typeOfObject == 3)
//			conditional_size_units_ = { 509, 246 };
//	}
//
//	sf::Vector2f log::calculate_texture_offset()
//	{
//		texture_box_.width = texture_box_.width * get_scale_ratio().x;
//		texture_box_.height = texture_box_.height * get_scale_ratio().y;
//
//		switch (type_of_object_)
//		{
//		case 1:
//			return { texture_box_.width * 0.492f, texture_box_.height * 0.721f };
//		case 2:
//			return { texture_box_.width * 0.479f, texture_box_.height * 0.735f };
//		case 3:
//			return { texture_box_.width * 0.502f, texture_box_.height * 0.584f };
//		default:
//			return { texture_box_.width * 0.485f, texture_box_.height * 0.591f };
//		}
//	}
//
//	void log::init_pedestal()
//	{
//		std::pair<sf::Vector2f, sf::Vector2f> microEllipse;
//		switch (type_of_object_)
//		{
//		case 1:
//			this->is_multi_ellipse = true;
//			focus1_ = sf::Vector2f(position_.x, position_.y);
//			focus2_ = sf::Vector2f(position_.x, position_.y);
//
//			microEllipse.first = sf::Vector2f(position_.x - texture_box_.width * 0.42f, position_.y + texture_box_.height * 0.15f);
//			microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.073f, position_.y - texture_box_.height * 0.26f);
//			internal_ellipses.push_back(microEllipse);
//
//			microEllipse.first = sf::Vector2f(position_.x - texture_box_.width * 0.107f, position_.y - texture_box_.height * 0.197f);
//			microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.169f, position_.y - texture_box_.height * 0.197f);
//			internal_ellipses.push_back(microEllipse);
//
//			microEllipse.first = sf::Vector2f(position_.x + texture_box_.width * 0.033f, position_.y - texture_box_.height * 0.2f);
//			microEllipse.second = sf::Vector2f(position_.x + texture_box_.width * 0.407f, position_.y + texture_box_.height * 0.103f);
//			internal_ellipses.push_back(microEllipse);
//			ellipse_size_multipliers = { 1.03f, 1.11f, 1.05f };
//			break;
//		case 2:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.399f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.399f, position_.y);
//			ellipse_size_multipliers[0] = { 1.07f };
//			break;
//		case 3:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.396f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.396f, position_.y);
//			ellipse_size_multipliers[0] = { 1.12f };
//			break;
//		default:
//			focus1_ = sf::Vector2f(position_.x - texture_box_.width * 0.311f, position_.y);
//			focus2_ = sf::Vector2f(position_.x + texture_box_.width * 0.311f, position_.y);
//			ellipse_size_multipliers[0] = { 1.307f };
//			break;
//		}
//		init_route_blocks();
//	}
//
//	sf::Vector2f log::get_build_position(std::vector<world_object*>, float, sf::Vector2f)
//	{
//		return { -1, -1 };
//	}
//
//	int log::get_build_type(sf::Vector2f, sf::Vector2f)
//	{
//		return 1;
//	}
//
//	std::vector<sprite_chain_element*> log::prepare_sprites(long long)
//	{
//		const auto body = new sprite_chain_element(pack_tag::birchGrove, pack_part::log, direction::DOWN, type_of_object_, position_, conditional_size_units_, sf::Vector2f(texture_box_offset_), color, mirrored_);
//
//		set_unscaled({ body });
//		
//		return { body };
//	}
//}