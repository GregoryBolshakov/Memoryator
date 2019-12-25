#pragma once
#ifndef TEXTWRITER_H
#define TEXTWRITER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Helper.h"

using namespace sf;

enum FontName { BebasFont = 1, NormalFont = 2, ConsoleFont = 3 };

class TextWriter
{
public:
	TextWriter();
	~TextWriter();	
	static void drawString(std::string str, FontName font, int size, float posX, float posY, RenderWindow* window, Color color = Color::Black);
	static void drawTextBox(std::string str, FontName font, int size, float posX, float posY, float width, float height, RenderWindow* window, Color color = Color::Black);
	static void drawNumberOfItems(Vector2f pos, int itemsCount, RenderWindow &window);
	void initFonts();
	void initTextBoxes();

	static std::unordered_map<FontName, Font> fonts;
	static std::unordered_map<FontName, Text> textBoxes;
	static int characterSize;
	static Text numberOfItems;
};
#endif

