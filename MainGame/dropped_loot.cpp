#include "dropped_loot.h"

#include "helper.h"

dropped_loot::dropped_loot(std::string objectName, Vector2f centerPosition, int typeOfObject, int count) : picked_object(std::move(objectName), centerPosition)
{
	varietyOfTypes = 5;
	this->typeOfObject = typeOfObject;
	toSaveName = "drop";
	dropped_loot::setType(typeOfObject);
	radius = sprite_pack::iconSize.x / 2;
	tag = entity_tag::droppedLoot;
	this->count = count;
}

void dropped_loot::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	id = entity_tag(typeOfObject);
	conditionalSizeUnits = sprite_pack::iconSize;
}

Vector2f dropped_loot::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2.0f, textureBox.height / 2.0f };
}

void dropped_loot::initPedestal()
{
	focus1 = position;
	focus2 = position;
	ellipseSizeMultipliers[0] = { 0 };
}

Vector2f dropped_loot::getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int dropped_loot::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<sprite_chain_element*> dropped_loot::prepareSprites(long long elapsedTime)
{
	sprite_chain_element* body = new sprite_chain_element(pack_tag::empty, pack_part::full, Direction::DOWN, this->isSelected, position, conditionalSizeUnits, textureBoxOffset);	

	switch (typeOfObject)
	{
	case 401:
		body->setDrawInfo(pack_tag::icons, pack_part::flowers, Direction::DOWN, body->number + 1);	
		break;
	case 402:
		body->setDrawInfo(pack_tag::icons, pack_part::flowers, Direction::DOWN, body->number + 9);
		break;
	case 201:
		body->setDrawInfo(pack_tag::inventory, pack_part::bag1, Direction::DOWN, body->number + 1);
		break;
	case 202:
		body->setDrawInfo(pack_tag::icons, pack_part::craftObjects, Direction::DOWN, body->number + 5);
		break;
	case 219:
		body->setDrawInfo(pack_tag::icons, pack_part::notCraftObjects, Direction::DOWN, 1);
		break;
	case 102:
		body->setDrawInfo(pack_tag::icons, pack_part::mobs, Direction::DOWN, 3);
		break;
	default:
		body->setDrawInfo(pack_tag::inventory, pack_part::areas, Direction::DOWN, 1);
		break;
	}

	return { body };


	

}