#include "forest_tree.h"

#include "helper.h"

forest_tree::forest_tree(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
{
	varietyOfTypes = 18; // BirchGrove: 1-7; DarkWoods: 8-13; SwampyTrees: 14-18
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 10;
	toSaveName = "ForestTree";
	forest_tree::setType(typeOfObject);
	tag = entity_tag::tree;
}

void forest_tree::setType(const int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = { 396, 1090 };
	if (typeOfObject == 2)
		conditionalSizeUnits = { 536, 1150 };
	if (typeOfObject == 3)
		conditionalSizeUnits = { 244, 799 };
	if (typeOfObject == 4)
		conditionalSizeUnits = { 623, 875 };
	if (typeOfObject == 5)
		conditionalSizeUnits = { 644, 1235 };
	if (typeOfObject == 6)
		conditionalSizeUnits = { 681, 1027 };
	if (typeOfObject == 7)
		conditionalSizeUnits = { 616, 1169 };
	if (typeOfObject == 8)
		conditionalSizeUnits = { 457, 1152 };
	if (typeOfObject == 9)
		conditionalSizeUnits = { 527, 1188 };
	if (typeOfObject == 10)
		conditionalSizeUnits = { 786, 1296 };
	if (typeOfObject == 11)
		conditionalSizeUnits = { 806, 1292 };
	if (typeOfObject == 12)
		conditionalSizeUnits = { 795, 1190 };
	if (typeOfObject == 13)
		conditionalSizeUnits = { 1221, 386 };
	if (typeOfObject == 14)
		conditionalSizeUnits = { 952, 962 };
	if (typeOfObject == 15)
		conditionalSizeUnits = { 565, 1261 };
	if (typeOfObject == 16)
		conditionalSizeUnits = { 914, 1108 };
	if (typeOfObject == 17)
		conditionalSizeUnits = { 652, 1359 };
	if (typeOfObject == 18)
		conditionalSizeUnits = { 667, 717 };

	//const auto extension = float(90 + rand() % 20) / 100.0f; // in percents
	//conditionalSizeUnits.x *= extension; conditionalSizeUnits.y *= extension;
}

Vector2f forest_tree::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;

	switch (typeOfObject)
	{
		case 1:
			return { textureBox.width * 0.257f, textureBox.height * 0.932f };
		case 2:
			return { textureBox.width * 0.45f, textureBox.height * 0.96f };
		case 3:
			return { textureBox.width * 0.55f, textureBox.height * 0.96f };
		case 4:
			return { textureBox.width * 0.409f, textureBox.height * 0.945f };
		case 5:
			return { textureBox.width * 0.41f, textureBox.height * 0.935f };
		case 6:
			return { textureBox.width * 0.536f, textureBox.height * 0.94f };
		case 7:
			return { textureBox.width * 0.526f, textureBox.height * 0.936f };
		case 8:
			return { textureBox.width * 0.560f, textureBox.height * 0.96f };
		case 9:
		case 10:
			return { textureBox.width * 0.467f, textureBox.height * 0.94f };
		case 11:
			return { textureBox.width * 0.554f, textureBox.height * 0.95f };
		case 12:
			return { textureBox.width * 0.327f, textureBox.height * 0.93f };
		case 13:
			return { textureBox.width * 0.574f, textureBox.height * 0.508f };
		case 14:
			return { textureBox.width * 0.521f, textureBox.height * 0.883f };
		case 15:
			return { textureBox.width * 0.509f, textureBox.height * 0.857f };
		case 16:
			return { textureBox.width * 0.537f, textureBox.height * 0.83f };
		case 17:
			return { textureBox.width * 0.349f, textureBox.height * 0.861f };
		case 18:
			return { textureBox.width * 0.439f, textureBox.height * 0.857f };
		default:
			return { textureBox.width * 0.45f, textureBox.height * 0.96f };
	}
}

