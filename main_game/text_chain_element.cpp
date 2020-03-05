#include "text_chain_element.h"

const float text_chain_element::default_character_size = helper::GetScreenSize().y * 0.024f;

text_chain_element::text_chain_element() : drawable_chain_element()
{
}

text_chain_element::text_chain_element(
	Vector2f position, 	
	Vector2f offset,
	sf::Color color,
	std::string string,	
	float character_size,
	font_name font,
	float rotation) : drawable_chain_element(position, { 0, 0 }, offset, color)
{
	this->string = std::move(string);
	this->font = font;
	this->character_size = character_size;
	this->rotation = rotation;
}

text_chain_element::~text_chain_element()
= default;
