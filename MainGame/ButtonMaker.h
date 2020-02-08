#ifndef BUTTONMAKER
#define BUTTONMAKER

#include <SFML/Graphics.hpp>
#include "SpriteChainElement.h"

using namespace sf;

class ButtonMaker
{
private:
    SpriteChainElement button, buttonSelected, buttonPressed;
	ButtonTag tag;
	bool selected = false, isGray = false;
public:
	ButtonMaker();
	void initialize(
        PackTag pack,
        PackPart packPart, 
        int defaultSpriteNumber,
        int selectedSpriteNumber, 
        int pressedSpriteNumber,
        Vector2f position,
        Vector2i size,
        bool isSelectable,
        ButtonTag tag, 
        Vector2f offset = {0, 0});
	SpriteChainElement& prepareSprite();
	void bekomeGray();
	void stopBeingGray();
	bool isSelected(Vector2f mousePos);
	bool isSelectable, isActive = true;    
	Vector2f getPosition() const { return button.position; }
    Vector2i getSize() const { return button.size; }
    Vector2f getOffset() const { return button.offset; }
	void setPosition(Vector2f position);
	void setSize(Vector2i size);
	ButtonTag getTag() const { return tag; }
	bool getGrayState() const { return isGray; }
};
#endif

