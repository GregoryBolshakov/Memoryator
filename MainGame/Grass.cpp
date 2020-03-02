#include "grass.h"

#include "helper.h"

grass::grass(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : static_object(std::move(objectName), centerPosition)
{
	varietyOfTypes = 30; // BirchGrove: 1-8; DarkWoods: 9-21; SwampyTrees: 22-30
	this->typeOfObject = typeOfObject;
	this->intangible = true;
	toSaveName = "grass";
	grass::setType(typeOfObject);
	tag = entity_tag::grass;
	mirrored = bool(rand() % 2);
}

void grass::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = { 86, 71 };
	if (typeOfObject == 2)
		conditionalSizeUnits = { 146, 182 };
	if (typeOfObject == 3)
		conditionalSizeUnits = { 205, 214 };
	if (typeOfObject == 4)
		conditionalSizeUnits = { 205, 137 };
	if (typeOfObject == 5)
		conditionalSizeUnits = { 108, 189 };
	if (typeOfObject == 6)
		conditionalSizeUnits = { 116, 211 };
	if (typeOfObject == 7)
		conditionalSizeUnits = { 141, 240 };
	if (typeOfObject == 8)
		conditionalSizeUnits = { 220, 264 };
	if (typeOfObject == 9)
		conditionalSizeUnits = { 251, 120 };
	if (typeOfObject == 10)
		conditionalSizeUnits = { 222, 343 };
	if (typeOfObject == 11)
		conditionalSizeUnits = { 357, 330 };
	if (typeOfObject == 12)
		conditionalSizeUnits = { 386, 428 };
	if (typeOfObject == 13)
		conditionalSizeUnits = { 425, 291 };
	if (typeOfObject == 14)
		conditionalSizeUnits = { 87, 183 };
	if (typeOfObject == 15)
		conditionalSizeUnits = { 113, 202 };
	if (typeOfObject == 16)
		conditionalSizeUnits = { 67, 158 };
	if (typeOfObject == 17)
		conditionalSizeUnits = { 119, 171 };
	if (typeOfObject == 18)
		conditionalSizeUnits = { 178, 170 };
	if (typeOfObject == 19)
		conditionalSizeUnits = { 188, 129 };
	if (typeOfObject == 20)
		conditionalSizeUnits = { 136, 171 };
	if (typeOfObject == 21)
		conditionalSizeUnits = { 171, 288 };
	if (typeOfObject == 22)
		conditionalSizeUnits = { 491, 209 };
	if (typeOfObject == 23)
		conditionalSizeUnits = { 166, 124 };
	if (typeOfObject == 24)
		conditionalSizeUnits = { 78, 255 };
	if (typeOfObject == 25)
		conditionalSizeUnits = { 171, 173 };
	if (typeOfObject == 26)
		conditionalSizeUnits = { 123, 137 };
	if (typeOfObject == 27)
		conditionalSizeUnits = { 133, 136 };
	if (typeOfObject == 28)
		conditionalSizeUnits = { 91, 112 };
	if (typeOfObject == 29)
		conditionalSizeUnits = { 119, 125 };
	if (typeOfObject == 30)
		conditionalSizeUnits = { 134, 84 };
}

Vector2f grass::calculateTextureOffset()
{	
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	
	return { textureBox.width / 2.0f, textureBox.height / 1.2f };
}

Vector2f grass::getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int grass::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<sprite_chain_element*> grass::prepareSprites(long long elapsedTime)
{
	std::vector<sprite_chain_element*> result = {};
	sprite_chain_element* body = new sprite_chain_element(pack_tag::darkWoods, pack_part::plant, direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset));
	body->unscaled = true;

	if (typeOfObject >= 1 && typeOfObject <= 8)
		body->pack_tag = pack_tag::birchGrove;
	if (typeOfObject >= 9 && typeOfObject <= 21)
	{
		body->pack_tag = pack_tag::darkWoods;
		body->number -= 8;
	}
	if (typeOfObject >= 22 && typeOfObject <= 30)
	{
		body->pack_tag = pack_tag::swampyTrees;
		body->number -= 21;
	}
	result.push_back(body);

	setUnscaled(result);

	return result;
}