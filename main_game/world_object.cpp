#include "world_object.h"
#include "sprite_chain_element.h"
#include "helper.h"

sf::Vector2f world_object::micro_block_size = {20, 20};

world_object::world_object(std::string object_name, const sf::Vector2f center_position) :
	  tag{ entity_tag::empty_object }
	, current_sprite_{std::vector<int>(1)}
	, name_{std::move(object_name)}
	, position_{ center_position }
{
	world_object::init_pedestal();
}

world_object::~world_object()
= default;

void world_object::set_unscaled(const std::vector<unique_ptr<sprite_chain_element>>& items)
{
	for (const auto& item : items) item->unscaled = true;
}

void world_object::set_texture_size(const sf::Vector2f texture_size)
{
	texture_box_.width = texture_size.x;
	texture_box_.height = texture_size.y;
	original_texture_box_ = texture_box_;
	texture_box_offset_ = calculate_texture_offset();

	texture_box_.left = ceil(position_.x - texture_box_offset_.x);
	texture_box_.top = ceil(position_.y - texture_box_offset_.y);
	
	init_pedestal();
}

sf::Vector2f world_object::get_scale_ratio() const
{
	return { float(conditional_size_units_.x) / original_texture_box_.width, float(conditional_size_units_.y) / original_texture_box_.height };
}

void world_object::cancel_mirroring()
{
	if (!mirrored_)
		return;

	texture_box_offset_.x = conditional_size_units_.x - texture_box_offset_.x;
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
	this->health_point_ -= damage / this->armor_;
}

void world_object::on_sprite_change()
{
}

float world_object::get_max_health_point_value() const
{
	return max_health_point_value_;
}

