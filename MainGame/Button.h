#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include "SpriteChainElement.h"

using namespace sf;

class Button
{
private:
	SpriteChainElement* button = new SpriteChainElement();
	SpriteChainElement* buttonSelected = new SpriteChainElement();
	SpriteChainElement* buttonPressed = new SpriteChainElement();
	ButtonTag tag;
	bool selected = false, isGray = false;
public:
	Button();
	void initialize(
        PackTag pack,
        PackPart packPart, 
        int defaultSpriteNumber,
        int selectedSpriteNumber, 
        int pressedSpriteNumber,
        Vector2f position,
        Vector2f size,
        bool isSelectable,
        ButtonTag tag, 
        Vector2f offset = {0, 0});
	SpriteChainElement* prepareSprite();
	void bekomeGray();
	void stopBeingGray();
	bool isSelected(Vector2f mousePos);
	bool isSelectable, isActive = true;    
	Vector2f getPosition() const { return button->position; }
    Vector2f getSize() const { return button->size; }
    Vector2f getOffset() const { return button->offset; }
	FloatRect getGlobalBounds() const { return { getPosition(), getSize() }; }
	void setPosition(Vector2f position);
	void setSize(Vector2f size);
	ButtonTag getTag() const { return tag; }
	bool getGrayState() const { return isGray; }
};
#endif

