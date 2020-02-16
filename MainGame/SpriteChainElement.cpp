#include "SpriteChainElement.h"

SpriteChainElement::SpriteChainElement()
{
    
}

SpriteChainElement::SpriteChainElement(
    PackTag tag,
    PackPart part,
    Direction direction, 
    int number, 
    Vector2f position,
    Vector2i size,
    Vector2f offset,
	Color color,
	bool mirrored,
    float rotation)
{
    this->packTag = tag;
    this->packPart = part;
    this->direction = direction;
    this->number = number;
    this->position = position;
    this->size = size;
    this->offset = offset;
	this->color = color;
	this->mirrored = mirrored;
    this->rotation = rotation;
}

SpriteChainElement::~SpriteChainElement()
{
    
}
