#include "WorldObject.h"
#include "Helper.h"

Vector2i WorldObject::microBlockSize = {20, 20};

WorldObject::WorldObject(std::string objectName, Vector2f centerPosition)
{
	name = objectName;
	position = centerPosition;
	WorldObject::initPedestal();
	currentSprite.resize(1);
	currentSprite[0] = 1;
}

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

Vector2f WorldObject::getScaleRatio()
{
	return Vector2f (float(conditionalSizeUnits.x) / originalTextureBox.width, float(conditionalSizeUnits.y) / originalTextureBox.height);
}

/*Vector2i WorldObject::calculateTextureOffset()
{
	return Vector2i(0, 0);
}*/

void WorldObject::initPedestal()
{
	initMicroBlocks();
}

void WorldObject::initMicroBlocks()
{
	microBlockCheckAreaBounds = { radius, radius };
	lockedMicroBlocks.clear();
	permissibleDistance = 0;
	const Vector2i currentMicroBlock = Vector2i(position.x / microBlockSize.x, position.y / microBlockSize.y);
	for (int i = -microBlockCheckAreaBounds.x / microBlockSize.x; i <= int(microBlockCheckAreaBounds.x / microBlockSize.x); i++)
		for (int j = -microBlockCheckAreaBounds.y / microBlockSize.y; j <= int(microBlockCheckAreaBounds.y / microBlockSize.y); j++)
			if (Helper::getDist(Vector2f(position.x + i * microBlockSize.x, position.y + j * microBlockSize.y), position) <= microBlockCheckAreaBounds.x - sqrt(2 * microBlockSize.x))
			{
				lockedMicroBlocks.emplace_back(currentMicroBlock.x + i, currentMicroBlock.y + j);
				if (abs(i) > permissibleDistance)
					permissibleDistance = abs(i);
				if (abs(j) > permissibleDistance)
					permissibleDistance = abs(j);
			}

	permissibleDistance++;
}

bool WorldObject::isLockedPlace(std::map<std::pair<int, int>, bool> checkBlocks)
{
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

WorldObject::~WorldObject()
{

}

float WorldObject::getMaxHealthPointValue()
{
	return maxHealthPointValue;
}

