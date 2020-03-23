#include "shader_system.h"

#include "visual_effects/dynamic_light.hpp"
#include "visual_effects/wave_wiggle.hpp"

shader_system::shader_system(camera_system& camera_system, time_system& time_system)
	: camera_system_{ camera_system }, time_system_{ time_system }
{
	shaders_[shader_kind::dynamic_light] = std::make_unique<dynamic_light>(camera_system_, time_system_);
	shaders_[shader_kind::wave_wiggle] = std::make_unique<wave_wiggle>();
}

void shader_system::initialize()
{
	for (auto& [key, value] : shaders_)
	{
		value->load();
	}
}

void shader_system::update()
{
	for (auto& [key, value] : shaders_)
	{
		value->update();
	}
}

Shader* shader_system::get_shader(const shader_kind kind)
{
	return shaders_[kind]->shader;
}

Shader* shader_system::get_shader(RenderTarget& target, sprite_chain_element* element, Sprite& sprite)
{
	if ((element->pack_tag == pack_tag::darkWoods ||
			element->pack_tag == pack_tag::birchGrove ||
			element->pack_tag == pack_tag::swampyTrees) &&
		(element->pack_part == pack_part::tree ||
			element->pack_part == pack_part::bush ||
			element->pack_part == pack_part::plant))
	{
		const auto effect = shaders_[shader_kind::wave_wiggle].get();
		effect->update(target, sprite);
		
		return effect->shader;
	}

	return nullptr;
}
