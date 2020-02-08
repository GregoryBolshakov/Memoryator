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
    float rotation)
{
    this->packTag = tag;
    this->packPart = part;
    this->direction = direction;
    this->number = number;
    this->position = position;
    this->size = size;
    this->offset = offset;
    this->rotation = rotation;
}

SpriteChainElement::~SpriteChainElement()
{
    
}
