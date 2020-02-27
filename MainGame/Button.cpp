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
	this->button->initialize(); this->buttonSelected->initialize(); this->buttonPressed->initialize();
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
	this->buttonSelected->position = { position.x - buttonSelected->offset.x, position.y - buttonSelected->offset.y };
	this->buttonPressed->position = { position.x - buttonPressed->offset.x, position.y - buttonPressed->offset.y };
}

void Button::setSize(Vector2f size)
{
	this->button->size = size;
	this->buttonSelected->size = size;
	this->buttonPressed->size = size;
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
	SpriteChainElement* result;
	isActive = true;
	if (isSelected(Vector2f(Mouse::getPosition())) && isSelectable)
	{
		if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
			result = new SpriteChainElement(
				buttonPressed->packTag,
				buttonPressed->packPart,
				buttonPressed->direction,
				buttonPressed->number,
				buttonPressed->position,
				buttonPressed->size,
				buttonPressed->offset,
				buttonPressed->color,
				buttonPressed->mirrored,
				buttonPressed->unscaled,
				buttonPressed->rotation);
		else result = new SpriteChainElement(
			buttonSelected->packTag, 
			buttonSelected->packPart, 
			buttonSelected->direction,
			buttonSelected->number, 
			buttonSelected->position,
			buttonSelected->size,
			buttonSelected->offset,
			buttonSelected->color, 
			buttonSelected->mirrored,
			buttonSelected->unscaled,
			buttonSelected->rotation);
	}
	else result = new SpriteChainElement(
		button->packTag,
		button->packPart, 
		button->direction,
		button->number,
		button->position,
		button->size,
		button->offset, 
		button->color,
		button->mirrored,
		button->unscaled,
		button->rotation);

	if (isGray)
		result->color = sf::Color(100, 100, 100, 150);	

	return result;
}
