#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "text_system.h"

using namespace sf;

class input_box
{
public:
	input_box();
	input_box(FloatRect rect);
	~input_box();
	void init(FloatRect rect);
	void draw(RenderWindow& window) const;
	void on_mouse_release();
	void handle_events(Event event);
	void interact(long long elapsed_time);
	void reset_cursor(bool to_end = false);
	std::string line;

private:
	void set_cursor_to_pos();
	void animate_cursor();
	FloatRect rect_;
	size_t cursor_pos_ = 0;
	sf::Color out_fill_ = sf::Color(85, 47, 8, 220);
	sf::Color in_fill_ = sf::Color(210, 163, 116, 60);
	sf::Color in_fill_selected_ = sf::Color(235, 200, 166, 100);
	sf::Color text_color_ = sf::Color(235, 200, 166, 180);
	RectangleShape body_;
	RectangleShape inner_part_;
	RectangleShape cursor_;
	bool mouse_pressed_ = false;
	std::pair<float, float> mouse_select_vector_;
	float character_size_ = 30.0F;
	float micro_offset_ = 6.0F;
	Text text_;

	long long time_for_cursor_animation_ = long(5e5);
	long long time_after_cursor_animation_ = 0;
};
