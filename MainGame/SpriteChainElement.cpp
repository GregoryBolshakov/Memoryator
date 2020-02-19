#include "SpriteChainElement.h"

SpriteChainElement::SpriteChainElement() : DrawableChainElement()
{
}

SpriteChainElement::SpriteChainElement(
    PackTag tag,
    PackPart part,
    Direction direction,
    int number, 
    Vector2f position,
    Vector2f size,
    Vector2f offset,
	Color color,
	bool mirrored,
	bool unscaled,
    float rotation) : DrawableChainElement(position, size, offset, color)
{
    this->packTag = tag;
    this->packPart = part;
    this->direction = direction;
    this->number = number;
	this->mirrored = mirrored;
	this->unscaled = unscaled;
    this->rotation = rotation;
}

SpriteChainElement::SpriteChainElement(
	Vector2f position,
	Vector2f size,
	Vector2f offset,
	Color color,
	bool mirrored,
	bool unscaled,
	float rotation) : DrawableChainElement(position, size, offset, color)
{
	this->mirrored = mirrored;
	this->unscaled = unscaled;
	this->rotation = rotation;
}

SpriteChainElement::~SpriteChainElement()
= default;

void SpriteChainElement::setDrawInfo(PackTag tag, PackPart part, Direction direction, int number) 
{
	this->packTag = tag;
	this->packPart = part;
	this->direction = direction;
	this->number = number;
}
