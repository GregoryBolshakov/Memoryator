#include "Spawn.h"
#include "Helper.h"

Spawn::Spawn(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : TerrainObject(std::move(objectName), centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 200;
	toSaveName = "spawn";
	Spawn::setType(typeOfObject);
}

void Spawn::setType(const int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = { 1000, 1000 };
}

Vector2f Spawn::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	return { textureBox.width / 2.0f, textureBox.height / 1.6f };
}

void Spawn::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2f (textureBox.left + floor(textureBox.width / 3.8f), position.y);
		focus2 = Vector2f (textureBox.left + textureBox.width - floor(textureBox.width / 3.8f), position.y);
	}
	if (typeOfObject == 2)
	{
		focus1 = Vector2f (textureBox.left + floor(textureBox.width / 2.3f), position.y);
		focus2 = Vector2f (textureBox.left + textureBox.width - floor(textureBox.width / 3.3f), position.y);	
	}
	if (typeOfObject == 3)
	{
		focus1 = Vector2f (textureBox.left + floor(textureBox.width / 3.5f), position.y + textureBox.height / 20.0f);
		focus2 = Vector2f (textureBox.left + textureBox.width - floor(textureBox.width / 2.0f), position.y + textureBox.height / 100.0f);
	}
	ellipseSizeMultipliers = { 1.08f, 1.2f, 1.3f };
}

Vector2f Spawn::getBuildPosition(std::vector<WorldObject*>, float, Vector2f)
{
	return { -1, -1 };
}

int Spawn::getBuildType(Vector2f, Vector2f)
{
	return 1;
}

std::vector<SpriteChainElement*> Spawn::prepareSprites(long long)
{
    return {};
	/*additionalSprites.clear();
	spriteChainElement spawnBody;
	spawnBody.path = "Game/worldSprites/terrainObjects/brazier/brazier" + std::to_string(typeOfObject) + ".png";
	spawnBody.size = Vector2f(conditionalSizeUnits);
	spawnBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(spawnBody);*/
}