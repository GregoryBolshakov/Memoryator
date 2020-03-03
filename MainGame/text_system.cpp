#include "text_system.h"

#include "hero_bag.h"

float text_system::character_size = 30.0f * helper::GetScreenSize().y / 1440.0f;
std::unordered_map<font_name, Font> text_system::fonts = {};
std::unordered_map<font_name, Text> text_system::text_boxes = {};
Text text_system::number_of_items;

text_system::text_system()
{
	init_fonts();
	init_text_boxes();
	number_of_items.setFont(fonts[font_name::bebas_font]);
	number_of_items.setCharacterSize(30);
	number_of_items.setFillColor(sf::Color::White);
}

text_system::~text_system()
= default;

void text_system::init_fonts()
{
	Font current_font;
	current_font.loadFromFile("fonts/Bebas.ttf");
	fonts.insert({font_name::bebas_font, current_font});
	current_font.loadFromFile("fonts/normal.ttf");
	fonts.insert({ font_name::normal_font, current_font });
	current_font.loadFromFile("fonts/Console.ttf");
	fonts.insert({ font_name::console_font, current_font });
}

void text_system::init_text_boxes()
{
	Text current_text;
	current_text.setFont(fonts[font_name::bebas_font]);
	text_boxes.insert({ font_name::bebas_font, current_text });
	current_text.setFont(fonts[font_name::normal_font]);
	text_boxes.insert({ font_name::normal_font, current_text });
	current_text.setFont(fonts[font_name::console_font]);
	text_boxes.insert({ font_name::console_font, current_text });
}

void text_system::draw_string(const std::string& str, const font_name font, const float size, const float pos_x, const float pos_y, RenderTarget& target, const sf::Color color)
{
	text_boxes.at(font).setPosition(pos_x, pos_y);
	text_boxes.at(font).setCharacterSize(unsigned(ceil(size)));
	text_boxes.at(font).setFillColor(color);
	text_boxes.at(font).setString(str);
	target.draw(text_boxes.at(font));
}

void text_system::draw_text_box(std::string str, const font_name font, const float size, const float pos_x, const float pos_y, const float width, const float height, RenderTarget& target, const sf::Color color)
{
	auto cur_text = text_boxes.at(font);
	cur_text.setString(str);
	const unsigned long long line_length = long(ceil(str.size() * (width / cur_text.getGlobalBounds().width)));
	auto cur_pos_y = pos_y;

	if (cur_text.getGlobalBounds().width <= width)
	{
		draw_string(str, font, size, pos_x, cur_pos_y, target, color);
		return;
	}

	while (!str.empty())
	{
		if (cur_pos_y > pos_y + height - cur_text.getGlobalBounds().height / 2)
			return;

		auto space_pos = std::min(line_length, str.size() - 1);
		if (str.length() > line_length)
		{
			while (!(str[space_pos] == ' ' || str[space_pos] == '_') && space_pos > 0)
				space_pos--;
		}
		else
			space_pos = 0;

		if (space_pos != 0)
			str.erase(str.begin() + space_pos);
		else
			space_pos = line_length;

		draw_string(str.substr(0, space_pos), font, size, pos_x, cur_pos_y, target, color);
		cur_pos_y += cur_text.getGlobalBounds().height;
		str.erase(0, space_pos);
	}
}

void text_system::draw_number_of_items(const Vector2f pos, const int items_count, RenderTarget& target)
{
	number_of_items.setString(std::to_string(items_count));
	number_of_items.setOrigin(number_of_items.getGlobalBounds().width, number_of_items.getGlobalBounds().height);
	number_of_items.setPosition(pos.x + sprite_pack::icon_size.x, pos.y + sprite_pack::icon_size.x);
	target.draw(number_of_items);
}

sf::Vector2f text_system::get_text_box_size(const std::string& string, const float character_size, const font_name font)
{
	if (fonts.count(font) <= 0 || text_boxes.count(font) <= 0)
		return { 0, 0 };

	text_boxes.at(font).setFont(fonts.at(font));
	text_boxes.at(font).setString(string);
	text_boxes.at(font).setCharacterSize(unsigned(ceil(character_size)));

	return { text_boxes.at(font).getGlobalBounds().width, text_boxes.at(font).getGlobalBounds().height };
}