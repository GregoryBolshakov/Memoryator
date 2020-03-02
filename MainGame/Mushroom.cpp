#include "mushroom.h"

#include "Helper.h"

mushroom::mushroom(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : StaticObject(std::move(objectName), centerPosition)
{
	varietyOfTypes = 16; // BirchGrove: 1-3; DarkWoods: 4-12; SwampyTrees: 13-16
	this->typeOfObject = typeOfObject;
	this->intangible = true;
	toSaveName = "mushroom";
	mushroom::setType(typeOfObject);
	tag = Tag::mushroom;
	mirrored = bool(rand() % 2);
}

void mushroom::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = { 72, 140 };
	if (typeOfObject == 2)
		conditionalSizeUnits = { 67, 178 };
	if (typeOfObject == 3)
		conditionalSizeUnits = { 82, 94 };
	if (typeOfObject == 4)
		conditionalSizeUnits = { 143, 203 };
	if (typeOfObject == 5)
		conditionalSizeUnits = { 120, 134 };
	if (typeOfObject == 6)
		conditionalSizeUnits = { 173, 161 };
	if (typeOfObject == 7)
		conditionalSizeUnits = { 70, 134 };
	if (typeOfObject == 8)
		conditionalSizeUnits = { 91, 81 };
	if (typeOfObject == 9)
		conditionalSizeUnits = { 94, 78 };
	if (typeOfObject == 10)
		conditionalSizeUnits = { 103, 137 };
	if (typeOfObject == 11)
		conditionalSizeUnits = { 89, 71 };
	if (typeOfObject == 12)
		conditionalSizeUnits = { 144, 122 };
	if (typeOfObject == 13)
		conditionalSizeUnits = { 54, 60 };
	if (typeOfObject == 14)
		conditionalSizeUnits = { 80, 74 };
	if (typeOfObject == 15)
		conditionalSizeUnits = { 80, 80 };
	if (typeOfObject == 16)
		conditionalSizeUnits = { 77, 53 };
}

Vector2f mushroom::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;

	if (typeOfObject == 0)
		return { 0, 0 };

	return { textureBox.width / 2.0f, textureBox.height / 2.0f };
}

Vector2f mushroom::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int mushroom::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<sprite_chain_element*> mushroom::prepareSprites(long long elapsedTime)
{
	sprite_chain_element* body = new sprite_chain_element(PackTag::darkWoods, PackPart::mushroom, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset));
	body->unscaled = true;

	if (typeOfObject >= 1 && typeOfObject <= 3)
		body->packTag = PackTag::birchGrove;
	if (typeOfObject >= 4 && typeOfObject <= 12)
	{
		body->packTag = PackTag::darkWoods;
		body->number -= 3;
	}
	if (typeOfObject >= 13 && typeOfObject <= 16)
	{
		body->packTag = PackTag::swampyTrees;
		body->number -= 12;
	}

	return { body };
}