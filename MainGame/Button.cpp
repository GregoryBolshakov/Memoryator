#include "Button.h"

Button::Button()
{
}

void Button::initialize(
    PackTag pack,
    PackPart part, 
    int defaultSpriteNumber,
    int selectedSpriteNumber, 
    int pressedSpriteNumber,
    Vector2f position,
    Vector2f size,
    bool isSelectable, 
    ButtonTag tag, 
    Vector2f offset)
{
	this->tag = tag;	
	this->button->packTag = pack; this->buttonSelected->packTag = pack; this->buttonPressed->packTag = pack;
    this->button->packPart = part; this->buttonSelected->packPart = part; this->buttonPressed->packPart = part;
    this->button->number = defaultSpriteNumber; this->buttonSelected->number = selectedSpriteNumber; this->buttonPressed->number = pressedSpriteNumber;
    this->button->position = position; this->buttonSelected->position = position; this->buttonPressed->position = position;
    this->button->size = size; this->buttonSelected->size = size; this->buttonPressed->size = size;
    this->button->offset = offset; this->buttonSelected->offset = offset; this->buttonPressed->offset = offset;
	this->isSelectable = isSelectable;	
}

bool Button::isSelected(Vector2f mousePos)
{
	//if (!isActive)
		//return false;

	//isActive = false;

	FloatRect rect = FloatRect(button->position.x, button->position.y, button->size.x, button->size.y);
	if (rect.contains(Vector2f(mousePos)))
	{
		selected = true;
		return true;		
	}
	else
	{
		selected = false;
		return false;
	}

	selected = false;
	return false;
}

void Button::setPosition(Vector2f position)
{
	this->button->position = {position.x - button->offset.x, position.y - button->offset.y};
}

void Button::setSize(Vector2f size)
{
	this->button->size = size;
}

void Button::bekomeGray()
{
	isGray = true;
}

void Button::stopBeingGray()
{
	isGray = false;
}

SpriteChainElement* Button::prepareSprite()
{
	isActive = true;
	if (isSelected(Vector2f(Mouse::getPosition())) && isSelectable)
	{
		if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
			return buttonPressed;
		return buttonSelected;
	}
	return button;
}
