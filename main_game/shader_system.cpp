#include "shader_system.h"

#include "visual_effects/dynamic_light.hpp"
#include "visual_effects/wave_wiggle.hpp"

shader_system::shader_system(camera_system& camera_system, time_system& time_system)
	: camera_system_{ camera_system }, time_system_{ time_system }
{
	shaders_[shader_kind::dynamic_light] = new dynamic_light(camera_system_, time_system_);
	shaders_[shader_kind::wave_wiggle] = new wave_wiggle(time_system_, camera_system::get_screen_size());
}

shader_system::~shader_system()
{
	for (auto& [key, value] : shaders_)
	{
		delete value;
	}
}

visual_effect* shader_system::get_shader(const shader_kind kind)
{
	return shaders_[kind];
}

std::vector<visual_effect*> shader_system::get_shaders(drawable_chain_element* element)
{
	std::vector<visual_effect*> result;

	const auto sprite_element = dynamic_cast<sprite_chain_element*>(element);
	if (sprite_element)
	{
		if ((sprite_element->pack_tag == pack_tag::darkWoods ||
			sprite_element->pack_tag == pack_tag::birchGrove ||
			sprite_element->pack_tag == pack_tag::swampyTrees) &&
			(sprite_element->pack_part == pack_part::tree ||
				sprite_element->pack_part == pack_part::bush ||
				sprite_element->pack_part == pack_part::plant))
		{
			result.push_back(get_shader(shader_kind::wave_wiggle));
		}
	}

	return result;
}


std::vector<drawable_chain_element_with_shaders*> shader_system::assign_shaders(const std::vector<drawable_chain_element*>& elements)
{
	std::vector<drawable_chain_element_with_shaders*> result = {};

	for (auto& element : elements)
	{
		result.push_back(new drawable_chain_element_with_shaders(element));

		// adding wave wiggle shader
		const auto sprite_element = dynamic_cast<sprite_chain_element*>(element);
		if (sprite_element)
		{
			if ((sprite_element->pack_tag == pack_tag::darkWoods ||
				sprite_element->pack_tag == pack_tag::birchGrove ||
				sprite_element->pack_tag == pack_tag::swampyTrees) &&
				(sprite_element->pack_part == pack_part::tree ||
					sprite_element->pack_part == pack_part::bush ||
					sprite_element->pack_part == pack_part::plant))
				result[result.size() - 1]->shaders[shader_kind::wave_wiggle] = true;
		}
	}

	return result;
}
