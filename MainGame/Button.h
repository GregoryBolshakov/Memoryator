#pragma once

#include <SFML/Graphics.hpp>

#include "SpriteChainElement.h"

using namespace sf;

class button
{
public:
	button();
	
	void initialize(
		PackTag pack,
		PackPart pack_part,
		int default_sprite_number,
		int selected_sprite_number,
		int pressed_sprite_number,
		Vector2f position,
		Vector2f size,
		bool is_selectable,
		ButtonTag tag,
		Vector2f offset = { 0, 0 });
	
	SpriteChainElement* prepare_sprite();
	
	void become_gray();
	
	void stop_being_gray();
	
	bool is_selected(Vector2f mouse_pos);

	[[nodiscard]] Vector2f get_position() const
	{
		return button_enabled_->position;
	}

	[[nodiscard]] Vector2f get_size() const
	{
		return button_enabled_->size;
	}

	[[nodiscard]] Vector2f get_offset() const
	{
		return button_enabled_->offset;
	}

	[[nodiscard]] FloatRect get_global_bounds() const
	{
		return {get_position(), get_size()};
	}

	void set_position(Vector2f position) const;
	
	void set_size(Vector2f size) const;

	[[nodiscard]] ButtonTag get_tag() const
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
	SpriteChainElement* button_enabled_ = new SpriteChainElement();
	SpriteChainElement* button_selected_ = new SpriteChainElement();
	SpriteChainElement* button_pressed_ = new SpriteChainElement();
	ButtonTag tag_{};
	bool selected_ = false;
	bool is_gray_ = false;
};
