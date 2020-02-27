#pragma once
#ifndef TEXTSYSTEM_H
#define TEXTSYSTEM_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "TextChainElement.h"

using namespace sf;

class TextSystem
{
public:
	TextSystem();
	~TextSystem();	
	static void drawString(const std::string& str, FontName font, int size, float posX, float posY, RenderTarget& target, sf::Color color = sf::Color::Black);
	static void drawTextBox(std::string str, FontName font, int size, float posX, float posY, float width, float height, RenderTarget& target, sf::Color color = sf::Color::Black);
	static void drawNumberOfItems(Vector2f pos, int itemsCount, RenderTarget& target);
	static void initFonts();
	static void initTextBoxes();
	static Vector2f getTextBoxSize(const std::string& string, int characterSize, FontName font);

	static std::unordered_map<FontName, Font> fonts;
	static std::unordered_map<FontName, Text> textBoxes;
	static int characterSize;
	static Text numberOfItems;
};
#endif

