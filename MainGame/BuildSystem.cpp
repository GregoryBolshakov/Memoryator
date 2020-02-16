#include "BuildSystem.h"
#include "ObjectInitializer.h"
#include "DroppedLoot.h"

BuildSystem::BuildSystem()
{

}

BuildSystem::~BuildSystem()
{

}

void BuildSystem::init(const std::unordered_map<Tag, CellSprite>& itemsSpriteList)
{
	this->craftIngredientsSpriteList = itemsSpriteList;
	initializeObjectsInfo();

	font.loadFromFile("Fonts/Bebas.ttf");
	successInit = true;
}

void BuildSystem::inventoryBounding(std::vector<HeroBag>* boundBags)
{
	this->boundBags = boundBags;
}

void BuildSystem::initializeObjectsInfo()
{
	std::string objectIconPath, objectImageType;
	std::ifstream fin(BuildSystemObjectsInfoFileDirectory);
	while (fin >> objectIconPath >> objectImageType)
	{
		objectInfo infoItem;

		infoItem.icon = objectIconPath;
		infoItem.type = objectImageType;

		int recipeItemId, recipeItemAmount, recipeLength;

		fin >> recipeLength;
		for (int i = 0; i < recipeLength; i++)
		{
			fin >> recipeItemId >> recipeItemAmount;
			infoItem.recipe.push_back(std::make_pair(Tag(recipeItemId), recipeItemAmount));
		}

		builtObjects.push_back(infoItem);
	}

	fin.close();
}

std::vector <SpriteChainElement> BuildSystem::prepareSprites(GridList& staticGrid, std::vector<WorldObject*> visibleItems, std::map<PackTag, SpritePack>* packsMap)
{
	if (selectedObject == Tag::emptyCell)
		return {};

	StaticObject* terrain = nullptr;
	if (droppedLootIdList.count(selectedObject) > 0)
		terrain = ObjectInitializer::initializeStaticItem(Tag::droppedLoot, mouseWorldPos, int(selectedObject), "", 1, DarkWoods, packsMap);
	else
	{
		if (selectedObject == Tag::totem)
		{
			for (auto item : visibleItems)
			{
				if (staticGrid.getIndexByPoint(item->getPosition().x, item->getPosition().y) != staticGrid.getIndexByPoint(mouseWorldPos.x, mouseWorldPos.y))
					continue;
				bool match = false;
				auto droppedLoot = dynamic_cast<DroppedLoot*>(item);
				if (droppedLoot && droppedLoot->getType() == 201)
				{
					for (auto& cell : droppedLoot->inventory)
						if (cell.first == Tag::hare)
						{
							match = true;
							break;
						}
				}
				if (match)
				{
					buildType = 2;
					break;
				}
			}
		}
		terrain = ObjectInitializer::initializeStaticItem(selectedObject, mouseWorldPos, buildType, "", 1, DarkWoods, packsMap, false);
	}

	auto sprites = terrain->prepareSprites(0);
	canBePlaced = true;

	if (staticGrid.isIntersectWithOthers(terrain, visibleItems, terrain->isDotsAdjusted))
	{
		for (auto& sprite : sprites)
			sprite.color = Color(255,99,71);
		canBePlaced = false;
	}
	else
		for (auto& sprite : sprites)
			sprite.color = Color(127, 255, 0);

	return sprites;
}

void BuildSystem::interact(Vector2f cameraPosition, float scaleFactor)
{
	const Vector2f mousePos = Vector2f(Mouse::getPosition());
	mouseWorldPos = Vector2f((mousePos.x - Helper::GetScreenSize().x / 2 + cameraPosition.x * scaleFactor) / scaleFactor,
		(mousePos.y - Helper::GetScreenSize().y / 2 + cameraPosition.y * scaleFactor) / scaleFactor);
}

void BuildSystem::onMouseUp()
{
	usedMouse = false;

	if (selectedObject != Tag::emptyCell || currentObject != -1)
		usedMouse = true;

	if (selectedObject != Tag::emptyCell && currentObject == -1 && canBePlaced)
	{
		if (spriteBuildPos != Vector2f (-1, -1))
			buildingPosition = spriteBuildPos;
		else
			buildingPosition = mouseWorldPos;
	}
	else
		buildingPosition = Vector2f (-1, -1);
}

void BuildSystem::buildHeldItem(Vector2f focusedObjectPosition, float scaleFactor)
{
	if (heldItem->first == Tag::emptyCell)
	{
		buildingPosition = Vector2f (-1, -1);
		return;
	}

	if (canBePlaced)
	{
		buildingPosition = Vector2f ((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + focusedObjectPosition.x * scaleFactor) / scaleFactor,
			(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + focusedObjectPosition.y*scaleFactor) / scaleFactor);
	}
}

bool BuildSystem::canAfford()
{
	if (currentObject != -1)
	{
		std::vector<std::pair <Tag, int>> temporaryInventory = builtObjects[currentObject].recipe;

		for (auto&curRecipeItem = temporaryInventory.begin(); curRecipeItem != temporaryInventory.end(); ++curRecipeItem)
		{
			for (auto bag = boundBags->begin(); bag != boundBags->end(); bag++)
			{
				bool isBreak = false;
				for (auto&item : bag->cells)
				{
					if (curRecipeItem->first == item.content.first)
					{
						if (item.content.second >= curRecipeItem->second)
						{
							curRecipeItem->second = 0;
							isBreak = true;
							break;
						}
					}
					else
						curRecipeItem->second -= item.content.second;
				}
				if (isBreak)
					break;
			}
			if (curRecipeItem->second > 0)
				return false;
		}

		return true;
	}
	return false;
}

void BuildSystem::wasPlaced()
{
	selectedObject = Tag::emptyCell;
	buildingPosition = Vector2f (-1, -1);
}

void BuildSystem::clearHareBags(int block, GridList& staticGrid, std::vector<WorldObject*>* visibleItems)
{
	for (auto& item : *visibleItems)
	{
		if (staticGrid.getIndexByPoint(item->getPosition().x, item->getPosition().y) != staticGrid.getIndexByPoint(mouseWorldPos.x, mouseWorldPos.y))
			continue;
		bool match = false;
		auto droppedLoot = dynamic_cast<DroppedLoot*>(item);
		if (droppedLoot && droppedLoot->getType() == 201)
		{
			for (auto& cell : droppedLoot->inventory)
				if (cell.first == Tag::hare)
				{
					match = true;
					break;
				}
		}
		if (match)
		{
			item->deletePromiseOn();
			break;
		}
	}
}

void BuildSystem::animator(long long elapsedTime)
{
	if (builtObjects[0].iconSprite.getPosition().x < builtObjects[0].iconSprite.getTextureRect().width / 8 && animationSpeed > 0)
		animationSpeed -= (float)elapsedTime / 400000000;
	else
	{
		animationSpeed = 0;
		for (int i = 0; i < builtObjects.size(); i++)
		{
			builtObjects[i].iconSprite.setPosition(builtObjects[i].iconSprite.getTextureRect().width / 8 + animationSpeed * elapsedTime, builtObjects[i].iconSprite.getPosition().y);
		}
	}
	for (int i = 0; i < builtObjects.size(); i++)
	{
		builtObjects[i].iconSprite.setPosition(builtObjects[i].iconSprite.getPosition().x + animationSpeed * elapsedTime, builtObjects[i].iconSprite.getPosition().y);
	}
}
