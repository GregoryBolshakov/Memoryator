#include "DroppedLoot.h"
#include "Helper.h"

DroppedLoot::DroppedLoot(std::string objectName, Vector2f centerPosition, int typeOfObject, int count) : PickedObject(objectName, centerPosition)
{
	varietyOfTypes = 5;
	this->typeOfObject = typeOfObject;
	toSaveName = "drop";
	DroppedLoot::setType(typeOfObject);
	radius = SpritePack::iconSize.x / 2;
	tag = Tag::droppedLoot;
	this->count = count;
}

void DroppedLoot::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	id = Tag(typeOfObject);
	conditionalSizeUnits = SpritePack::iconSize;
}

Vector2f DroppedLoot::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2.0f, textureBox.height / 2.0f };
}

void DroppedLoot::initPedestal()
{
	focus1 = position;
	focus2 = position;
	ellipseSizeMultipliers[0] = { 0 };
}

Vector2f DroppedLoot::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int DroppedLoot::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<SpriteChainElement*> DroppedLoot::prepareSprites(long long elapsedTime)
{
	SpriteChainElement* body = new SpriteChainElement(PackTag::empty, PackPart::full, Direction::DOWN, this->isSelected, position, conditionalSizeUnits, textureBoxOffset);	

	switch (typeOfObject)
	{
	case 401:
		body->setDrawInfo(PackTag::icons, PackPart::flowers, Direction::DOWN, body->number + 1);	
		break;
	case 402:
		body->setDrawInfo(PackTag::icons, PackPart::flowers, Direction::DOWN, body->number + 9);
		break;
	case 201:
		body->setDrawInfo(PackTag::inventory, PackPart::bag1, Direction::DOWN, body->number + 1);
		break;
	case 202:
		body->setDrawInfo(PackTag::icons, PackPart::craftObjects, Direction::DOWN, body->number + 5);
		break;
	case 219:
		body->setDrawInfo(PackTag::icons, PackPart::notCraftObjects, Direction::DOWN, 1);
		break;
	case 102:
		body->setDrawInfo(PackTag::icons, PackPart::mobs, Direction::DOWN, 3);
		break;
	default:
		body->setDrawInfo(PackTag::inventory, PackPart::areas, Direction::DOWN, 1);
		break;
	}

	return { body };


	

}