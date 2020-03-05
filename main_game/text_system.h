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
	static void draw_string(const std::string& str, font_name font, const float size, float pos_x, float pos_y, RenderTarget& target, sf::Color color = sf::Color::Black);
	static void draw_text_box(std::string str, font_name font, const float size, float pos_x, float pos_y, float width, float height, RenderTarget& target, sf::Color color = sf::Color::Black);
	static void draw_number_of_items(Vector2f pos, int items_count, RenderTarget& target);
	static void init_fonts();
	static void init_text_boxes();
	static Vector2f get_text_box_size(const std::string& string, float character_size, font_name font);

	static std::unordered_map<font_name, Font> fonts;
	static std::unordered_map<font_name, Text> text_boxes;
	static float character_size;
	static Text number_of_items;
};
