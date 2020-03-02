#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "text_chain_element.h"

using namespace sf;

class text_system
{
public:
	text_system();
	~text_system();	
	static void drawString(const std::string& str, font_name font, const float size, float posX, float posY, RenderTarget& target, sf::Color color = sf::Color::Black);
	static void drawTextBox(std::string str, font_name font, const float size, float posX, float posY, float width, float height, RenderTarget& target, sf::Color color = sf::Color::Black);
	static void drawNumberOfItems(Vector2f pos, int itemsCount, RenderTarget& target);
	static void initFonts();
	static void initTextBoxes();
	static Vector2f getTextBoxSize(const std::string& string, float characterSize, font_name font);

	static std::unordered_map<font_name, Font> fonts;
	static std::unordered_map<font_name, Text> textBoxes;
	static float characterSize;
	static Text numberOfItems;
};
