#include "text_system.h"

#include "hero_bag.h"

float text_system::characterSize = 30.0f * helper::GetScreenSize().y / 1440.0f;
std::unordered_map<FontName, Font> text_system::fonts = {};
std::unordered_map<FontName, Text> text_system::textBoxes = {};
Text text_system::numberOfItems;

text_system::text_system()
{
	initFonts();
	initTextBoxes();
	numberOfItems.setFont(fonts[FontName::BebasFont]);
	numberOfItems.setCharacterSize(30);
	numberOfItems.setFillColor(sf::Color::White);
}

text_system::~text_system()
= default;

void text_system::initFonts()
{
	Font currentFont;
	currentFont.loadFromFile("fonts/Bebas.ttf");
	fonts.insert({FontName::BebasFont, currentFont});
	currentFont.loadFromFile("fonts/normal.ttf");
	fonts.insert({ FontName::NormalFont, currentFont });
	currentFont.loadFromFile("fonts/Console.ttf");
	fonts.insert({ FontName::ConsoleFont, currentFont });
}

void text_system::initTextBoxes()
{
	Text currentText;
	currentText.setFont(fonts[FontName::BebasFont]);
	textBoxes.insert({ FontName::BebasFont, currentText });
	currentText.setFont(fonts[FontName::NormalFont]);
	textBoxes.insert({ FontName::NormalFont, currentText });
	currentText.setFont(fonts[FontName::ConsoleFont]);
	textBoxes.insert({ FontName::ConsoleFont, currentText });
}

void text_system::drawString(const std::string& str, const FontName font, const float size, const float posX, const float posY, RenderTarget& target, const sf::Color color)
{
	textBoxes.at(font).setPosition(posX, posY);
	textBoxes.at(font).setCharacterSize(unsigned(ceil(size)));
	textBoxes.at(font).setFillColor(color);
	textBoxes.at(font).setString(str);
	target.draw(textBoxes.at(font));
}

void text_system::drawTextBox(std::string str, const FontName font, const float size, const float posX, const float posY, const float width, const float height, RenderTarget& target, const sf::Color color)
{
	auto curText = textBoxes.at(font);
	curText.setString(str);
	const unsigned long long lineLength = long(ceil(str.size() * (width / curText.getGlobalBounds().width)));
	auto curPosY = posY;

	if (curText.getGlobalBounds().width <= width)
	{
		drawString(str, font, size, posX, curPosY, target, color);
		return;
	}

	while (!str.empty())
	{
		if (curPosY > posY + height - curText.getGlobalBounds().height / 2)
			return;

		auto spacePos = std::min(lineLength, str.size() - 1);
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

void text_system::drawNumberOfItems(const Vector2f pos, const int itemsCount, RenderTarget& target)
{
	numberOfItems.setString(std::to_string(itemsCount));
	numberOfItems.setOrigin(numberOfItems.getGlobalBounds().width, numberOfItems.getGlobalBounds().height);
	numberOfItems.setPosition(pos.x + sprite_pack::iconSize.x, pos.y + sprite_pack::iconSize.x);
	target.draw(numberOfItems);
}

sf::Vector2f text_system::getTextBoxSize(const std::string& string, const float characterSize, const FontName font)
{
	if (fonts.count(font) <= 0 || textBoxes.count(font) <= 0)
		return { 0, 0 };

	textBoxes.at(font).setFont(fonts.at(font));
	textBoxes.at(font).setString(string);
	textBoxes.at(font).setCharacterSize(unsigned(ceil(characterSize)));

	return { textBoxes.at(font).getGlobalBounds().width, textBoxes.at(font).getGlobalBounds().height };
}