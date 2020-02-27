#pragma once
#include "TextSystem.h"
#include "HeroBag.h"

int TextSystem::characterSize = 30 * Helper::GetScreenSize().y / 1440;
std::unordered_map<FontName, Font> TextSystem::fonts = {};
std::unordered_map<FontName, Text> TextSystem::textBoxes = {};
Text TextSystem::numberOfItems;

TextSystem::TextSystem()
{
	initFonts();
	initTextBoxes();
	numberOfItems.setFont(fonts[FontName::BebasFont]);
	numberOfItems.setCharacterSize(30);
	numberOfItems.setFillColor(sf::Color::White);
}

TextSystem::~TextSystem()
= default;

void TextSystem::initFonts()
{
	Font currentFont;
	currentFont.loadFromFile("fonts/Bebas.ttf");
	fonts.insert({FontName::BebasFont, currentFont});
	currentFont.loadFromFile("fonts/normal.ttf");
	fonts.insert({ FontName::NormalFont, currentFont });
	currentFont.loadFromFile("fonts/Console.ttf");
	fonts.insert({ FontName::ConsoleFont, currentFont });
}

void TextSystem::initTextBoxes()
{
	Text currentText;
	currentText.setFont(fonts[FontName::BebasFont]);
	textBoxes.insert({ FontName::BebasFont, currentText });
	currentText.setFont(fonts[FontName::NormalFont]);
	textBoxes.insert({ FontName::NormalFont, currentText });
	currentText.setFont(fonts[FontName::ConsoleFont]);
	textBoxes.insert({ FontName::ConsoleFont, currentText });
}

void TextSystem::drawString(const std::string& str, FontName font, int size, float posX, float posY, RenderTarget& target, sf::Color color)
{
	textBoxes.at(font).setPosition(posX, posY);
	textBoxes.at(font).setCharacterSize(size);
	textBoxes.at(font).setFillColor(color);
	textBoxes.at(font).setString(str);
	target.draw(textBoxes.at(font));
}

void TextSystem::drawTextBox(std::string str, FontName font, int size, float posX, float posY, float width, float height, RenderTarget& target, sf::Color color)
{
	auto curText = textBoxes.at(font);
	curText.setString(str);
	const int lineLength = str.size() * (width / curText.getGlobalBounds().width);
	float curPosY = posY;

	if (curText.getGlobalBounds().width <= width)
	{
		drawString(str, font, size, posX, curPosY, target, color);
		return;
	}

	while (!str.empty())
	{
		if (curPosY > posY + height - curText.getGlobalBounds().height / 2)
			return;
		
		int spacePos = std::min(lineLength, int(str.size() - 1));
		if (str.length() > lineLength)
		{
			while (!(str[spacePos] == ' ' || str[spacePos] == '_') && spacePos > 0)
				spacePos--;
		}
		else
			spacePos = 0;

		if (spacePos != 0)
			str.erase(str.begin() + spacePos);
		else
			spacePos = lineLength;

		drawString(str.substr(0, spacePos), font, size, posX, curPosY, target, color);
		curPosY += curText.getGlobalBounds().height;
		str.erase(0, spacePos);
	}
}

void TextSystem::drawNumberOfItems(Vector2f pos, int itemsCount, RenderTarget& target)
{
	numberOfItems.setString(std::to_string(itemsCount));
	numberOfItems.setOrigin(numberOfItems.getGlobalBounds().width, numberOfItems.getGlobalBounds().height);
	numberOfItems.setPosition(pos.x + SpritePack::iconSize.x, pos.y + SpritePack::iconSize.x);
	target.draw(numberOfItems);
}

sf::Vector2f TextSystem::getTextBoxSize(const std::string& string, int characterSize, FontName font)
{
	if (fonts.count(font) <= 0 || textBoxes.count(font) <= 0)
		return { 0, 0 };

	textBoxes.at(font).setFont(fonts.at(font));
	textBoxes.at(font).setString(string);
	textBoxes.at(font).setCharacterSize(characterSize);

	return { textBoxes.at(font).getGlobalBounds().width, textBoxes.at(font).getGlobalBounds().height };
}