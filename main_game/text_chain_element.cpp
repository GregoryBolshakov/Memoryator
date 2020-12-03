#include "text_chain_element.h"
#include "world_metrics.h"

const float text_chain_element::default_character_size = world_metrics::window_size.y * 0.024f;

text_chain_element::text_chain_element() : drawable_chain_element()
{
}

text_chain_element::text_chain_element(
	sf::Vector2f position, 	
	sf::Vector2f offset,
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
