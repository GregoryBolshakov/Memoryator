#include "world_object.h"

#include "Helper.h"

Vector2f world_object::microBlockSize = {20, 20};

world_object::world_object(std::string objectName, const Vector2f centerPosition)
{
	name = std::move(objectName);
	position = centerPosition;
	world_object::initPedestal();
	currentSprite.resize(1);
	currentSprite[0] = 1;
}

world_object::~world_object()
= default;

void world_object::setTextureSize(const Vector2f textureSize)
{
	textureBox.width = textureSize.x;
	textureBox.height = textureSize.y;
	originalTextureBox = textureBox;
	textureBoxOffset = calculateTextureOffset();

	textureBox.left = ceil(position.x - textureBoxOffset.x);
	textureBox.top = ceil(position.y - textureBoxOffset.y);
	
	initPedestal();
}

Vector2f world_object::getScaleRatio() const
{
	return { float(conditionalSizeUnits.x) / originalTextureBox.width, float(conditionalSizeUnits.y) / originalTextureBox.height };
}

void world_object::cancelMirroring()
{
	if (!mirrored)
		return;

	textureBoxOffset.x = conditionalSizeUnits.x - textureBoxOffset.x;
	mirrored = false;
}

void world_object::initPedestal()
{
	initMicroBlocks();
}

void world_object::initMicroBlocks()
{
}

bool world_object::isLockedPlace(const std::map<std::pair<int, int>, bool>& checkBlocks)
{
	for (auto& block : lockedMicroBlocks)
		if (checkBlocks.count({ block.x, block.y }) > 0)
			return true;
	
	return false;
}

void world_object::setPosition(const Vector2f newPosition)
{
	position = Vector2f(newPosition);
	textureBox.left = ceil(newPosition.x - textureBoxOffset.x);
	textureBox.top = ceil(newPosition.y - textureBoxOffset.y);
}

void world_object::takeDamage(const float damage, Vector2f attackerPos)
{
	this->healthPoint -= damage / this->armor;
}

void world_object::onSpriteChange()
{
}

float world_object::getMaxHealthPointValue() const
{
	return maxHealthPointValue;
}

