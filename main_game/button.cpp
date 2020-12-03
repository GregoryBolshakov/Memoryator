#include "button.h"
#include "direction_system.h"

button::button(pack_tag pack,
	pack_part pack_part,
	int default_sprite_number,
	int selected_sprite_number,
	int pressed_sprite_number,
	sf::Vector2f position,
	sf::Vector2f size,
	bool is_selectable,
	button_tag tag,
	sf::Vector2f offset) :
	is_selectable(is_selectable)
	, button_enabled_(pack, pack_part, direction::DOWN, default_sprite_number, position, size, offset, sf::Color(255, 255, 255, 255), false, false)
	, button_selected_(pack, pack_part, direction::DOWN, default_sprite_number, position, size, offset, sf::Color(255, 255, 255, 255), false, false)
	, button_pressed_(pack, pack_part, direction::DOWN, default_sprite_number, position, size, offset, sf::Color(255, 255, 255, 255), false, false)
	, tag_(tag)
{
}

bool button::is_selected(sf::Vector2f mouse_pos)
{
	//if (!isActive)
	//return false;

	//isActive = false;

	const auto rect = sf::FloatRect(button_enabled_.position.x, button_enabled_.position.y, button_enabled_.size.x, button_enabled_.size.y);
	if (rect.contains(sf::Vector2f(mouse_pos)))
	{
		selected_ = true;
		return true;
	}
	selected_ = false;
	return false;
}

void button::set_position(const sf::Vector2f position)
{
	button_enabled_.position = {position.x - button_enabled_.offset.x, position.y - button_enabled_.offset.y};
	button_selected_.position = {position.x - button_selected_.offset.x, position.y - button_selected_.offset.y};
	button_pressed_.position = {position.x - button_pressed_.offset.x, position.y - button_pressed_.offset.y};
}

void button::set_size(const sf::Vector2f size)
{
	button_enabled_.size = size;
	button_selected_.size = size;
	button_pressed_.size = size;
}

void button::become_gray()
{
	is_gray_ = true;
}

void button::stop_being_gray()
{
	is_gray_ = false;
}

unique_ptr<sprite_chain_element> button::prepare_sprite()
{
	unique_ptr<sprite_chain_element> result;
	is_active = true;
	if (is_selected(sf::Vector2f(sf::Mouse::getPosition())) && is_selectable)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
			result = make_unique<sprite_chain_element>(button_pressed_);
		else
			result = make_unique<sprite_chain_element>(button_selected_);
	}
	else
		result = make_unique<sprite_chain_element>(button_enabled_);

	if (is_gray_)
	{
		result->color = sf::Color(100, 100, 100, 150);
	}

	return result;
}
