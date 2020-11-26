#include "input_box.h"
#include "helper.h"
#include "text_system.h"
#include "text_chain_element.h"

input_box::input_box() = default;

input_box::input_box(const sf::FloatRect rect)
{
	init(rect);
}

input_box::~input_box() = default;

void input_box::init(const sf::FloatRect rect)
{
	this->rect_ = sf::FloatRect(rect);
	body_.setSize(sf::Vector2f(rect.width, rect.height));
	body_.setPosition(rect.left, rect.top);
	body_.setFillColor(out_fill_);
	inner_part_.setSize(sf::Vector2f(rect.width - micro_offset_, rect.height - micro_offset_));
	inner_part_.setPosition(rect.left + micro_offset_ / 2.0f, rect.top + micro_offset_ / 2.0f);
	inner_part_.setFillColor(in_fill_);
	cursor_.setSize(sf::Vector2f(micro_offset_ / 1.5f, inner_part_.getSize().y - 1.5f * micro_offset_));
	cursor_.setPosition(inner_part_.getPosition().x + micro_offset_, inner_part_.getPosition().y + micro_offset_ * 3.0f / 4);
	cursor_.setFillColor(text_color_);

	text_.setFont(text_system::fonts.at(font_name::console_font));
	text_.setCharacterSize(unsigned int(ceil(text_system::character_size)));
	text_.setFillColor(text_color_);
	text_.setPosition(inner_part_.getPosition().x + micro_offset_ * 2, inner_part_.getPosition().y - micro_offset_ / 2.0f);
}

void input_box::reset_cursor(const bool to_end)
{
	if (to_end)
	{
		cursor_pos_ = line.size();
		set_cursor_to_pos();
	}
	else cursor_pos_ = 0;
}

void input_box::set_cursor_to_pos()
{
	text_.setString(line.substr(0, cursor_pos_));
	cursor_.setPosition(text_.getGlobalBounds().left + text_.getGlobalBounds().width + micro_offset_ / 2.0f, cursor_.getPosition().y);
}

void input_box::draw(const std::shared_ptr<sf::RenderWindow>& target) const
{
	target->draw(body_);
	target->draw(inner_part_);
	text_system::draw_string(line, font_name::console_font, character_size_, inner_part_.getPosition().x + micro_offset_ * 2.0f, inner_part_.getPosition().y - micro_offset_ / 2.0f, target, text_color_);
	target->draw(cursor_);
}

void input_box::animate_cursor()
{
	if (cursor_.getFillColor() == text_color_)
		cursor_.setFillColor(sf::Color(text_color_.r, text_color_.g, text_color_.b, 10));
	else
		cursor_.setFillColor(text_color_);
}

void input_box::on_mouse_release()
{
	mouse_pressed_ = false;
	float minDist = helper::GetScreenSize().x + helper::GetScreenSize().y;
	int newPos = 0;
	for (auto i = 0u; i < line.size(); i++)
	{
		text_.setString(line.substr(0, i));
		const float dist = abs(text_.getGlobalBounds().left + text_.getGlobalBounds().width - sf::Mouse::getPosition().x);
		if (dist < minDist)
		{
			minDist = dist;
			newPos = i;
		}
	}
	cursor_pos_ = newPos;
}

void input_box::interact(const long long elapsed_time)
{
	set_cursor_to_pos();
	if (helper::isIntersects(sf::Vector2f(sf::Mouse::getPosition()), sf::FloatRect(body_.getPosition().x, body_.getPosition().y, body_.getSize().x, body_.getSize().y)))
		inner_part_.setFillColor(in_fill_selected_);
	else
		inner_part_.setFillColor(in_fill_);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		mouse_pressed_ = true;
		mouse_select_vector_.second = float(sf::Mouse::getPosition().x);
	}

	time_after_cursor_animation_ += elapsed_time;
	if (time_after_cursor_animation_ >= time_for_cursor_animation_)
	{
		time_after_cursor_animation_ = 0;
		animate_cursor();
	}
}

void input_box::handle_events(const sf::Event event)
{
	if (event.type == sf::Event::MouseButtonReleased)
		on_mouse_release();

	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode == 8)
		{
			if (!line.empty() && cursor_pos_ != 0)
			{
				line.erase(line.begin() + cursor_pos_ - 1);
				cursor_pos_--;
			}
		}
		else
			if (text_.getGlobalBounds().left + text_.getGlobalBounds().width <= inner_part_.getGlobalBounds().left + inner_part_.getGlobalBounds().width &&
				!(sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)))
			{
				/*if (event.text.unicode >= 65 && event.text.unicode <= 90)
					event.text.unicode += 32;*/
				if (line.size() >= cursor_pos_)
					line.insert(cursor_pos_, 1, char(event.text.unicode));
				cursor_pos_++;
			}
	}

	if (event.type == sf::Event::KeyPressed)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) && cursor_pos_ < line.size())
			line.erase(line.begin() + cursor_pos_);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && cursor_pos_ > 0)
			cursor_pos_--;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && cursor_pos_ < line.size())
			cursor_pos_++;
	}
}
