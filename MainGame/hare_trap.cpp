#include "hare_trap.h"

#include "helper.h"

hare_trap::hare_trap(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : picked_object(std::move(objectName), centerPosition)
{
	variety_of_types_ = 1;
	this->type_of_object_ = typeOfObject;
	radius_ = 100;
	to_save_name_ = "hareTrap";
	hare_trap::setType(typeOfObject);
	current_sprite_[0] = 1;
	id = entity_tag::hareTrap;
	tag = entity_tag::hareTrap;
	inventory = hero_bag::empty_inventory;
}

void hare_trap::setType(int typeOfObject)
{
	this->type_of_object_ = typeOfObject;
	this->conditional_size_units_ = { 140, 111 };
}

Vector2f hare_trap::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return { texture_box_.width / 2.0f, texture_box_.height / 1.2f };
}

void hare_trap::init_pedestal()
{
	focus1 = position_;
	focus2 = position_;
	ellipseSizeMultipliers[0] = { 0 };
}

Vector2f hare_trap::get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

Vector2f hare_trap::getEnterPosition() const
{
	return {position_.x - conditional_size_units_.x / 2.0f, position_.y + conditional_size_units_.y / 10.0f };
}

int hare_trap::get_build_type(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<sprite_chain_element*> hare_trap::prepare_sprites(long long elapsedTime)
{
	const auto body = new sprite_chain_element(pack_tag::craftObjects, pack_part::hareTrap, direction::DOWN, 1, position_, conditional_size_units_, Vector2f(texture_box_offset_));
	body->is_background = true;

	return { body };
}