#include "WorldObject.h"
#include "Helper.h"

Vector2f WorldObject::microBlockSize = {20, 20};

WorldObject::WorldObject(std::string objectName, Vector2f centerPosition)
{
	name = objectName;
	position = centerPosition;
	WorldObject::initPedestal();
	currentSprite.resize(1);
	currentSprite[0] = 1;
}

WorldObject::~WorldObject()
= default;

void WorldObject::setTextureSize(Vector2f textureSize)
{
	textureBox.width = textureSize.x;
	textureBox.height = textureSize.y;
	originalTextureBox = textureBox;
	textureBoxOffset = calculateTextureOffset();

	textureBox.left = int(position.x) - textureBoxOffset.x;
	textureBox.top = int(position.y) - textureBoxOffset.y;
	
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

bool WorldObject::isLockedPlace(std::map<std::pair<int, int>, bool> checkBlocks)
{
	if (!checkBlocks.empty() && tag == Tag::totem)
		int test = 123;
	for (auto& block : lockedMicroBlocks)
		if (checkBlocks.count({ block.x, block.y }) > 0)
			return true;
	return false;
}

void WorldObject::setPosition(Vector2f newPosition)
{
	position = Vector2f(newPosition);
	textureBox.left = int(newPosition.x) - textureBoxOffset.x;
	textureBox.top = int(newPosition.y) - textureBoxOffset.y;
}

void WorldObject::takeDamage(float damage, Vector2f attackerPos)
{
	this->healthPoint -= damage / this->armor;
}

void WorldObject::onSpriteChange()
{
	return;
}


