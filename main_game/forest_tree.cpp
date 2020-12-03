#include "forest_tree.h"
#include "direction_system.h"
#include "helper.h"
#include "sprite_chain_element.h"

sf::Vector2f calculate_size(const int kind)
{
	if (kind == 1)
		return { 396, 1090 };
	if (kind == 2)
		return { 536, 1150 };
	if (kind == 3)
		return { 244, 799 };
	if (kind == 4)
		return { 623, 875 };
	if (kind == 5)
		return { 644, 1235 };
	if (kind == 6)
		return { 681, 1027 };
	if (kind == 7)
		return { 616, 1169 };
	if (kind == 8)
		return { 457, 1152 };
	if (kind == 9)
		return { 527, 1188 };
	if (kind == 10)
		return { 786, 1296 };
	if (kind == 11)
		return { 806, 1292 };
	if (kind == 12)
		return { 795, 1190 };
	if (kind == 13)
		return { 1221, 386 };
	if (kind == 14)
		return { 952, 962 };
	if (kind == 15)
		return { 565, 1261 };
	if (kind == 16)
		return { 914, 1108 };
	if (kind == 17)
		return { 652, 1359 };
	if (kind == 18)
		return { 667, 717 };

	return { 0, 0 };
}

sf::Vector2f calculate_offset(int kind, sf::Vector2f size)
{
	switch (kind)
	{
	case 1:
		return { size.x * 0.257f, size.y * 0.932f };
	case 2:
		return { size.x * 0.45f, size.y * 0.96f };
	case 3:
		return { size.x * 0.55f, size.y * 0.96f };
	case 4:
		return { size.x * 0.409f, size.y * 0.945f };
	case 5:
		return { size.x * 0.41f, size.y * 0.935f };
	case 6:
		return { size.x * 0.536f, size.y * 0.94f };
	case 7:
		return { size.x * 0.526f, size.y * 0.936f };
	case 8:
		return { size.x * 0.560f, size.y * 0.96f };
	case 9:
	case 10:
		return { size.x * 0.467f, size.y * 0.94f };
	case 11:
		return { size.x * 0.554f, size.y * 0.95f };
	case 12:
		return { size.x * 0.327f, size.y * 0.93f };
	case 13:
		return { size.x * 0.574f, size.y * 0.508f };
	case 14:
		return { size.x * 0.521f, size.y * 0.883f };
	case 15:
		return { size.x * 0.509f, size.y * 0.857f };
	case 16:
		return { size.x * 0.537f, size.y * 0.83f };
	case 17:
		return { size.x * 0.349f, size.y * 0.861f };
	case 18:
		return { size.x * 0.439f, size.y * 0.857f };
	default:
		return { size.x * 0.45f, size.y * 0.96f };
	}
}

forest_tree::forest_tree(std::string name, const sf::Vector2f position, const int kind) : terrain_object(std::move(name), position, kind)
{
	// kinds: BirchGrove: 1-7; DarkWoods: 8-13; SwampyTrees: 14-18
	radius_ = 50;
	animation_speed_ = 10;
	size_ = calculate_size(kind);
	offset_ = calculate_offset(kind, size_);
	tag = entity_tag::tree;
}

