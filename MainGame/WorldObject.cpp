#include "WorldObject.h"
#include "Helper.h"

Vector2f WorldObject::microBlockSize = {20, 20};

WorldObject::WorldObject(std::string objectName, const Vector2f centerPosition)
{
	name = std::move(objectName);
	position = centerPosition;
	WorldObject::initPedestal();
	currentSprite.resize(1);
	currentSprite[0] = 1;
}

WorldObject::~WorldObject()
= default;

void WorldObject::setTextureSize(const Vector2f textureSize)
{
	textureBox.width = textureSize.x;
	textureBox.height = textureSize.y;
	originalTextureBox = textureBox;
	textureBoxOffset = calculateTextureOffset();

	textureBox.left = ceil(position.x - textureBoxOffset.x);
	textureBox.top = ceil(position.y - textureBoxOffset.y);
	
	initPedestal();
}

Vector2f WorldObject::getScaleRatio() const
{
	return { float(conditionalSizeUnits.x) / originalTextureBox.width, float(conditionalSizeUnits.y) / originalTextureBox.height };
}

void WorldObject::cancelMirroring()
{
	if (!mirrored)
		return;

	textureBoxOffset.x = conditionalSizeUnits.x - textureBoxOffset.x;
	mirrored = false;
}

void WorldObject::initPedestal()
{
	initMicroBlocks();
}

void WorldObject::initMicroBlocks()
{
}

bool WorldObject::isLockedPlace(const std::map<std::pair<int, int>, bool>& checkBlocks)
{
	for (auto& block : lockedMicroBlocks)
		if (checkBlocks.count({ block.x, block.y }) > 0)
			return true;
	
	return false;
}

void WorldObject::setPosition(const Vector2f newPosition)
{
	position = Vector2f(newPosition);
	textureBox.left = ceil(newPosition.x - textureBoxOffset.x);
	textureBox.top = ceil(newPosition.y - textureBoxOffset.y);
}

void WorldObject::takeDamage(const float damage, Vector2f attackerPos)
{
	this->healthPoint -= damage / this->armor;
}

void WorldObject::onSpriteChange()
{
}

float WorldObject::getMaxHealthPointValue() const
{
	return maxHealthPointValue;
}

