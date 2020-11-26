#pragma once

#include <SFML/Graphics.hpp>

#include "sprite_chain_element.h"

using std::unique_ptr, std::make_unique;

class button
{
public:
	button();
	
	void initialize(
		pack_tag pack,
		pack_part pack_part,
		int default_sprite_number,
		int selected_sprite_number,
		int pressed_sprite_number,
		sf::Vector2f position,
		sf::Vector2f size,
		bool is_selectable,
		button_tag tag,
		sf::Vector2f offset = { 0, 0 });
	
	unique_ptr<sprite_chain_element> prepare_sprite();
	
	void become_gray();
	
	void stop_being_gray();
	
	bool is_selected(sf::Vector2f mouse_pos);

	[[nodiscard]] sf::Vector2f get_position() const
	{
		return button_enabled_->position;
	}

	[[nodiscard]] sf::Vector2f get_size() const
	{
		return button_enabled_->size;
	}

	[[nodiscard]] sf::Vector2f get_offset() const
	{
		return button_enabled_->offset;
	}

	[[nodiscard]] sf::FloatRect get_global_bounds() const
	{
		return {get_position(), get_size()};
	}

	void set_position(sf::Vector2f position) const;
	
	void set_size(sf::Vector2f size) const;

	[[nodiscard]] button_tag get_tag() const
	{
		return tag_;
	}

	[[nodiscard]] bool get_gray_state() const
	{
		return is_gray_;
	}	
	
	bool is_selectable{};
	bool is_active = true;

private:
	sprite_chain_element* button_enabled_ = new sprite_chain_element();
	sprite_chain_element* button_selected_ = new sprite_chain_element();
	sprite_chain_element* button_pressed_ = new sprite_chain_element();
	button_tag tag_{};
	bool selected_ = false;
	bool is_gray_ = false;
};
