#include "HareTrap.h"

#include "Helper.h"

HareTrap::HareTrap(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : PickedObject(std::move(objectName), centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 100;
	toSaveName = "hareTrap";
	HareTrap::setType(typeOfObject);
	currentSprite[0] = 1;
	id = Tag::hareTrap;
	tag = Tag::hareTrap;
	inventory = HeroBag::emptyInventory;
}

void HareTrap::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = { 140, 111 };
}

Vector2f HareTrap::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2.0f, textureBox.height / 1.2f };
}

void HareTrap::initPedestal()
{
	focus1 = position;
	focus2 = position;
	ellipseSizeMultipliers[0] = { 0 };
}

Vector2f HareTrap::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

Vector2f HareTrap::getEnterPosition() const
{
	return {position.x - conditionalSizeUnits.x / 2.0f, position.y + conditionalSizeUnits.y / 10.0f };
}

int HareTrap::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<SpriteChainElement*> HareTrap::prepareSprites(long long elapsedTime)
{
	const auto body = new SpriteChainElement(PackTag::craftObjects, PackPart::hareTrap, Direction::DOWN, 1, position, conditionalSizeUnits, Vector2f(textureBoxOffset));
	body->isBackground = true;

	return { body };
}