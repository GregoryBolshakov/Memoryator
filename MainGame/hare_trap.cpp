#include "hare_trap.h"

#include "helper.h"

hare_trap::hare_trap(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : picked_object(std::move(objectName), centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 100;
	toSaveName = "hareTrap";
	hare_trap::setType(typeOfObject);
	currentSprite[0] = 1;
	id = entity_tag::hareTrap;
	tag = entity_tag::hareTrap;
	inventory = hero_bag::emptyInventory;
}

void hare_trap::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = { 140, 111 };
}

Vector2f hare_trap::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2.0f, textureBox.height / 1.2f };
}

void hare_trap::initPedestal()
{
	focus1 = position;
	focus2 = position;
	ellipseSizeMultipliers[0] = { 0 };
}

Vector2f hare_trap::getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

Vector2f hare_trap::getEnterPosition() const
{
	return {position.x - conditionalSizeUnits.x / 2.0f, position.y + conditionalSizeUnits.y / 10.0f };
}

int hare_trap::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<sprite_chain_element*> hare_trap::prepareSprites(long long elapsedTime)
{
	const auto body = new sprite_chain_element(pack_tag::craftObjects, pack_part::hareTrap, Direction::DOWN, 1, position, conditionalSizeUnits, Vector2f(textureBoxOffset));
	body->isBackground = true;

	return { body };
}