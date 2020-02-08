#include "Grass.h"
#include "Helper.h"

Grass::Grass(std::string objectName, Vector2f centerPosition, int typeOfObject) : StaticObject(objectName, centerPosition)
{
	varietyOfTypes = 30; // BirchGrove: 1-7; DarkWoods: 8-21; SwampyTrees: 22-30
	this->typeOfObject = typeOfObject;
	this->intangible = true;
	toSaveName = "grass";
	setType(typeOfObject);
	tag = Tag::grass;
	mirrored = bool(rand() % 2);
}

void Grass::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(86, 71);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(146, 182);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(205, 214);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i(205, 137);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i(108, 189);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2i(116, 211);
	if (typeOfObject == 7)
		conditionalSizeUnits = Vector2i(141, 240);
	if (typeOfObject == 8)
		conditionalSizeUnits = Vector2i(220, 264);
	if (typeOfObject == 9)
		conditionalSizeUnits = Vector2i(57, 150);
	if (typeOfObject == 10)
		conditionalSizeUnits = Vector2i(75, 185);
	if (typeOfObject == 11)
		conditionalSizeUnits = Vector2i(85, 200);
	if (typeOfObject == 12)
		conditionalSizeUnits = Vector2i(119, 172);
	if (typeOfObject == 13)
		conditionalSizeUnits = Vector2i(178, 171);
	if (typeOfObject == 14)
		conditionalSizeUnits = Vector2i(200, 114);
	if (typeOfObject == 15)
		conditionalSizeUnits = Vector2i(249, 117);
	if (typeOfObject == 16)
		conditionalSizeUnits = Vector2i(136, 167);
	if (typeOfObject == 17)
		conditionalSizeUnits = Vector2i(171, 285);
	if (typeOfObject == 18)
		conditionalSizeUnits = Vector2i(222, 342);
	if (typeOfObject == 19)
		conditionalSizeUnits = Vector2i(389, 429);
	if (typeOfObject == 20)
		conditionalSizeUnits = Vector2i(357, 330);
	if (typeOfObject == 21)
		conditionalSizeUnits = Vector2i(425, 287);
	if (typeOfObject == 22)
		conditionalSizeUnits = Vector2i(469, 209);
	if (typeOfObject == 23)
		conditionalSizeUnits = Vector2i(166, 124);
	if (typeOfObject == 24)
		conditionalSizeUnits = Vector2i(78, 255);
	if (typeOfObject == 25)
		conditionalSizeUnits = Vector2i(171, 173);
	if (typeOfObject == 26)
		conditionalSizeUnits = Vector2i(123, 137);
	if (typeOfObject == 27)
		conditionalSizeUnits = Vector2i(133, 136);
	if (typeOfObject == 28)
		conditionalSizeUnits = Vector2i(91, 112);
	if (typeOfObject == 29)
		conditionalSizeUnits = Vector2i(119, 125);
	if (typeOfObject == 30)
		conditionalSizeUnits = Vector2i(134, 84);
}

Vector2i Grass::calculateTextureOffset()
{	
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	
	return Vector2i (textureBox.width / 2, int(textureBox.height / 1.2));
}

Vector2f Grass::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Grass::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::vector<SpriteChainElement> Grass::prepareSprites(long long elapsedTime)
{
    return {};
	/*animationSpeed = 10;
	additionalSprites.clear();
	spriteChainElement grassBody;
		
	animationLength = 1;
	if (typeOfObject >= 1 && typeOfObject <= 8)
		grassBody.path = "Game/worldSprites/BirchGrove/plant" + std::to_string(typeOfObject) + ".png";	
	if (typeOfObject >= 9 && typeOfObject <= 21)	
		grassBody.path = "Game/worldSprites/DarkWoods/plant" + std::to_string(typeOfObject - 8) + "/" + std::to_string(currentSprite[0]) + ".png";
	if (typeOfObject >= 22 && typeOfObject <= 30)
		grassBody.path = "Game/worldSprites/SwampyTrees/plant" + std::to_string(typeOfObject - 21) + ".png";
	
	grassBody.size = Vector2f(conditionalSizeUnits);
	grassBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(grassBody);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 1e6 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			if (state == absorbed)
			{
				state = common;
				deletePromiseOn();
			}
			currentSprite[0] = 1;
		}
	}*/
}