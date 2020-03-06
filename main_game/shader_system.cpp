#include "shader_system.h"

shader_system::shader_system()
= default;

shader_system::~shader_system()
= default;

std::vector<drawable_chain_element_with_shader*> shader_system::put_on_shaders(std::vector<drawable_chain_element*> elements)
{
	std::vector<drawable_chain_element_with_shader*> result = {};

	for (auto& element : elements)
	{
		result.push_back(new drawable_chain_element_with_shader(element));
		result[result.size() - 1]->shaders[shader::ambient_light] = true;

		// adding wave wiggle shader
		const auto sprite_element = dynamic_cast<sprite_chain_element*>(element);
		if (sprite_element)
		{
			if ((sprite_element->pack_tag == pack_tag::darkWoods || sprite_element->pack_tag == pack_tag::birchGrove ||
				sprite_element->pack_tag == pack_tag::swampyTrees) && (sprite_element->pack_part == pack_part::tree ||
					sprite_element->pack_part == pack_part::bush || sprite_element->pack_part == pack_part::plant))
				result[result.size() - 1]->shaders[shader::wave_wiggle] = true;
		}
		//--------------------------
	}

	return result;
}