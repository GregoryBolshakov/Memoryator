#pragma once
#ifndef TEXTCHAINELEMENT_H
#define TEXTCHAINELEMENT_H
#include <string>
#include <SFML/Graphics.hpp>
#include "DrawableChainElement.h"
#include "Helper.h"

enum class FontName { BebasFont = 1, NormalFont = 2, ConsoleFont = 3 };

using Color = sf::Color;

class TextChainElement : public DrawableChainElement
{
public:
	static const int defaultCharacterSize;
	TextChainElement();
	TextChainElement(Vector2f position, Vector2f offset, sf::Color color, std::string string, int characterSize = defaultCharacterSize, FontName font = FontName::NormalFont, float rotation = 0);
	virtual ~TextChainElement();
	DrawableChainElement* upCast() { DrawableChainElement* result = this; return result; }

	std::string string = "";
	FontName font = FontName::ConsoleFont;
	int characterSize = defaultCharacterSize;
};

#endif