void forest_tree::init_pedestal()
{
	std::pair<sf::Vector2f, sf::Vector2f> microEllipse;
	switch (kind_)
	{
		case 1:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.158f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.158f, position_.y);
			ellipse_size_multipliers[0] = { 1.44f };
			break;
		case 2:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.07f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.07f, position_.y);
			ellipse_size_multipliers[0] = { 1.58f };
			break;
		case 3:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.184f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.184f, position_.y);
			ellipse_size_multipliers[0] = { 1.42f };
			break;
		case 4:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.082f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.082f, position_.y);
			ellipse_size_multipliers[0] = { 1.42f };
			break;
		case 5:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.088f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.088f, position_.y);
			ellipse_size_multipliers[0] = { 1.57f };
			break;
		case 6:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.084f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.084f, position_.y);
			ellipse_size_multipliers[0] = { 1.31f };
			break;
		case 7:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.131f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.131f, position_.y);
			ellipse_size_multipliers[0] = { 1.28f };
			break;
		case 8:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.118f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.118f, position_.y);
			ellipse_size_multipliers[0] = { 1.42f };
			break;
		case 9:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.124f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.124f, position_.y);
			ellipse_size_multipliers[0] = { 1.21f };
			break;
		case 10:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.127f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.127f, position_.y);
			ellipse_size_multipliers[0] = { 1.25f };
			break;
		case 11:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.163f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.163f, position_.y);
			ellipse_size_multipliers[0] = { 1.25f };
			break;
		case 12:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.137f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.137f, position_.y);
			ellipse_size_multipliers[0] = { 1.25f };
			break;
		case 13:
			this->is_multi_ellipse = true;
			focus1_ = sf::Vector2f(position_.x, position_.y);
			focus2_ = sf::Vector2f(position_.x, position_.y);

			microEllipse.first = sf::Vector2f(position_.x - size_.x * 0.42f, position_.y + size_.y * 0.247f);
			microEllipse.second = sf::Vector2f(position_.x - size_.x * 0.178f, position_.y + size_.y * 0.247f);
			internal_ellipses.push_back(microEllipse);

			microEllipse.first = sf::Vector2f(position_.x - size_.x * 0.235f, position_.y + size_.y * 0.171f);
			microEllipse.second = sf::Vector2f(position_.x - size_.x * 0.026f, position_.y + size_.y * 0.171f);
			internal_ellipses.push_back(microEllipse);

			microEllipse.first = sf::Vector2f(position_.x - size_.x * 0.044f, position_.y + size_.y * 0.047f);
			microEllipse.second = sf::Vector2f(position_.x + size_.x * 0.132f, position_.y + size_.y * 0.047f);
			internal_ellipses.push_back(microEllipse);

			microEllipse.first = sf::Vector2f(position_.x + size_.x * 0.14f, position_.y - size_.y * 0.012f);
			microEllipse.second = sf::Vector2f(position_.x + size_.x * 0.385f, position_.y - size_.y * 0.012f);
			internal_ellipses.push_back(microEllipse);
			ellipse_size_multipliers = { 1.03f, 1.09f, 1.27f, 1.12f };
			break;
		case 14:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.079f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.079f, position_.y);
			ellipse_size_multipliers[0] = { 1.42f };
			break;
		case 15:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.09f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.09f, position_.y);
			ellipse_size_multipliers[0] = { 1.55f };
			break;
		case 16:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.066f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.066f, position_.y);
			ellipse_size_multipliers[0] = { 1.46f };
			break;
		case 17:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.162f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.162f, position_.y);
			ellipse_size_multipliers[0] = { 1.25f };
			break;
		case 18:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.109f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.109f, position_.y);
			ellipse_size_multipliers[0] = { 1.25f };
			break;
		default:
			focus1_ = sf::Vector2f(position_.x - size_.x * 0.106f, position_.y);
			focus2_ = sf::Vector2f(position_.x + size_.x * 0.106f, position_.y);
			ellipse_size_multipliers[0] = { 1.82f };
			break;
	}
	init_route_blocks();
}

std::vector<std::unique_ptr<sprite_chain_element>> forest_tree::prepare_sprites(const long long elapsed_time)
{
	std::vector<std::unique_ptr<sprite_chain_element>> result;
	auto body = make_unique<sprite_chain_element>(pack_tag::darkWoods, pack_part::tree, direction::DOWN, kind_, position_, size_, sf::Vector2f(offset_), color, mirrored_);
	if (kind_ == 13)
		body->isometric = true;
	
	if (kind_ >= 1 && kind_ <= 7)
		body->pack_tag = pack_tag::birchGrove;
	if (kind_ >= 8 && kind_ <= 13)
	{
		body->pack_tag = pack_tag::darkWoods;
		body->number -= 7;
	}
	if (kind_ >= 14 && kind_ <= 18)
	{
		body->pack_tag = pack_tag::swampyTrees;
		body->number -= 13;
	}

	if (state_ == absorbed)
	{
		body->animation_length = 15;
		color.a = 255 - current_sprite_[0] * 255 / body->animation_length;
	}

	time_for_new_sprite_ += elapsed_time;

	if (time_for_new_sprite_ >= long(1e6 / animation_speed_))
	{
		time_for_new_sprite_ = 0;

		if (++current_sprite_[0] > body->animation_length)
		{
			if (state_ == absorbed)
			{
				state_ = common;
				delete_promise_on();
			}
			current_sprite_[0] = 1;
		}
	}

	result.emplace_back(std::move(body));

	return result;
}