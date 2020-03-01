#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "DrawableChainElement.h"
#include "Helper.h"

enum class FontName { BebasFont = 1, NormalFont = 2, ConsoleFont = 3 };

using Color = sf::Color;

class TextChainElement : public DrawableChainElement
{
public:
	static const float defaultCharacterSize;
	TextChainElement();
	TextChainElement(Vector2f position, Vector2f offset, sf::Color color, std::string string, float characterSize = defaultCharacterSize, FontName font = FontName::NormalFont, float rotation = 0);
	virtual ~TextChainElement();
	DrawableChainElement* upCast() { DrawableChainElement* result = this; return result; }

	std::string string = "";
	FontName font = FontName::ConsoleFont;
	float characterSize = defaultCharacterSize;
};
