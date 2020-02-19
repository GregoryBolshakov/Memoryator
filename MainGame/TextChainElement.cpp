#include "TextChainElement.h"

const int TextChainElement::defaultCharacterSize = Helper::GetScreenSize().y * 0.024;

TextChainElement::TextChainElement() : DrawableChainElement()
{
}

TextChainElement::TextChainElement(
	Vector2f position, 	
	Vector2f offset,
	sf::Color color,
	std::string string,	
	int characterSize,
	FontName font,
	float rotation) : DrawableChainElement(position, { 0, 0 }, offset, color)
{
	this->string = string;
	this->font = font;
	this->characterSize = characterSize;
	this->rotation = rotation;
}

TextChainElement::~TextChainElement()
= default;
