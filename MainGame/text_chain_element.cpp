#include "text_chain_element.h"

const float text_chain_element::defaultCharacterSize = Helper::GetScreenSize().y * 0.024f;

text_chain_element::text_chain_element() : drawable_chain_element()
{
}

text_chain_element::text_chain_element(
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

text_chain_element::~text_chain_element()
= default;
