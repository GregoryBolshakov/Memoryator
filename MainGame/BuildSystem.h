#pragma once

#include <SFML/Graphics.hpp>

#include "GridList.h"
#include "Helper.h"
#include "InventorySystem.h"
#include "SpritePack.h"
#include "WorldObject.h"

using namespace sf;

struct objectInfo {
	std::string type, icon;
	Sprite sprite, iconSprite;
	Texture iconTexture;
	std::vector<std::pair<Tag, int>> recipe;
};

class BuildSystem
{
public:	
	BuildSystem();
	~BuildSystem();
	void inventoryBounding(std::vector<HeroBag>* boundBags);
	void init();	
	void onMouseUp();
	void buildHeldItem(Vector2f focusedObjectPosition, float scaleFactor);
	void interact(Vector2f cameraPosition = {0, 0}, float scaleFactor = 1);
	void clearHareBags(int block, GridList& staticGrid, std::vector<WorldObject*>* visibleItems);
	void wasPlaced();
	std::vector<SpriteChainElement*> prepareSprites(GridList& staticGrid, const std::vector<WorldObject*>& visibleItems, std::map<PackTag, SpritePack>* packsMap);

	int getCurrentObject() const { return currentObject; }
	bool getUsedMouse() const { return usedMouse; }
	bool getSuccessInit() const { return successInit; }

	void setCurrentObject(int n) { currentObject = n; }
	void setHeldItem(std::pair<Tag, int>& heldItem) { if (nullptr != &heldItem) this->heldItem = &heldItem; }
	
	bool canAfford();

	std::vector<HeroBag>* boundBags;
	bool canBePlaced = false, instantBuild = false;
	Tag selectedObject = Tag::emptyCell;
	Vector2f buildingPosition = { -1, -1 };
	std::map<Tag, bool> droppedLootIdList = { {Tag::heroBag, 1} };
	int buildType = 1;
private:
	//std::unordered_map<Tag, CellSprite> craftIngredientsSpriteList;
	std::pair<Tag, int> *heldItem = nullptr;
	Vector2f mouseWorldPos = { 0, 0 };
	Vector2f spriteBuildPos = Vector2f (-1, -1);
	void initializeObjectsInfo();
	void animator(long long elapsedTime);	
	float originalAnimationSpeed = 0.001f, animationSpeed = 0.001f;
	bool isPressed = false, usedMouse = false, successInit = false;
	std::vector<objectInfo> builtObjects;
	int currentObject = -1;
	std::string BuildSystemObjectsInfoFileDirectory = "Game/buildSystem/buildSystemObjectsInfo.txt";
	Font font;
};
