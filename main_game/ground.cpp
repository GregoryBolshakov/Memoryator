#include "ground.h"
#include "direction_system.h"
#include "sprite_chain_element.h"
#include "world_metrics.h"
#include "helper.h"

ground::ground(std::string objectName, const sf::Vector2f centerPosition, const int typeOfObject) : static_object(std::move(objectName), centerPosition)
{
	variety_of_types_ = 3;
	this->type_of_object_ = typeOfObject;
	is_background = true;
	ground::setType(typeOfObject);
	tag = entity_tag::ground;
}

void ground::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->type_of_object_ = typeOfObject;
	this->conditional_size_units_ = world_metrics::block_size;
	this->z_coordinate_ = typeOfObject * 10 + 1;
}

sf::Vector2f ground::calculate_texture_offset()
{
	return {0, 0};
}

sf::Vector2f ground::get_build_position(std::vector<world_object*>, float, sf::Vector2f)
{
	return { -1, -1 };
}

int ground::get_build_type(sf::Vector2f, sf::Vector2f)
{
	return 1;
}

std::vector<unique_ptr<sprite_chain_element>> ground::prepare_sprites(long long)
{
	std::vector<unique_ptr<sprite_chain_element>> result;
	auto body = make_unique<sprite_chain_element>(sprite_chain_element(pack_tag::darkWoods, pack_part::ground, direction::DOWN, 1, position_, conditional_size_units_, sf::Vector2f(texture_box_offset_)));
	
	body->z_coordinate = z_coordinate_;
	body->is_background = true;

	if (type_of_object_ == 1)
		body->pack_tag = pack_tag::swampyTrees;
	if (type_of_object_ == 2)
		body->pack_tag = pack_tag::darkWoods;
	if (type_of_object_ == 3)
		body->pack_tag = pack_tag::birchGrove;

	result.push_back(std::move(body));
	return result;
}