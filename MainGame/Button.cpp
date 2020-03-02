#include "button.h"

button::button()
= default;

void button::initialize(
	const PackTag pack,
	const PackPart pack_part,
	const int default_sprite_number,
	const int selected_sprite_number,
	const int pressed_sprite_number,
	const Vector2f position,
	const Vector2f size,
	const bool is_selectable,
	const ButtonTag tag,
	const Vector2f offset)
{
	tag_ = tag;
	button_enabled_->packTag = pack;
	this->button_selected_->packTag = pack;
	this->button_pressed_->packTag = pack;
	button_enabled_->packPart = pack_part;
	this->button_selected_->packPart = pack_part;
	this->button_pressed_->packPart = pack_part;
	button_enabled_->number = default_sprite_number;
	this->button_selected_->number = selected_sprite_number;
	this->button_pressed_->number = pressed_sprite_number;
	button_enabled_->position = position;
	this->button_selected_->position = position;
	this->button_pressed_->position = position;
	button_enabled_->size = size;
	this->button_selected_->size = size;
	this->button_pressed_->size = size;
	button_enabled_->offset = offset;
	this->button_selected_->offset = offset;
	this->button_pressed_->offset = offset;
	button_enabled_->initialize();
	this->button_selected_->initialize();
	this->button_pressed_->initialize();
	this->is_selectable = is_selectable;
}

bool button::is_selected(Vector2f mouse_pos)
{
	//if (!isActive)
	//return false;

	//isActive = false;

	const auto rect = FloatRect(button_enabled_->position.x, button_enabled_->position.y, button_enabled_->size.x, button_enabled_->size.y);
	if (rect.contains(Vector2f(mouse_pos)))
	{
		selected_ = true;
		return true;
	}
	selected_ = false;
	return false;
}

void button::set_position(const Vector2f position) const
{
	button_enabled_->position = {position.x - button_enabled_->offset.x, position.y - button_enabled_->offset.y};
	button_selected_->position = {position.x - button_selected_->offset.x, position.y - button_selected_->offset.y};
	button_pressed_->position = {position.x - button_pressed_->offset.x, position.y - button_pressed_->offset.y};
}

void button::set_size(const Vector2f size) const
{
	button_enabled_->size = size;
	button_selected_->size = size;
	button_pressed_->size = size;
}

void button::become_gray()
{
	is_gray_ = true;
}

void button::stop_being_gray()
{
	is_gray_ = false;
}

SpriteChainElement* button::prepare_sprite()
{
	SpriteChainElement* result;
	is_active = true;
	if (is_selected(Vector2f(Mouse::getPosition())) && is_selectable)
	{
		if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
		{
			result = new SpriteChainElement(
				button_pressed_->packTag,
				button_pressed_->packPart,
				button_pressed_->direction,
				button_pressed_->number,
				button_pressed_->position,
				button_pressed_->size,
				button_pressed_->offset,
				button_pressed_->color,
				button_pressed_->mirrored,
				button_pressed_->unscaled,
				button_pressed_->rotation);
		}
		else
		{
			result = new SpriteChainElement(
				button_selected_->packTag,
				button_selected_->packPart,
				button_selected_->direction,
				button_selected_->number,
				button_selected_->position,
				button_selected_->size,
				button_selected_->offset,
				button_selected_->color,
				button_selected_->mirrored,
				button_selected_->unscaled,
				button_selected_->rotation);
		}
	}
	else
	{
		result = new SpriteChainElement(
			button_enabled_->packTag,
			button_enabled_->packPart,
			button_enabled_->direction,
			button_enabled_->number,
			button_enabled_->position,
			button_enabled_->size,
			button_enabled_->offset,
			button_enabled_->color,
			button_enabled_->mirrored,
			button_enabled_->unscaled,
			button_enabled_->rotation);
	}

	if (is_gray_)
	{
		result->color = sf::Color(100, 100, 100, 150);
	}

	return result;
}
