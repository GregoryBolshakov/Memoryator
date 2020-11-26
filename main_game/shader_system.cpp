#include "shader_system.h"
#include "sprite_chain_element.h"
#include "visual_effects/dynamic_light.hpp"
#include "visual_effects/wave_wiggle.hpp"

shader_system::shader_system(shared_ptr<camera_system>& camera_system, const shared_ptr<time_system>& time_system)
	: camera_system_{ camera_system }, time_system_{ time_system }
{
	shaders_[shader_kind::dynamic_light] = std::make_unique<dynamic_light>(*camera_system_, *time_system_);
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

const sf::Shader& shader_system::get_shader(const shader_kind kind)
{
	return *shaders_[kind]->shader;
}

const sf::Shader& shader_system::get_shader(sf::RenderTarget& target, sprite_chain_element& element, sf::Sprite& sprite)
{
	if ((element.pack_tag == pack_tag::darkWoods ||
			element.pack_tag == pack_tag::birchGrove ||
			element.pack_tag == pack_tag::swampyTrees) &&
		(element.pack_part == pack_part::tree ||
			element.pack_part == pack_part::bush ||
			element.pack_part == pack_part::plant))
	{
		auto* effect = shaders_[shader_kind::wave_wiggle].get();
		effect->update(target, sprite);
		
		return *effect->shader;
	}

	return empty_shader_;
}
