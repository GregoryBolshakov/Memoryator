#include "Fence.h"

#include "Helper.h"

Fence::Fence(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : TerrainObject(std::move(objectName), centerPosition)
{
	varietyOfTypes = 3;
	this->typeOfObject = typeOfObject;
	radius = 120;
	toSaveName = "fence";
	Fence::setType(typeOfObject);
	isDotsAdjusted = true;
	tag = Tag::fence;
}

void Fence::setType(const int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	switch (typeOfObject)
	{
		case 1:
		case 2:
		{
			this->conditionalSizeUnits = { 240, 230 };
			break;
		}
		case 3:
		case 4:
		{
			this->conditionalSizeUnits = { 95, 350 };
			break;
		}
		default:
		{
			this->conditionalSizeUnits = { 300, 300 };
			break;
		}
	}
}

Vector2f Fence::calculateTextureOffset()
{
	textureBox.width = textureBox.width * getScaleRatio().x;
	textureBox.height = textureBox.height * getScaleRatio().y;
	switch (typeOfObject)
	{
		case 1:		
		case 2:
			return { textureBox.width / 2.0f, textureBox.height / 1.0f };
		case 3:
			return { textureBox.width / 2.0f, textureBox.height / 1.5f };
		case 4:
		default:
			return { textureBox.width / 2.0f, textureBox.height / 1.0f };
	}
}

void Fence::initPedestal()
{
	switch (typeOfObject)
	{
		case 1:
		case 2:
		{
			dot1 = Vector2f(position.x - textureBox.width / 3.2f, position.y);
			dot2 = Vector2f(position.x + textureBox.width / 1.8f, position.y);
			break;
		}
		case 3:
		{
			dot1 = Vector2f(position.x, position.y - textureBox.height / 9.0f);
			dot2 = Vector2f(position.x, position.y + textureBox.height / 2.8f);
			break;
		}
		case 4:
		{
			dot1 = Vector2f(position.x - textureBox.width / 4.0f, position.y/* - textureBox.height / 7*/);
			dot2 = Vector2f(position.x - textureBox.width / 4.0f, position.y + textureBox.height / 2.0f);
			break;
		}
		default:
		{
			dot1 = Vector2f(position.x - textureBox.width / 2.0f, position.y);
			dot2 = Vector2f(position.x + textureBox.width / 2.0f, position.y);
			break;
		}
	}

	//radius = sqrt(pow(dot1.x - dot2.x, 2) + pow(dot1.y + dot2.y, 2)) / 2 - 10;
}

Vector2f Fence::getBuildPosition(std::vector<WorldObject*> visibleItems, const float scaleFactor, const Vector2f cameraPosition)
{
	const auto mousePos = Vector2f (Mouse::getPosition());
	const auto mouseWorldPos = Vector2f ((mousePos.x - Helper::GetScreenSize().x / 2 + cameraPosition.x * scaleFactor) / scaleFactor,
	                                     (mousePos.y - Helper::GetScreenSize().y / 2 + cameraPosition.y * scaleFactor) / scaleFactor);

	const auto dot1 = Vector2f ((this->dot1.x - this->position.x) + mouseWorldPos.x, (this->dot1.y - this->position.y) + mouseWorldPos.y);
	const auto dot2 = Vector2f ((this->dot2.x - this->position.x) + mouseWorldPos.x, (this->dot2.y - this->position.y) + mouseWorldPos.y);

	for (auto&item : visibleItems)
	{
		if (item->tag == Tag::fence)
		{
			const auto object = dynamic_cast<Fence*>(item);

			auto const dist1 = sqrt(pow(dot1.x - object->getDot2().x, 2) + pow(dot1.y - object->getDot2().y, 2));
			auto const dist2 = sqrt(pow(dot2.x - object->getDot1().x, 2) + pow(dot2.y - object->getDot1().y, 2));
			auto const dist3 = sqrt(pow(dot1.x - object->getDot1().x, 2) + pow(dot1.y - object->getDot1().y, 2));
			auto const dist4 = sqrt(pow(dot2.x - object->getDot2().x, 2) + pow(dot2.y - object->getDot2().y, 2));

			auto ownDot = Vector2f (-1, -1);
			auto objDot = Vector2f (-1, -1);

			if (dist1 <= dist2 && dist1 <= dist3 && dist1 <= dist4 && dist1 < 100)
			{
				ownDot = Vector2f (this->position.x - this->dot1.x, this->position.y - this->dot1.y);
				objDot = Vector2f (object->getDot2());				
			}
			else if (dist2 <= dist1 && dist2 <= dist3 && dist2 <= dist4 && dist2 < 100)
			{
				ownDot = Vector2f (this->position.x - this->dot2.x, this->position.y - this->dot2.y);
				objDot = Vector2f (object->getDot1());				
			}
			else if (dist3 <= dist1 && dist3 <= dist2 && dist3 <= dist4 && dist3 < 100)
			{
				ownDot = Vector2f (this->position.x - this->dot1.x, this->position.y - this->dot1.y);
				objDot = Vector2f (object->getDot1());				
			}
			else if (dist4 <= dist1 && dist4 <= dist2 && dist4 <= dist3 && dist4 < 100)
			{
				ownDot = Vector2f (this->position.x - this->dot2.x, this->position.y - this->dot2.y);
				objDot = Vector2f (object->getDot2());				
			}	

			if (ownDot != Vector2f (-1, -1) && objDot != Vector2f (-1, -1))
			{			
				currentDot = Vector2f (objDot);
				return Vector2f (objDot.x + ownDot.x, objDot.y + ownDot.y);
			}
		}
	}
	return { -1, -1 };
}

int Fence::getBuildType(const Vector2f ounPos, const Vector2f otherPos)
{
	if (otherPos != Vector2f (-1, -1))
	{
		const auto side = Helper::getSide(ounPos, otherPos);
		if (side == up)
			return 3;
		if (side == down)
			return 3;
		if (side == left)
			return 1;
		if (side == right)
			return 2;
	}

	return 1;
}

std::vector<sprite_chain_element*> Fence::prepareSprites(long long elapsedTime)
{
    return {};
	/*additionalSprites.clear();
	spriteChainElement fullSprite;
	fullSprite.path = "Game/worldSprites/terrainObjects/fence/fence" + std::to_string(typeOfObject) + ".png";
	fullSprite.size = Vector2f(conditionalSizeUnits);
	fullSprite.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(fullSprite);*/
}