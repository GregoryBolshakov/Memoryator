#pragma once

#include "sprite_chain_element.h"
#include "visual_effects/visual_effect.hpp"

class camera_system;
class scale_system;
class time_system;
class world_handler;

enum class shader_kind { dynamic_light = 1, wave_wiggle = 0 };

struct drawable_chain_element_with_shaders
{
	explicit drawable_chain_element_with_shaders(drawable_chain_element* element) : drawable_chain_element(element) {}
	drawable_chain_element* drawable_chain_element;
	std::map<shader_kind, bool> shaders;
};

class shader_system
{
public:
	shader_system(camera_system& camera_system, time_system& time_system);
	~shader_system();

	visual_effect* get_shader(shader_kind kind);
	std::vector<visual_effect*> get_shaders(drawable_chain_element*);

	static std::vector<drawable_chain_element_with_shaders*> assign_shaders(const std::vector<drawable_chain_element*>& elements);
private:
	std::map<shader_kind, visual_effect*> shaders_;
	camera_system& camera_system_;
	time_system& time_system_;
};
