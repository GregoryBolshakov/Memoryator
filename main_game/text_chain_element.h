#pragma once

#include "drawable_chain_element.h"

#include <SFML/Graphics.hpp>
#include <string>

enum class font_name { bebas_font = 1, normal_font = 2, console_font = 3 };

class text_chain_element : public drawable_chain_element
{
public:
	static const float default_character_size;
	text_chain_element();
	text_chain_element(
		sf::Vector2f position,
		sf::Vector2f offset,
		sf::Color color,
		std::string string,
		float character_size = default_character_size,
		font_name font = font_name::normal_font,
		float rotation = 0);
	text_chain_element(text_chain_element&& element) noexcept : drawable_chain_element(std::move(element))
	{
		this->string = element.string;
		this->font = element.font;
		this->character_size = element.character_size;
	}
	virtual ~text_chain_element() = default;
	drawable_chain_element* up_cast() { drawable_chain_element* result = this; return result; }

	std::string string = "";
	font_name font = font_name::console_font;
	float character_size = default_character_size;
};
