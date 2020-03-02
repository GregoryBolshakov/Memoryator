#include "TextChainElement.h"

const float TextChainElement::defaultCharacterSize = Helper::GetScreenSize().y * 0.024f;

TextChainElement::TextChainElement() : drawable_chain_element()
{
}

TextChainElement::TextChainElement(
	Vector2f position, 	
	Vector2f offset,
	sf::Color color,
	std::string string,	
	float characterSize,
	FontName font,
	float rotation) : drawable_chain_element(position, { 0, 0 }, offset, color)
{
	this->string = std::move(string);
	this->font = font;
	this->characterSize = characterSize;
	this->rotation = rotation;
}

TextChainElement::~TextChainElement()
= default;
