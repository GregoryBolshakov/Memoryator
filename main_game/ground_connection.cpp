#include "ground_connection.h"
#include "direction_system.h"
#include "sprite_chain_element.h"
#include "world_metrics.h"

sf::Vector2f ground_connection::calculate_size(const int kind)
{
	if (kind % 4 == 1)
		return { world_metrics::block_size.x, world_metrics::block_size.y / 4 };
	if (kind % 4 == 2)
		return { world_metrics::block_size.x, world_metrics::block_size.y / 4 };
	if (kind % 4 == 3)
		return { world_metrics::block_size.x / 4, world_metrics::block_size.y };
	if (kind % 4 == 0)
		return { world_metrics::block_size.x / 4, world_metrics::block_size.y };
	return { 0, 0 };
}

sf::Vector2f ground_connection::calculate_offset(const int kind, const sf::Vector2f size)
{
	if (kind % 4 == 1)
		return { 0, size.y - 5 };
	if (kind % 4 == 2)
		return { 0, 5 };
	if (kind % 4 == 3)
		return { size.x - 5, 0 };
	if (kind % 4 == 0)
		return { 5, 0 };
	return { 0, 0 };
}

int ground_connection::calculate_z_coord(const int kind)
{
	if (kind >= 1 && kind <= 4)
		return  10;
	if (kind >= 5 && kind <= 8)
		return  20;
	if (kind >= 9 && kind <= 12)
		return  30;
	if (kind >= 13 && kind <= 16)
		return 40;
	if (kind >= 17 && kind <= 20)
		return  50;
	return 0;
}

ground_connection::ground_connection(std::string name, const sf::Vector2f position, const int kind) : static_object(std::move(name), position, kind)
{
	// kinds: SwampyTrees: 1-4; DarkWoods: 5-8; BirchGrove: 9-12
	tag = entity_tag::ground_connection;
	size_ = calculate_size(kind_);
	offset_ = calculate_offset(kind_, size_);
	z_coordinate_ = calculate_z_coord(kind_);
	is_background = true;
}

std::vector<unique_ptr<sprite_chain_element>> ground_connection::prepare_sprites(long long)
{
	std::vector<unique_ptr<sprite_chain_element>> result;

	if (kind_ >= 1 && kind_ <= 4)
	{
		result.emplace_back(std::move(make_unique<sprite_chain_element>()));
		return result;
	}

	int spriteType = kind_ % 4 + 1;
	if (spriteType == 1)
		spriteType = 5;

	auto body = make_unique<sprite_chain_element>(pack_tag::darkWoods, pack_part::ground, direction::DOWN, spriteType, position_, size_, sf::Vector2f(offset_));
	body->z_coordinate = z_coordinate_;

	//if (typeOfObject >= 1 && typeOfObject <= 4)
		//body->packTag = PackTag::darkWoods;
	if (kind_ >= 5 && kind_ <= 8)
		body->pack_tag = pack_tag::darkWoods;
	if (kind_ >= 9 && kind_ <= 12)
		body->pack_tag = pack_tag::birchGrove;

	body->is_background = true;

	result.emplace_back(std::move(body));
	return result;
}