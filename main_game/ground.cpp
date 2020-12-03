#include "ground.h"
#include "direction_system.h"
#include "sprite_chain_element.h"
#include "world_metrics.h"
#include "helper.h"

ground::ground(std::string name, const sf::Vector2f position, const int kind) : static_object(std::move(name), position, kind)
{
	tag = entity_tag::ground;
	size_ = world_metrics::block_size;
	offset_ = { 0, 0 };
	z_coordinate_ = kind_ * 10 + 1;
	is_background = true;
}

std::vector<unique_ptr<sprite_chain_element>> ground::prepare_sprites(long long)
{
	std::vector<unique_ptr<sprite_chain_element>> result;
	auto body = make_unique<sprite_chain_element>(sprite_chain_element(pack_tag::darkWoods, pack_part::ground, direction::DOWN, 1, position_, size_, sf::Vector2f(offset_)));
	
	body->z_coordinate = z_coordinate_;
	body->is_background = true;

	if (kind_ == 1)
		body->pack_tag = pack_tag::swampyTrees;
	if (kind_ == 2)
		body->pack_tag = pack_tag::darkWoods;
	if (kind_ == 3)
		body->pack_tag = pack_tag::birchGrove;

	result.push_back(std::move(body));
	return result;
}