#include "Mushroom.h"
#include "Helper.h"

Mushroom::Mushroom(std::string objectName, Vector2f centerPosition, int typeOfObject) : StaticObject(objectName, centerPosition)
{
	varietyOfTypes = 16; // BirchGrove: 1-3; DarkWoods: 4-12; SwampyTrees: 13-16
	this->typeOfObject = typeOfObject;
	this->intangible = true;
	toSaveName = "mushroom";
	setType(typeOfObject);
	tag = Tag::mushroom;
	mirrored = bool(rand() % 2);
}

void Mushroom::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(72, 140);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(67, 178);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(82, 94);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i(143, 203);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i(120, 134);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2i(173, 161);
	if (typeOfObject == 7)
		conditionalSizeUnits = Vector2i(70, 134);
	if (typeOfObject == 8)
		conditionalSizeUnits = Vector2i(91, 81);
	if (typeOfObject == 9)
		conditionalSizeUnits = Vector2i(94, 78);
	if (typeOfObject == 10)
		conditionalSizeUnits = Vector2i(103, 137);
	if (typeOfObject == 11)
		conditionalSizeUnits = Vector2i(89, 71);
	if (typeOfObject == 12)
		conditionalSizeUnits = Vector2i(144, 122);
	if (typeOfObject == 13)
		conditionalSizeUnits = Vector2i(54, 60);
	if (typeOfObject == 14)
		conditionalSizeUnits = Vector2i(80, 74);
	if (typeOfObject == 15)
		conditionalSizeUnits = Vector2i(80, 80);
	if (typeOfObject == 16)
		conditionalSizeUnits = Vector2i(77, 53);
}

Vector2i Mushroom::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	if (typeOfObject == 0)
		return Vector2i(0, 0);

	return Vector2i(textureBox.width / 2, int(textureBox.height / 2));
}

Vector2f Mushroom::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Mushroom::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<SpriteChainElement> Mushroom::prepareSprites(long long elapsedTime)
{
	SpriteChainElement body(PackTag::darkWoods, PackPart::mushroom, Direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset));
	body.isBackground = true;

	if (typeOfObject >= 1 && typeOfObject <= 3)
		body.packTag = PackTag::birchGrove;
	if (typeOfObject >= 4 && typeOfObject <= 12)
	{
		body.packTag = PackTag::darkWoods;
		body.number -= 3;
	}
	if (typeOfObject >= 13 && typeOfObject <= 16)
	{
		body.packTag = PackTag::swampyTrees;
		body.number -= 12;
	}

	return { body };
}