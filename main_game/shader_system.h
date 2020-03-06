#pragma once
#ifndef SHADERSYSTEM_H
#define SHADERSYSTEM_H

#include "sprite_chain_element.h"

enum class shader { ambient_light = 1, wave_wiggle = 2 };

struct drawable_chain_element_with_shader
{
	drawable_chain_element_with_shader(drawable_chain_element* element) : drawable_chain_element(element) {}
	drawable_chain_element* drawable_chain_element;
	std::map<shader, bool> shaders;
};

class shader_system
{
public:
	shader_system();
	~shader_system();
	static std::vector<drawable_chain_element_with_shader*> put_on_shaders(std::vector<drawable_chain_element*> elements);
};

#endif

