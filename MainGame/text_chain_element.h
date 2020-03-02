#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "drawable_chain_element.h"
#include "helper.h"

enum class font_name { BebasFont = 1, NormalFont = 2, ConsoleFont = 3 };

using Color = sf::Color;

class text_chain_element : public drawable_chain_element
{
public:
	static const float defaultCharacterSize;
	text_chain_element();
	text_chain_element(Vector2f position, Vector2f offset, sf::Color color, std::string string, float characterSize = defaultCharacterSize, font_name font = font_name::NormalFont, float rotation = 0);
	virtual ~text_chain_element();
	drawable_chain_element* upCast() { drawable_chain_element* result = this; return result; }

	std::string string = "";
	font_name font = font_name::ConsoleFont;
	float characterSize = defaultCharacterSize;
};
