#pragma once

#include <memory>
#include <unordered_map>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class RenderWindow;
}
enum class font_name;

class text_system
{
public:
	text_system();
	~text_system();	
	static void draw_string(const std::string& str, font_name font, const float size, float pos_x, float pos_y, const std::shared_ptr<sf::RenderWindow>& target, sf::Color color = sf::Color::Black);
	static void draw_text_box(std::string str, font_name font, const float size, float pos_x, float pos_y, float width, float height, const std::shared_ptr<sf::RenderWindow>& target, sf::Color color = sf::Color::Black);
	static void draw_number_of_items(sf::Vector2f pos, int items_count, const std::shared_ptr<sf::RenderWindow>& target);
	static void init_fonts();
	static void init_text_boxes();
	static sf::Vector2f get_text_box_size(const std::string& string, float character_size, font_name font);

	static std::unordered_map<font_name, sf::Font> fonts;
	static std::unordered_map<font_name, sf::Text> text_boxes;
	static float character_size;
	static sf::Text number_of_items;
};
