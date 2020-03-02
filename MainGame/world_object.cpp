#include "world_object.h"

#include "helper.h"

Vector2f world_object::micro_block_size = {20, 20};

world_object::world_object(std::string object_name, const Vector2f center_position)
{
	name_ = std::move(object_name);
	position_ = center_position;
	world_object::init_pedestal();
	current_sprite_.resize(1);
	current_sprite_[0] = 1;
}

world_object::~world_object()
= default;

void world_object::set_texture_size(const Vector2f texture_size)
{
	texture_box_.width = texture_size.x;
	texture_box_.height = texture_size.y;
	original_texture_box_ = texture_box_;
	texture_box_offset_ = calculate_texture_offset();

	texture_box_.left = ceil(position_.x - texture_box_offset_.x);
	texture_box_.top = ceil(position_.y - texture_box_offset_.y);
	
	init_pedestal();
}

Vector2f world_object::get_scale_ratio() const
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
	init_micro_blocks();
}

void world_object::init_micro_blocks()
{
}

bool world_object::is_locked_place(const std::map<std::pair<int, int>, bool>& check_blocks)
{
	for (auto& block : locked_micro_blocks_)
		if (check_blocks.count({ block.x, block.y }) > 0)
			return true;
	
	return false;
}

void world_object::set_position(const Vector2f new_position)
{
	position_ = Vector2f(new_position);
	texture_box_.left = ceil(new_position.x - texture_box_offset_.x);
	texture_box_.top = ceil(new_position.y - texture_box_offset_.y);
}

void world_object::take_damage(const float damage, Vector2f attacker_pos)
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

