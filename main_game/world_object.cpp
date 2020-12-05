#include "world_object.h"
#include "sprite_chain_element.h"
#include "helper.h"

sf::Vector2f world_object::micro_block_size = {20, 20};

world_object::world_object(std::string name, const sf::Vector2f position, const int kind) :
	  color(sf::Color(255, 255, 255))
	, tag{ entity_tag::empty_object }
	, kind_(kind)
	, z_coordinate_(1)
	, current_sprite_{1}
	, time_for_new_sprite_(0)
	, animation_speed_(0)
	, max_health_point_value_(0)
	, health_(0)
	, armor_(0)
	, name_{std::move(name)}
	, position_{ position }
	, radius_(0)
	, state_(common)
{
	world_object::init_pedestal();
}

world_object::~world_object()
= default;

void world_object::set_isometric_state(const std::vector<unique_ptr<sprite_chain_element>>& items, const bool state)
{
	for (const auto& item : items) item->isometric = state;
}

void world_object::cancel_mirroring()
{
	if (!mirrored_)
		return;

	offset_.x = size_.x - offset_.x;
	mirrored_ = false;
}

void world_object::init_pedestal()
{
	init_route_blocks();
}

void world_object::init_route_blocks()
{
}

bool world_object::is_locked_place(const std::map<std::pair<int, int>, bool>& check_blocks)
{
	for (auto& block : locked_route_blocks_)
		if (check_blocks.count({ block.x, block.y }) > 0)
			return true;
	
	return false;
}

void world_object::take_damage(const float damage, sf::Vector2f attacker_pos)
{
	this->health_ -= damage / this->armor_;
}

void world_object::on_sprite_change()
{
}

float world_object::get_max_health_point_value() const
{
	return max_health_point_value_;
}