void forest_tree::initPedestal()
{
	std::pair<Vector2f, Vector2f> microEllipse;
	switch (typeOfObject)
	{
		case 1:		
			focus1 = Vector2f(position.x - textureBox.width * 0.158f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.158f, position.y);
			ellipseSizeMultipliers[0] = { 1.44f };
			break;		
		case 2:		
			focus1 = Vector2f(position.x - textureBox.width * 0.07f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.07f, position.y);
			ellipseSizeMultipliers[0] = { 1.58f };
			break;		
		case 3:		
			focus1 = Vector2f(position.x - textureBox.width * 0.184f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.184f, position.y);
			ellipseSizeMultipliers[0] = { 1.42f };
			break;		
		case 4:		
			focus1 = Vector2f(position.x - textureBox.width * 0.082f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.082f, position.y);
			ellipseSizeMultipliers[0] = { 1.42f };
			break;		
		case 5:		
			focus1 = Vector2f(position.x - textureBox.width * 0.088f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.088f, position.y);
			ellipseSizeMultipliers[0] = { 1.57f };
			break;		
		case 6:		
			focus1 = Vector2f(position.x - textureBox.width * 0.084f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.084f, position.y);
			ellipseSizeMultipliers[0] = { 1.31f };
			break;		
		case 7:		
			focus1 = Vector2f(position.x - textureBox.width * 0.131f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.131f, position.y);
			ellipseSizeMultipliers[0] = { 1.28f };
			break;		
		case 8:		
			focus1 = Vector2f(position.x - textureBox.width * 0.118f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.118f, position.y);
			ellipseSizeMultipliers[0] = { 1.42f };
			break;
		case 9:		
			focus1 = Vector2f(position.x - textureBox.width * 0.124f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.124f, position.y);
			ellipseSizeMultipliers[0] = { 1.21f };
			break;		
		case 10:		
			focus1 = Vector2f(position.x - textureBox.width * 0.127f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.127f, position.y);
			ellipseSizeMultipliers[0] = { 1.25f };
			break;		
		case 11:		
			focus1 = Vector2f(position.x - textureBox.width * 0.163f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.163f, position.y);
			ellipseSizeMultipliers[0] = { 1.25f };
			break;		
		case 12:		
			focus1 = Vector2f(position.x - textureBox.width * 0.137f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.137f, position.y);
			ellipseSizeMultipliers[0] = { 1.25f };
			break;		
		case 13:
			this->isMultiEllipse = true;
			focus1 = Vector2f(position.x, position.y);
			focus2 = Vector2f(position.x, position.y);

			microEllipse.first = Vector2f(position.x - textureBox.width * 0.42f, position.y + textureBox.height * 0.247f);
			microEllipse.second = Vector2f(position.x - textureBox.width * 0.178f, position.y + textureBox.height * 0.247f);
			internalEllipses.push_back(microEllipse);

			microEllipse.first = Vector2f(position.x - textureBox.width * 0.235f, position.y + textureBox.height * 0.171f);
			microEllipse.second = Vector2f(position.x - textureBox.width * 0.026f, position.y + textureBox.height * 0.171f);
			internalEllipses.push_back(microEllipse);

			microEllipse.first = Vector2f(position.x - textureBox.width * 0.044f, position.y + textureBox.height * 0.047f);
			microEllipse.second = Vector2f(position.x + textureBox.width * 0.132f, position.y + textureBox.height * 0.047f);
			internalEllipses.push_back(microEllipse);

			microEllipse.first = Vector2f(position.x + textureBox.width * 0.14f, position.y - textureBox.height * 0.012f);
			microEllipse.second = Vector2f(position.x + textureBox.width * 0.385f, position.y - textureBox.height * 0.012f);
			internalEllipses.push_back(microEllipse);
			ellipseSizeMultipliers = { 1.03f, 1.09f, 1.27f, 1.12f };
			break;
		case 14:
			focus1 = Vector2f(position.x - textureBox.width * 0.079f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.079f, position.y);
			ellipseSizeMultipliers[0] = { 1.42f };
			break;
		case 15:
			focus1 = Vector2f(position.x - textureBox.width * 0.09f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.09f, position.y);
			ellipseSizeMultipliers[0] = { 1.55f };
			break;
		case 16:
			focus1 = Vector2f(position.x - textureBox.width * 0.066f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.066f, position.y);
			ellipseSizeMultipliers[0] = { 1.46f };
			break;
		case 17:
			focus1 = Vector2f(position.x - textureBox.width * 0.162f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.162f, position.y);
			ellipseSizeMultipliers[0] = { 1.25f };
			break;
		case 18:
			focus1 = Vector2f(position.x - textureBox.width * 0.109f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.109f, position.y);
			ellipseSizeMultipliers[0] = { 1.25f };
			break;
		default:		
			focus1 = Vector2f(position.x - textureBox.width * 0.106f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.106f, position.y);
			ellipseSizeMultipliers[0] = { 1.82f };
			break;		
	}
	initMicroBlocks();
}

Vector2f forest_tree::getBuildPosition(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

Vector2f forest_tree::getOwlBase() const
{
	return { position.x, position.y - conditionalSizeUnits.y / 2 };
}

int forest_tree::getBuildType(Vector2f, Vector2f)
{
	return 1;
}

std::vector<sprite_chain_element*> forest_tree::prepareSprites(const long long elapsedTime)
{
	auto body = new sprite_chain_element(pack_tag::darkWoods, pack_part::tree, direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset), color, mirrored);

	if (typeOfObject >= 1 && typeOfObject <= 7)
		body->pack_tag = pack_tag::birchGrove;
	if (typeOfObject >= 8 && typeOfObject <= 13)
	{
		body->pack_tag = pack_tag::darkWoods;
		body->number -= 7;
	}
	if (typeOfObject >= 14 && typeOfObject <= 18)
	{
		body->pack_tag = pack_tag::swampyTrees;
		body->number -= 13;
	}

	if (state == absorbed)
	{
		body->animation_length = 15;
		color.a = 255 - currentSprite[0] * 255 / body->animation_length;
	}

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= long(1e6 / animationSpeed))
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > body->animation_length)
		{
			if (state == absorbed)
			{
				state = common;
				deletePromiseOn();
			}
			currentSprite[0] = 1;
		}
	}

	return { body };
}