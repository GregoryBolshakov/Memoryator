#include "Mushroom.h"
#include "Helper.h"

Mushroom::Mushroom(std::string objectName, Vector2f centerPosition, int typeOfObject) : StaticObject(objectName, centerPosition)
{
	varietyOfTypes = 12;
	this->typeOfObject = typeOfObject;
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
		conditionalSizeUnits = Vector2i(120, 134);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i(109, 151);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2i(55, 130);
	if (typeOfObject == 7)
		conditionalSizeUnits = Vector2i(90, 80);
	if (typeOfObject == 8)
		conditionalSizeUnits = Vector2i(86, 75);
	if (typeOfObject == 9)
		conditionalSizeUnits = Vector2i(96, 135);
	if (typeOfObject == 10)
		conditionalSizeUnits = Vector2i(75, 62);
	if (typeOfObject == 11)
		conditionalSizeUnits = Vector2i(144, 120);
	if (typeOfObject == 12)
		conditionalSizeUnits = Vector2i(154, 199);
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

void Mushroom::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement grassBody;
	if (typeOfObject >= 1 && typeOfObject <= 3)
		grassBody.path = "Game/worldSprites/BirchGrove/mushroom" + std::to_string(typeOfObject) + ".png";
	if (typeOfObject >= 4 && typeOfObject <= 12)
		grassBody.path = "Game/worldSprites/DarkWoods/amanita" + std::to_string(typeOfObject - 3) + ".png";
	grassBody.size = Vector2f(conditionalSizeUnits);
	grassBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(grassBody);
}