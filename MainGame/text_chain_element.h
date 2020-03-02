#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "drawable_chain_element.h"
#include "helper.h"

enum class font_name { bebas_font = 1, normal_font = 2, console_font = 3 };

using Color = sf::Color;

class text_chain_element : public drawable_chain_element
{
public:
	static const float default_character_size;
	text_chain_element();
	text_chain_element(
		Vector2f position,
		Vector2f offset,
		sf::Color color,
		std::string string,
		float character_size = default_character_size,
		font_name font = font_name::normal_font,
		float rotation = 0);
	virtual ~text_chain_element();
	drawable_chain_element* up_cast() { drawable_chain_element* result = this; return result; }

	std::string string = "";
	font_name font = font_name::console_font;
	float character_size = default_character_size;
};
